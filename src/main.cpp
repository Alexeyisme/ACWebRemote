#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>
#include <Preferences.h>

#include "config.h"
#ifdef __has_include
    #if __has_include("secrets.h")
        #include "secrets.h"
    #endif
#endif
#include "ac_controller.h"
#include "web_interface.h"

// Global objects
WiFiManager wifiManager;
WebServer server(WEB_SERVER_PORT);
IRsend irsend(IR_LED_PIN);
ACController acController(&irsend);
Preferences preferences;

// Global variables
int currentACModel = DEFAULT_AC_MODEL;

// Function declarations
bool connectToWiFi();
void setupWiFiManager();
void setupWebServer();
void acHandler();
void configHandler();
void resetHandler();
void startConfigPortal();

void setup() {
    // Initialize hardware
    pinMode(IR_LED_PIN, OUTPUT);
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);
    
    Serial.println("AC Web Remote Starting...");

    // Load saved AC model from NVS
    preferences.begin("acwebremote", false);
    int savedModel = preferences.getInt("acmodel", DEFAULT_AC_MODEL);
    if (savedModel < 0 || savedModel >= AC_MODEL_COUNT) {
        savedModel = DEFAULT_AC_MODEL;
    }
    currentACModel = savedModel;
    Serial.printf("Loaded AC Model from NVS: %d\n", currentACModel);
    
    // Setup WiFi Manager
    setupWiFiManager();
    
    // Try to connect to WiFi
    if (!connectToWiFi()) {
        Serial.println("Failed to connect to WiFi, starting config portal");
        startConfigPortal();
    }
    
    // Setup web server
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
    server.handleClient();
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
    server.on(ENDPOINT_SET, acHandler);
    server.on(ENDPOINT_CONFIG, configHandler);
    server.on(ENDPOINT_RESET, resetHandler);
    server.begin();
    Serial.println("Web server started");
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
            preferences.putInt("acmodel", currentACModel);
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

void configHandler() {
    server.send(200, "text/html", generateConfigPage(currentACModel));
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
