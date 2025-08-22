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

input_number:
  ac_temperature:
    name: AC Temperature
    initial: 22
    min: 16
    max: 30
    step: 1
    mode: slider

rest_command:
  set_ac_control:
    url: "http://accontrol.local/set?mode={{ mode }}&temp={{ temp }}"
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
GET /set?mode={mode}&temp={temp}&fan={fan}&swing={swing}
```

**Parameters:**
- `mode`: 0=Off, 1=Cool, 2=Heat, 3=Circulate, 4=Dry
- `temp`: Temperature (16-30°C)
- `fan`: Fan speed (1-4, optional, default=1)
- `swing`: Swing mode (0=Off, 1=On, optional, default=0)

**Examples:**
```bash
# Turn on cool mode at 24°C
curl "http://accontrol.local/set?mode=1&temp=24"

# Turn on heat mode at 22°C with fan speed 3
curl "http://accontrol.local/set?mode=2&temp=22&fan=3"

# Turn off AC
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

This project uses the [IRTadiran library](https://github.com/arikfe/IRTadiran) by [arikfe](https://github.com/arikfe) and supports:
- Tadiran air conditioners (common in Israel)
- All standard modes: Cool, Heat, Circulate, Dry
- Temperature range: 16-30°C
- Fan speeds: 1-4
- Swing control

**Note**: The IRTadiran library is licensed under MIT License, which allows for commercial use, modification, distribution, and private use.

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
│   └── web_interface.h       # Web interface header
├── lib/
│   └── IRTadiran/            # IR protocol library
├── platformio.ini            # PlatformIO configuration
├── README.md                 # This file
├── HOME_ASSISTANT_INTEGRATION.md  # Home Assistant guide
└── LICENSE                   # MIT License
```

### Dependencies
- `IRremoteESP8266` - IR signal generation
- `WiFiManager` - WiFi configuration
- `ArduinoJson` - JSON handling
- `IRTadiran` - Tadiran AC protocol

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
