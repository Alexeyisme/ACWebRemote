#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>
#include <Preferences.h>
#include <ArduinoJson.h>

#include "config.h"
#ifdef __has_include
    #if __has_include("secrets.h")
        #include "secrets.h"
    #endif
#endif
#include "ac_controller.h"
#include "IoTWebUI.h"
#include "IoTWebUIManager.h"

// Global objects
WiFiManager wifiManager;
WebServer server(WEB_SERVER_PORT);
IRsend irsend(IR_LED_PIN);
ACController acController(&irsend);
Preferences preferences;
IoTWebUIManager webManager(&server, &preferences, "ACWebRemote", "acconfig");

// Global variables
int currentACModel = DEFAULT_AC_MODEL;

// Function declarations
bool connectToWiFi();
void setupWiFiManager();
void setupWebServer();
void acHandler();
void resetHandler();
void startConfigPortal();

void handleConfigSave(const String& data);
String getConfigValue(const String& key, const String& defaultValue = "");
void setConfigValue(const String& key, const String& value);
void setupCustomNavigation();
String generateHomeContent();
String generateConfigContent();
String generateSensorDataJSON();

void setup() {
    // Initialize hardware
    pinMode(IR_LED_PIN, OUTPUT);
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);
    
    Serial.println("AC Web Remote Starting...");

    // Initialize preferences for configuration storage
    preferences.begin("acconfig", false);
    
    // Load saved AC model
    int savedModel = preferences.getInt("acmodel", DEFAULT_AC_MODEL);
    if (savedModel < 0 || savedModel >= AC_MODEL_COUNT) {
        savedModel = DEFAULT_AC_MODEL;
    }
    currentACModel = savedModel;
    Serial.printf("Loaded AC Model: %d (%s)\n", currentACModel, AC_MODEL_NAMES[currentACModel]);
    
    // Setup WiFi Manager
    setupWiFiManager();
    
    // Try to connect to WiFi
    if (!connectToWiFi()) {
        Serial.println("Failed to connect to WiFi, starting config portal");
        startConfigPortal();
    }
    
    // Set up custom navigation for AC remote
    setupCustomNavigation();
    
    // Set up callbacks for sensor data and configuration
    webManager.setSensorDataCallback(generateSensorDataJSON);
    webManager.setConfigSaveCallback(handleConfigSave);
    webManager.setHomeContentCallback(generateHomeContent);
    webManager.setConfigContentCallback(generateConfigContent);
    
    // Initialize web interface manager AFTER setting up callbacks
    webManager.begin();
    
    // Setup additional web server endpoints (AC-specific endpoints)
    setupWebServer();
    
    Serial.println("AC Web Remote Ready!");
}

void loop() {
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost, attempting to reconnect...");
        if (!connectToWiFi()) {
            Serial.println("Reconnection failed, starting config portal");
            startConfigPortal();
        }
    }
    
    // Handle web server requests
    webManager.handleClient();
}

void setupWiFiManager() {
    wifiManager.setDebugOutput(true);
    wifiManager.setConfigPortalTimeout(WIFI_CONFIG_TIMEOUT);
    wifiManager.setCaptivePortalEnable(true);
    wifiManager.setHostname(WIFI_HOSTNAME);
    
    wifiManager.setAPCallback([](WiFiManager *myWiFiManager) {
        Serial.println("Entered config mode");
        Serial.print("AP IP address: ");
        Serial.println(WiFi.softAPIP());
    });
}

void setupWebServer() {
    // Add AC-specific endpoints (IoTWebUIManager handles common endpoints)
    server.on(ENDPOINT_SET, acHandler);
    server.on(ENDPOINT_RESET, resetHandler);
    
    // Note: IoTWebUIManager already handles:
    // - / (root) - with custom home content
    // - /status - with custom status content  
    // - /api/status - with custom sensor data
    // - /config - with custom config content
    
    // Ensure the HTTP server is actually started
    webManager.startServer();
    Serial.println("Web server started with AC control interface");
}

