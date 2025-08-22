#include "web_interface.h"
#include "config.h"
#include <WiFi.h>

String generateStatusHTML() {
    String html = "<div class='status'>";
    html += "<strong>Current Status:</strong><br>";
    html += "WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "<br>";
    html += "IP: " + WiFi.localIP().toString() + "<br>";
    html += "SSID: " + WiFi.SSID() + "<br>";
    html += "</div>";
    return html;
}

String generateACControlForm() {
    String html = "<h2>AC Control</h2>";
    html += "<p>Control your Tadiran air conditioner:</p>";
    html += "<form action='" + String(ENDPOINT_SET) + "' method='get'>";
    
    // Mode selection
    html += "<label>Mode:</label><select name='mode'>";
    html += "<option value='0'>Off</option>";
    html += "<option value='1' selected>Cool</option>";
    html += "<option value='2'>Heat</option>";
    html += "<option value='3'>Circulate</option>";
    html += "<option value='4'>Dry</option>";
    html += "</select>";
    
    // Temperature
    html += "<label>Temperature (°C):</label>";
    html += "<input type='number' name='temp' min='" + String(AC_TEMP_MIN) + "' max='" + String(AC_TEMP_MAX) + "' value='24'>";
    
    // Fan speed
    html += "<label>Fan Speed:</label><select name='fan'>";
    html += "<option value='1' selected>Speed 1</option>";
    html += "<option value='2'>Speed 2</option>";
    html += "<option value='3'>Speed 3</option>";
    html += "<option value='4'>Speed 4</option>";
    html += "</select>";
    
    // Swing
    html += "<label>Swing:</label><select name='swing'>";
    html += "<option value='0' selected>Off</option>";
    html += "<option value='1'>On</option>";
    html += "</select>";
    
    html += "<button type='submit'>Send Command</button>";
    html += "</form>";
    
    return html;
}

String generateQuickActions() {
    String html = "<h3>Quick Actions</h3>";
    html += "<div style='display:flex;gap:10px;flex-wrap:wrap;'>";
    html += "<button onclick='sendCommand(1,24,1,0)'>Cool 24°C</button>";
    html += "<button onclick='sendCommand(2,22,1,0)'>Heat 22°C</button>";
    html += "<button onclick='sendCommand(0,0,1,0)'>Turn Off</button>";
    html += "<button onclick='sendCommand(1,18,4,0)'>Cool 18°C Max Fan</button>";
    html += "</div>";
    return html;
}

String generateSystemControls() {
    String html = "<h2>System</h2>";
    html += "<button onclick='resetWiFi()'>Reset WiFi Settings</button>";
    html += "<button onclick='restart()'>Restart Device</button>";
    return html;
}

String generateJavaScript() {
    String html = "<script>";
    html += "function startConfig() {";
    html += "  if(confirm('This will disconnect from current WiFi and start configuration portal. Continue?')) {";
    html += "    window.location.href='" + String(ENDPOINT_RESET) + "';";
    html += "  }";
    html += "}";
    html += "function resetWiFi() {";
    html += "  if(confirm('This will erase all WiFi settings. Continue?')) {";
    html += "    fetch('" + String(ENDPOINT_RESET) + "?erase=1').then(() => { alert('WiFi settings reset. Device will restart.'); });";
    html += "  }";
    html += "}";
    html += "function restart() {";
    html += "  if(confirm('Restart the device?')) {";
    html += "    fetch('" + String(ENDPOINT_RESET) + "?restart=1').then(() => { alert('Device restarting...'); });";
    html += "  }";
    html += "}";
    html += "function sendCommand(mode, temp, fan, swing) {";
    html += "  const url = `" + String(ENDPOINT_SET) + "?mode=${mode}&temp=${temp}&fan=${fan}&swing=${swing}`;";
    html += "  fetch(url).then(response => response.text()).then(result => {";
    html += "    alert('Command sent: ' + result);";
    html += "  }).catch(error => {";
    html += "    alert('Error: ' + error);";
    html += "  });";
    html += "}";
    html += "</script>";
    return html;
}

String generateConfigPage() {
    String html = "<!DOCTYPE html><html><head><title>AC Controller Config</title>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body{font-family:Arial;margin:20px;background:#f0f0f0;}";
    html += ".container{max-width:600px;margin:0 auto;background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}";
    html += "input,select{width:100%;padding:10px;margin:10px 0;border:1px solid #ddd;border-radius:5px;box-sizing:border-box;}";
    html += "button{background:#007cba;color:white;padding:10px 20px;border:none;border-radius:5px;cursor:pointer;margin:5px;}";
    html += "button:hover{background:#005a87;}";
    html += ".status{background:#e7f3ff;padding:10px;border-radius:5px;margin:10px 0;}";
    html += "</style></head><body><div class='container'>";
    
    html += "<h1>AC Controller Configuration</h1>";
    html += generateStatusHTML();
    
    // WiFi Configuration section
    html += "<h2>WiFi Configuration</h2>";
    html += "<p>To change WiFi settings, click the button below to start the configuration portal.</p>";
    html += "<p><strong>Instructions:</strong></p>";
    html += "<ol>";
    html += "<li>Click 'Start WiFi Config Portal'</li>";
    html += "<li>Connect to WiFi network: <strong>" + String(WIFI_AP_SSID) + "</strong> (password: " + String(WIFI_AP_PASSWORD) + ")</li>";
    html += "<li>Open any website or visit <strong>http://" + String(WIFI_HOSTNAME) + ".local</strong></li>";
    html += "<li>You'll be automatically redirected to the configuration page</li>";
    html += "<li>Enter your WiFi credentials and save</li>";
    html += "</ol>";
    html += "<button onclick='startConfig()'>Start WiFi Config Portal</button>";
    
    html += generateACControlForm();
    html += generateQuickActions();
    html += generateSystemControls();
    html += generateJavaScript();
    
    html += "</div></body></html>";
    return html;
}
