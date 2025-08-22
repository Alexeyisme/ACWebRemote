#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>

#include "config.h"
#ifdef __has_include
    #if __has_include("secrets.h")
        #include "secrets.h"
    #endif
#endif
#include "IRTadiran.h"
#include "web_interface.h"

// Global objects
WiFiManager wifiManager;
WebServer server(WEB_SERVER_PORT);
IRsend irsend(IR_LED_PIN);

// Function declarations
bool connectToWiFi();
void setupWiFiManager();
void setupWebServer();
void acHandler();
void configHandler();
void resetHandler();
void startConfigPortal();
void acAction(int acMode, int acTemp, int acFan = 1, bool acSwing = false);

void setup() {
    // Initialize hardware
    pinMode(IR_LED_PIN, OUTPUT);
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);
    
    Serial.println("AC Web Remote Starting...");
    
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
        
        // Setup mDNS
        if (!MDNS.begin(WIFI_HOSTNAME)) {
            Serial.println("Error setting up MDNS responder!");
            return false;
        }
        Serial.println("mDNS responder started");
        Serial.printf("Address: %s.local\n", WIFI_HOSTNAME);
        MDNS.addService("http", "tcp", WEB_SERVER_PORT);
        
        return true;
    } else {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
}

void acAction(int acMode, int acTemp, int acFan, bool acSwing) {
    bool onOff = (acMode != AC_MODE_OFF);
    
    // Validate parameters
    if (acTemp < AC_TEMP_MIN || acTemp > AC_TEMP_MAX) {
        Serial.printf("Invalid temperature: %d (must be %d-%d)\n", acTemp, AC_TEMP_MIN, AC_TEMP_MAX);
        return;
    }
    
    if (acMode < AC_MODE_MIN || acMode > AC_MODE_MAX) {
        Serial.printf("Invalid mode: %d (must be %d-%d)\n", acMode, AC_MODE_MIN, AC_MODE_MAX);
        return;
    }
    
    if (acFan < AC_FAN_MIN || acFan > AC_FAN_MAX) {
        Serial.printf("Invalid fan speed: %d (must be %d-%d)\n", acFan, AC_FAN_MIN, AC_FAN_MAX);
        return;
    }
    
    Serial.printf("Sending IR command: Power=%s, Mode=%d, Temp=%d, Fan=%d, Swing=%s\n", 
                  onOff ? "ON" : "OFF", acMode, acTemp, acFan, acSwing ? "ON" : "OFF");
    
    IRTadiran ir(&irsend);
    bool success = ir.send(onOff, acMode, acFan, acTemp, acSwing);
    
    if (success) {
        Serial.println("IR command sent successfully");
    } else {
        Serial.println("Failed to send IR command");
    }
}

void acHandler() {
    if (server.hasArg("mode") && server.hasArg("temp")) {
        int mode = server.arg("mode").toInt();
        int temp = server.arg("temp").toInt();
        int fan = server.hasArg("fan") ? server.arg("fan").toInt() : AC_FAN_MIN;
        bool swing = server.hasArg("swing") ? (server.arg("swing").toInt() == 1) : false;
        
        Serial.printf("AC Command: Mode=%d, Temp=%d, Fan=%d, Swing=%s\n", 
                      mode, temp, fan, swing ? "ON" : "OFF");
        
        acAction(mode, temp, fan, swing);
        server.send(200, "text/plain", "AC command sent successfully!");
    } else {
        Serial.println("Incorrect Command - missing required parameters");
        Serial.println("Required: mode, temp");
        Serial.println("Optional: fan, swing");
        server.send(400, "text/plain", "Incorrect request! Required: mode, temp");
    }
}

void configHandler() {
    server.send(200, "text/html", generateConfigPage());
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