bool connectToWiFi() {
    Serial.println("Attempting to connect to WiFi...");
    
    if (wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD)) {
        Serial.println("WiFi connected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
        // Setup mDNS (non-fatal if it fails)
        if (!MDNS.begin(WIFI_HOSTNAME)) {
            Serial.println("Error setting up mDNS responder. Continuing without mDNS.");
        } else {
            Serial.println("mDNS responder started");
            Serial.printf("Address: %s.local\n", WIFI_HOSTNAME);
            MDNS.addService("http", "tcp", WEB_SERVER_PORT);
        }
        
        return true;
    } else {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
}

void acHandler() {
    if (server.hasArg("mode") && (server.hasArg("temp") || server.arg("mode").toInt() == AC_MODE_OFF)) {
        // Update current model if provided, otherwise use saved model
        if (server.hasArg("model")) {
            int newModel = server.arg("model").toInt();
            if (newModel < 0) newModel = 0;
            if (newModel >= AC_MODEL_COUNT) newModel = AC_MODEL_COUNT - 1;
            currentACModel = newModel;
            setConfigValue("acmodel", String(currentACModel));
            Serial.printf("AC Model changed to: %d (%s)\n", currentACModel, AC_MODEL_NAMES[currentACModel]);
        }
        
        int mode = server.arg("mode").toInt();
        int temp = server.hasArg("temp") ? server.arg("temp").toInt() : 0;
        int fan = server.hasArg("fan") ? server.arg("fan").toInt() : AC_FAN_MIN;
        bool swing = server.hasArg("swing") ? (server.arg("swing").toInt() == 1) : false;
        
        Serial.printf("AC Command: Model=%d (%s), Mode=%d, Temp=%d, Fan=%d, Swing=%s\n", 
                      currentACModel, AC_MODEL_NAMES[currentACModel], mode, temp, fan, swing ? "ON" : "OFF");
        
        bool success = acController.sendCommand(currentACModel, mode, temp, fan, swing);
        if (success) {
            server.send(200, "text/plain", "AC command sent successfully!");
        } else {
            server.send(500, "text/plain", "Failed to send AC command");
        }
    } else {
        Serial.println("Incorrect Command - missing required parameters");
        Serial.println("Required: mode, temp");
        Serial.println("Optional: model, fan, swing");
        server.send(400, "text/plain", "Incorrect request! Required: mode, temp");
    }
}





void resetHandler() {
    if (server.hasArg("erase") && server.arg("erase") == "1") {
        Serial.println("Erasing WiFi settings...");
        wifiManager.resetSettings();
        server.send(200, "text/plain", "WiFi settings erased. Device will restart.");
        delay(1000);
        ESP.restart();
    } else if (server.hasArg("restart") && server.arg("restart") == "1") {
        Serial.println("Restarting device...");
        server.send(200, "text/plain", "Device restarting...");
        delay(1000);
        ESP.restart();
    } else {
        Serial.println("Starting WiFi configuration portal...");
        server.send(200, "text/plain", "Starting WiFi config portal. Connect to '" + String(WIFI_AP_SSID) + "' WiFi network, then visit http://" + String(WIFI_HOSTNAME) + ".local or any website.");
        delay(1000);
        startConfigPortal();
    }
}



void startConfigPortal() {
    Serial.println("Starting WiFi configuration portal...");
    Serial.println("Connect to WiFi network: " + String(WIFI_AP_SSID));
    Serial.println("Password: " + String(WIFI_AP_PASSWORD));
    Serial.println("Then navigate to: http://" + String(WIFI_HOSTNAME) + ".local or http://192.168.4.1");
    Serial.println("Or simply open any website - you'll be redirected automatically!");
    
    wifiManager.startConfigPortal(WIFI_AP_SSID, WIFI_AP_PASSWORD);
}









void handleConfigSave(const String& data) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    
    if (error) {
        Serial.println("Failed to parse JSON");
        return;
    }
    
    // Save AC model
    if (doc["acmodel"].is<int>()) {
        int acModel = doc["acmodel"].as<int>();
        if (acModel >= 0 && acModel < AC_MODEL_COUNT) {
            currentACModel = acModel;
            setConfigValue("acmodel", String(currentACModel));
            Serial.printf("Saved AC Model: %d (%s)\n", currentACModel, AC_MODEL_NAMES[currentACModel]);
        }
    }
    
    // Save device settings
    if (doc["hostname"].is<String>()) {
        String hostname = doc["hostname"].as<String>();
        setConfigValue("hostname", hostname);
        Serial.println("Saved hostname: " + hostname);
    }
    
    if (doc["ap_ssid"].is<String>()) {
        String apSSID = doc["ap_ssid"].as<String>();
        setConfigValue("ap_ssid", apSSID);
        Serial.println("Saved AP SSID: " + apSSID);
    }
    
    if (doc["ap_password"].is<String>()) {
        String apPassword = doc["ap_password"].as<String>();
        setConfigValue("ap_password", apPassword);
        Serial.println("Saved AP password: [HIDDEN]");
    }
    
    if (doc["wifi_ssid"].is<String>()) {
        String wifiSSID = doc["wifi_ssid"].as<String>();
        setConfigValue("wifi_ssid", wifiSSID);
        Serial.println("Saved WiFi SSID: " + wifiSSID);
    }
    
    if (doc["wifi_password"].is<String>()) {
        String wifiPassword = doc["wifi_password"].as<String>();
        setConfigValue("wifi_password", wifiPassword);
        Serial.println("Saved WiFi password: [HIDDEN]");
    }
    
    Serial.println("Configuration saved successfully");
}

