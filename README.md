# AC Web Remote - ESP32 Air Conditioner Controller

[![PlatformIO CI](https://github.com/Alexeyisme/ACWebRemote/workflows/PlatformIO%20CI/badge.svg)](https://github.com/Alexeyisme/ACWebRemote/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-brightgreen.svg)](https://www.home-assistant.io/)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp32)

A professional web-based remote control for Tadiran air conditioners using ESP32 and IR LED. **Perfect for Home Assistant integration!**

## Features

- 🌐 **Web Interface**: Control your AC from any device via web browser
- 🏠 **Home Assistant Integration**: Seamless smart home automation
- 📱 **Mobile Friendly**: Responsive design works on phones and tablets
- ⌚ **Apple Watch Support**: Quick actions from your wrist
- 🔧 **WiFi Manager**: Easy WiFi configuration without reflashing
- 🎛️ **Full Control**: Power, mode, temperature, fan speed, and swing
- 🚀 **Quick Actions**: One-click presets for common settings
- 🔒 **Captive Portal**: Automatic redirection for easy setup
- 📡 **mDNS**: Access via `http://accontrol.local`
- 🤖 **Automation Ready**: Temperature-based and schedule-based control
- 🎯 **38+ AC Models**: Support for major brands (Daikin, Mitsubishi, Panasonic, etc.)
- 💾 **Persistent Model Selection**: Set your AC model once, use simple commands
- 🔄 **Smart Command Handling**: Automatic model persistence and fallback

## Hardware Requirements

- ESP32 development board
- IR LED (connected to GPIO 33)
- Power supply (USB or external)

## Installation

### 1. Clone and Setup
```bash
git clone https://github.com/yourusername/ACWebRemote.git
cd ACWebRemote
```

### 2. Configuration (Optional)
```bash
# Copy the secrets template and customize if needed
cp src/secrets.h.template src/secrets.h
# Edit src/secrets.h with your preferred settings
```

### 3. Build and Upload
```bash
pio run --target upload
```

### 4. First Time Setup
1. Power on the device
2. Connect to WiFi network: `ACWebRemote` (password: `12345678`)
3. Open any website or visit `http://accontrol.local`
4. You'll be automatically redirected to the configuration page
5. Enter your WiFi credentials and save

### 5. Home Assistant Integration (Optional)
If you're using Home Assistant, check out the **[complete integration guide](HOME_ASSISTANT_INTEGRATION.md)** for:
- Beautiful UI cards and dashboards
- Apple Watch quick actions
- Advanced automations
- Voice control integration

## Usage

### Web Interface
- **Main Control**: Visit `http://accontrol.local/config`
- **Direct API**: `http://accontrol.local/set?mode=1&temp=24&fan=2&swing=0`

### API Documentation
For complete API reference and examples, see **[📖 API Documentation](API_DOCUMENTATION.md)**

## 🏠 Home Assistant Integration

This project works seamlessly with Home Assistant! Check out the complete integration guide:

**[📖 Home Assistant Integration Guide](HOME_ASSISTANT_INTEGRATION.md)**

### **✨ Home Assistant Features**
- **🖥️ Beautiful UI**: Responsive cards with sliders and buttons
- **⌚ Apple Watch Support**: Quick actions from your wrist
- **🤖 Full Automation**: Temperature-based and schedule-based control
- **📱 Mobile Friendly**: Works perfectly on phones and tablets
- **🎯 Voice Control**: Siri and Google Assistant integration
- **📊 Energy Monitoring**: Track usage and costs
- **🏠 Smart Home**: Integrate with presence detection and sensors

### **Quick Home Assistant Setup**
```yaml
# configuration.yaml
input_select:
  ac_mode:
    name: AC Mode
    options: ["0: Off", "1: Cool", "2: Heat", "3: Fan", "4: Dry"]
    initial: "0: Off"
  
  ac_model:
    name: AC Model
    options: 
      - "0: Tadiran"
      - "4: Daikin"
      - "6: Daikin216"
      - "24: Mitsubishi AC"
      - "29: Panasonic AC"
      - "31: Samsung AC"
    initial: "0: Tadiran"

input_number:
  ac_temperature:
    name: AC Temperature
    initial: 22
    min: 16
    max: 30
    step: 1
    mode: slider

rest_command:
  # Set AC model (run once to configure)
  set_ac_model:
    url: "http://accontrol.local/set?model={{ model }}&mode=1&temp=24"
    method: "GET"
  
  # Simple commands (uses saved model)
  set_ac_control:
    url: "http://accontrol.local/set?mode={{ mode }}&temp={{ temp }}"
    method: "GET"
  
  # Commands with model override
  set_ac_control_with_model:
    url: "http://accontrol.local/set?model={{ model }}&mode={{ mode }}&temp={{ temp }}&fan={{ fan }}&swing={{ swing }}"
    method: "GET"
```

### **🍎 Apple Watch & Voice Control**
Create quick scripts for Apple Watch and voice assistants:
- **AC Off** - Quick power off
- **AC Cool 20°C** - Comfortable cooling
- **AC Heat 22°C** - Warm heating
- **AC Dry 20°C** - Dehumidification
- **AC Eco Mode** - Energy saving
- **AC Sleep Mode** - Night comfort

### **🤖 Advanced Automations**
- **Temperature-based control**: Auto-cool when room is hot
- **Schedule-based**: Morning and evening routines
- **Presence detection**: Turn off when leaving home
- **Energy optimization**: Smart temperature management

### **📱 Mobile Dashboard**
- **Mushroom Cards**: Beautiful, modern UI
- **Quick Actions**: One-tap presets
- **Real-time Control**: Instant response
- **Status Monitoring**: Current AC state

### API Endpoints

#### AC Control
```
GET /set?mode={mode}&temp={temp}&fan={fan}&swing={swing}&model={model}
```

**Parameters:**
- `mode`: 0=Off, 1=Cool, 2=Heat, 3=Circulate, 4=Dry
- `temp`: Temperature (16-30°C)
- `fan`: Fan speed (1-4, optional, default=1)
- `swing`: Swing mode (0=Off, 1=On, optional, default=0)
- `model`: AC model ID (0-37, optional, see supported models below)

**💾 Persistent Model Selection:**
- **First request**: Include `model` parameter to set your AC model
- **Subsequent requests**: Omit `model` parameter to use the saved model
- **Change model**: Include `model` parameter anytime to switch models

**Examples:**
```bash
# Set model to Daikin (model 4) and turn on cool mode at 24°C
curl "http://accontrol.local/set?model=4&mode=1&temp=24"

# Use saved model (Daikin) for subsequent commands
curl "http://accontrol.local/set?mode=1&temp=22"
curl "http://accontrol.local/set?mode=2&temp=26&fan=3"

# Change to Mitsubishi AC (model 6) and send command
curl "http://accontrol.local/set?model=6&mode=1&temp=25"

# Use saved model (Mitsubishi) for next command
curl "http://accontrol.local/set?mode=0&temp=0"
```

#### Configuration
- `GET /config` - Configuration web interface
- `GET /reset` - Start WiFi configuration portal
- `GET /reset?erase=1` - Reset WiFi settings
- `GET /reset?restart=1` - Restart device

### Quick Actions
The web interface includes one-click buttons for common settings:
- **Cool 24°C**: Standard cooling
- **Heat 22°C**: Standard heating
- **Turn Off**: Power off AC
- **Cool 18°C Max Fan**: Maximum cooling

## Supported AC Models

This project supports **38+ AC models** including major brands worldwide:

### **🎯 Supported Brands & Models:**
- **Tadiran** (0) - Original implementation
- **Carrier** (1-3) - AC64, AC84, AC128
- **Daikin** (4-13) - Standard + 9 variants (Daikin2, Daikin216, Daikin64, etc.)
- **Fujitsu** (14) - Fujitsu AC
- **Gree** (15) - Gree AC
- **Hitachi** (16-21) - Standard + 5 variants (HitachiAC1-4, HitachiAC424)
- **Kelvinator** (22) - Kelvinator AC
- **Midea** (23) - Midea AC
- **Mitsubishi** (24-28) - Standard + 4 variants (Mitsubishi136, Mitsubishi112, Heavy88, Heavy152)
- **Panasonic** (29-30) - Standard + PanasonicAC32
- **Samsung** (31) - Samsung AC
- **Sharp** (32) - Sharp AC
- **TCL** (33) - TCL 112AC
- **Toshiba** (34) - Toshiba AC
- **Trotec** (35) - Trotec
- **Vestel** (36) - Vestel AC
- **Whirlpool** (37) - Whirlpool AC

### **🔧 Model Selection:**
1. **Web Interface**: Select your AC model from the dropdown
2. **API**: Use `model` parameter (0-37) in API calls
3. **Persistent**: Model selection is saved until changed
4. **Fallback**: Unknown models default to Tadiran

### **📋 All Supported Models:**
```
0:  Tadiran (Current)
1:  Carrier AC64
2:  Carrier AC84  
3:  Carrier AC128
4:  Daikin
5:  Daikin2
6:  Daikin216
7:  Daikin64
8:  Daikin128
9:  Daikin152
10: Daikin160
11: Daikin176
12: Daikin200
13: Daikin312
14: Fujitsu AC
15: Gree AC
16: Hitachi AC
17: Hitachi AC1
18: Hitachi AC2
19: Hitachi AC3
20: Hitachi AC4
21: Hitachi AC424
22: Kelvinator AC
23: Midea AC
24: Mitsubishi AC
25: Mitsubishi 136
26: Mitsubishi 112
27: Mitsubishi Heavy 88
28: Mitsubishi Heavy 152
29: Panasonic AC
30: Panasonic AC32
31: Samsung AC
32: Sharp AC
33: TCL 112AC
34: Toshiba AC
35: Trotec
36: Vestel AC
37: Whirlpool AC
```

### **🎛️ Standard Features:**
- **Modes**: Cool, Heat, Circulate, Dry, Off
- **Temperature**: 16-30°C range
- **Fan Speeds**: 1-4 levels
- **Swing Control**: On/Off
- **IR Protocols**: Based on IRremoteESP8266 library

**Note**: The original [IRTadiran library](https://github.com/arikfe/IRTadiran) by [arikfe](https://github.com/arikfe) is licensed under MIT License and is used for Tadiran AC units.

## Troubleshooting

### WiFi Issues
1. **Can't connect to setup network**: Check if device is powered and LED is blinking
2. **Configuration portal not working**: Try visiting `http://192.168.4.1` directly
3. **WiFi credentials not saved**: Use `/reset?erase=1` to clear settings and try again

### AC Control Issues
1. **IR commands not working**: Check IR LED connection to GPIO 33
2. **Wrong temperature**: Verify your AC supports the temperature range
3. **Mode not working**: Check if your AC model supports the selected mode

### Device Issues
1. **Device not responding**: Check power supply and USB connection
2. **Web interface not loading**: Try accessing via IP address instead of hostname
3. **Serial output**: Connect via USB and check Serial Monitor at 115200 baud

## Development

### Project Structure
```
ACWebRemote/
├── src/
│   ├── main.cpp              # Main application
│   ├── config.h              # Configuration constants
│   ├── web_interface.cpp     # Web UI generation
│   ├── web_interface.h       # Web interface header
│   ├── ac_controller.cpp     # AC model control logic
│   └── ac_controller.h       # AC controller header
├── lib/
│   └── IRTadiran/            # IR protocol library
├── platformio.ini            # PlatformIO configuration
├── README.md                 # This file
├── API_DOCUMENTATION.md      # Complete API reference
├── HOME_ASSISTANT_INTEGRATION.md  # Home Assistant guide
└── LICENSE                   # MIT License
```

### Dependencies
- `IRremoteESP8266` - IR signal generation for 38+ AC models
- `WiFiManager` - WiFi configuration
- `ArduinoJson` - JSON handling
- `IRTadiran` - Tadiran AC protocol (original implementation)

### Building
```bash
# Build project
pio run

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Third-Party Libraries

This project uses the following third-party libraries:

- **[IRTadiran](https://github.com/arikfe/IRTadiran)** by [arikfe](https://github.com/arikfe) - MIT License
- **WiFiManager** by [tzapu](https://github.com/tzapu) - MIT License  
- **IRremoteESP8266** by [crankyoldgit](https://github.com/crankyoldgit) - GPL-3.0 License
- **ArduinoJson** by [bblanchon](https://github.com/bblanchon) - MIT License

All libraries are used in compliance with their respective licenses.

## Contributing

Feel free to submit issues and enhancement requests!

## Acknowledgments

- **[arikfe/IRTadiran](https://github.com/arikfe/IRTadiran)** - Original IR protocol implementation for Tadiran AC units (MIT License)
- [tzapu/WiFiManager](https://github.com/tzapu/WiFiManager) - WiFi configuration library
- [crankyoldgit/IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) - IR signal generation library