String getConfigValue(const String& key, const String& defaultValue) {
    return preferences.getString(key.c_str(), defaultValue);
}

void setConfigValue(const String& key, const String& value) {
    preferences.putString(key.c_str(), value);
}

// ===== SENSOR DATA GENERATOR =====

String generateSensorDataJSON() {
    JsonDocument doc;
    
    doc["timestamp"] = millis();
    doc["status"] = "running";
    
    // AC control data
    doc["ac"]["current_model"] = currentACModel;
    doc["ac"]["model_name"] = String(AC_MODEL_NAMES[currentACModel]);
    
    // System info
    doc["system"]["uptime"] = millis();
    doc["system"]["free_heap"] = ESP.getFreeHeap();
    doc["system"]["wifi_rssi"] = WiFi.RSSI();
    doc["system"]["wifi_connected"] = WiFi.status() == WL_CONNECTED;
    doc["system"]["ip_address"] = WiFi.localIP().toString();
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

// ===== CUSTOM NAVIGATION SETUP =====

void setupCustomNavigation() {
    webManager.setNavLabels("AC Control", "System Status", "Configuration");
    webManager.setCustomNavigationLinks("");
}

// ===== CUSTOM CONTENT GENERATORS =====

String generateHomeContent() {
    String content = "";

    // Create comprehensive AC control form
    String formContent = "<form id='ac-control-form'>";
    
    // AC Model Selection
    String modelSelect = "<select id='ac_model' name='model'>";
    for (int i = 0; i < AC_MODEL_COUNT; i++) {
        modelSelect += "<option value='" + String(i) + "'";
        if (i == currentACModel) modelSelect += " selected";
        modelSelect += ">" + String(AC_MODEL_NAMES[i]) + "</option>";
    }
    modelSelect += "</select>";
    formContent += IoTWebUI::getFormGroup("AC Model", modelSelect, "select");
    
    // Mode Selection
    String modeSelect = "<select id='ac_mode' name='mode'>";
    modeSelect += "<option value='0'>Off</option>";
    modeSelect += "<option value='1' selected>Cool</option>";
    modeSelect += "<option value='2'>Heat</option>";
    modeSelect += "<option value='3'>Fan</option>";
    modeSelect += "<option value='4'>Dry</option>";
    modeSelect += "</select>";
    formContent += IoTWebUI::getFormGroup("Mode", modeSelect, "select");
    
    // Temperature Input
    formContent += IoTWebUI::getFormGroup("Temperature (°C)", "", "number", "24", "16-30°C");
    
    // Fan Speed Selection
    String fanSelect = "<select id='ac_fan' name='fan'>";
    fanSelect += "<option value='1' selected>Low</option>";
    fanSelect += "<option value='2'>Medium</option>";
    fanSelect += "<option value='3'>High</option>";
    fanSelect += "<option value='4'>Max</option>";
    fanSelect += "</select>";
    formContent += IoTWebUI::getFormGroup("Fan Speed", fanSelect, "select");
    
    // Swing Control
    String swingSelect = "<select id='ac_swing' name='swing'>";
    swingSelect += "<option value='0' selected>Off</option>";
    swingSelect += "<option value='1'>On</option>";
    swingSelect += "</select>";
    formContent += IoTWebUI::getFormGroup("Swing", swingSelect, "select");
    
    // Control Buttons
    formContent += IoTWebUI::getButton("Send Command", "primary", "sendACCommand()");
    formContent += IoTWebUI::getButton("Quick Off", "secondary", "quickOff()");
    formContent += IoTWebUI::getButton("Quick Cool 24°", "secondary", "quickCool()");
    formContent += IoTWebUI::getButton("Quick Heat 22°", "secondary", "quickHeat()");
    
    formContent += "</form>";
    
    // Add JavaScript for form handling
    formContent += "<script>";
    formContent += "function sendACCommand() {";
    formContent += "  const model = document.getElementById('ac_model').value;";
    formContent += "  const mode = document.getElementById('ac_mode').value;";
    formContent += "  const temp = document.getElementById('temperature_c').value;";
    formContent += "  const fan = document.getElementById('ac_fan').value;";
    formContent += "  const swing = document.getElementById('ac_swing').value;";
    formContent += "  ";
    formContent += "  const url = `/set?model=${model}&mode=${mode}&temp=${temp}&fan=${fan}&swing=${swing}`;";
    formContent += "  ";
    formContent += "  fetch(url)";
    formContent += "    .then(response => response.text())";
    formContent += "    .then(data => {";
    formContent += "      alert('Command sent successfully!');";
    formContent += "    })";
    formContent += "    .catch(error => {";
    formContent += "      alert('Error sending command: ' + error);";
    formContent += "    });";
    formContent += "}";
    formContent += "";
    formContent += "function quickOff() {";
    formContent += "  const model = document.getElementById('ac_model').value;";
    formContent += "  fetch(`/set?model=${model}&mode=0&temp=0`)";
    formContent += "    .then(response => response.text())";
    formContent += "    .then(data => alert('AC turned off!'))";
    formContent += "    .catch(error => alert('Error: ' + error));";
    formContent += "}";
    formContent += "";
    formContent += "function quickCool() {";
    formContent += "  const model = document.getElementById('ac_model').value;";
    formContent += "  fetch(`/set?model=${model}&mode=1&temp=24&fan=2&swing=0`)";
    formContent += "    .then(response => response.text())";
    formContent += "    .then(data => alert('Cool mode activated!'))";
    formContent += "    .catch(error => alert('Error: ' + error));";
    formContent += "}";
    formContent += "";
    formContent += "function quickHeat() {";
    formContent += "  const model = document.getElementById('ac_model').value;";
    formContent += "  fetch(`/set?model=${model}&mode=2&temp=22&fan=2&swing=0`)";
    formContent += "    .then(response => response.text())";
    formContent += "    .then(data => alert('Heat mode activated!'))";
    formContent += "    .catch(error => alert('Error: ' + error));";
    formContent += "}";
    formContent += "</script>";

    content += IoTWebUI::getSection("AC Control", formContent);

    // Add system status (generic key/value list)
    String statusLabels[] = {"Current Model", "WiFi Status", "IP Address", "Free Heap"};
    String statusValues[] = {
        String(AC_MODEL_NAMES[currentACModel]),
        WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected",
        WiFi.localIP().toString(),
        String(ESP.getFreeHeap()) + " bytes"
    };
    content += IoTWebUI::getKeyValueList(statusLabels, statusValues, 4, "System Status");

    return content;
}

String generateConfigContent() {
    String content = "";

    // Get current values
    String currentHostname = getConfigValue("hostname", WIFI_HOSTNAME);
    String currentAPSSID = getConfigValue("ap_ssid", WIFI_AP_SSID);
    String currentAPPassword = getConfigValue("ap_password", WIFI_AP_PASSWORD);
    String currentWiFiSSID = getConfigValue("wifi_ssid", "");
    String currentWiFiPassword = getConfigValue("wifi_password", "");

    // Wrap in a form to let IoTWebUIManager JS handle submit
    content += "<form id='config-form'>";

    // AC Model Section
    String acModelSelect = "<select id='acmodel' name='acmodel'>";
    for (int i = 0; i < AC_MODEL_COUNT; i++) {
        acModelSelect += "<option value='" + String(i) + "'";
        if (i == currentACModel) acModelSelect += " selected";
        acModelSelect += ">" + String(AC_MODEL_NAMES[i]) + "</option>";
    }
    acModelSelect += "</select>";
    String acSection = IoTWebUI::getFormGroup("AC Model", acModelSelect, "select");
    content += IoTWebUI::getSection("AC Settings", acSection);

    // Home WiFi Section
    String wifiSection = IoTWebUI::getFormGroup("WiFi SSID", "", "text", currentWiFiSSID, "Your home WiFi network name");
    wifiSection += IoTWebUI::getFormGroup("WiFi Password", "", "password", currentWiFiPassword, "Your home WiFi password");
    content += IoTWebUI::getSection("Home WiFi Settings", wifiSection);

    // Device Settings Section
    String deviceSection = IoTWebUI::getFormGroup("Device Hostname", "", "text", currentHostname);
    deviceSection += IoTWebUI::getFormGroup("Access Point SSID", "", "text", currentAPSSID);
    deviceSection += IoTWebUI::getFormGroup("Access Point Password", "", "password", currentAPPassword);
    content += IoTWebUI::getSection("Device Settings", deviceSection);

    // Action buttons
    content += IoTWebUI::getButton("Save Configuration", "primary");
    content += IoTWebUI::getButton("Back to Home", "secondary", "window.location.href=\"/\"");

    content += "</form>";
    
    // Reset WiFi button - simple link to reset endpoint with confirmation
    content += IoTWebUI::getButton("Reset WiFi", "danger", "if(confirm(&quot;Are you sure?&quot;)){window.location.href=&quot;/reset?erase=1&quot;}" );

    return content;
}
