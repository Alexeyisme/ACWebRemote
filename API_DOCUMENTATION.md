# AC Web Remote - API Documentation

## Overview

The AC Web Remote provides a REST API for controlling air conditioners via HTTP requests. The API supports **38+ AC models** with **persistent model selection** for convenient automation.

## Base URL

```
http://accontrol.local
```
or
```
http://[device-ip-address]
```

## Authentication

No authentication required - the API is designed for local network use.

## Endpoints

### 1. AC Control

**Endpoint:** `GET /set`

**Description:** Control your air conditioner with mode, temperature, fan, swing, and model selection.

**Parameters:**
- `mode` (required): AC operation mode
  - `0` = Off
  - `1` = Cool
  - `2` = Heat
  - `3` = Circulate/Fan
  - `4` = Dry
- `temp` (required): Temperature in Celsius (16-30°C)
- `fan` (optional): Fan speed (1-4, default: 1)
- `swing` (optional): Swing mode (0=Off, 1=On, default: 0)
- `model` (optional): AC model ID (0-37, see supported models below)

**💾 Persistent Model Selection:**
- **First request**: Include `model` parameter to set your AC model
- **Subsequent requests**: Omit `model` parameter to use the saved model
- **Change model**: Include `model` parameter anytime to switch models

**Response:**
- `200 OK`: Command sent successfully
- `400 Bad Request`: Missing required parameters
- `500 Internal Server Error`: Failed to send command

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

# Full command with all parameters
curl "http://accontrol.local/set?model=4&mode=1&temp=24&fan=2&swing=1"
```

### 2. Configuration Interface

**Endpoint:** `GET /config`

**Description:** Web-based configuration interface for AC control and WiFi settings.

**Response:** HTML page with:
- Current AC model status
- AC control form with model selection
- Quick action buttons
- WiFi configuration options
- System controls

### 3. WiFi Configuration Portal

**Endpoint:** `GET /reset`

**Description:** Start the WiFi configuration portal.

**Parameters:**
- `erase=1` (optional): Erase saved WiFi settings
- `restart=1` (optional): Restart the device

**Examples:**
```bash
# Start WiFi config portal
curl "http://accontrol.local/reset"

# Erase WiFi settings and start portal
curl "http://accontrol.local/reset?erase=1"

# Restart device
curl "http://accontrol.local/reset?restart=1"
```

## Supported AC Models

### Model ID Reference

| ID | Brand | Model | Description |
|----|-------|-------|-------------|
| 0 | Tadiran | Current | Original implementation |
| 1 | Carrier | AC64 | Carrier AC64 protocol |
| 2 | Carrier | AC84 | Carrier AC84 protocol |
| 3 | Carrier | AC128 | Carrier AC128 protocol |
| 4 | Daikin | Standard | Daikin standard protocol |
| 5 | Daikin | Daikin2 | Daikin2 protocol |
| 6 | Daikin | Daikin216 | Daikin216 protocol |
| 7 | Daikin | Daikin64 | Daikin64 protocol |
| 8 | Daikin | Daikin128 | Daikin128 protocol |
| 9 | Daikin | Daikin152 | Daikin152 protocol |
| 10 | Daikin | Daikin160 | Daikin160 protocol |
| 11 | Daikin | Daikin176 | Daikin176 protocol |
| 12 | Daikin | Daikin200 | Daikin200 protocol |
| 13 | Daikin | Daikin312 | Daikin312 protocol |
| 14 | Fujitsu | AC | Fujitsu AC protocol |
| 15 | Gree | AC | Gree AC protocol |
| 16 | Hitachi | AC | Hitachi standard protocol |
| 17 | Hitachi | AC1 | Hitachi AC1 protocol |
| 18 | Hitachi | AC2 | Hitachi AC2 protocol |
| 19 | Hitachi | AC3 | Hitachi AC3 protocol |
| 20 | Hitachi | AC4 | Hitachi AC4 protocol |
| 21 | Hitachi | AC424 | Hitachi AC424 protocol |
| 22 | Kelvinator | AC | Kelvinator AC protocol |
| 23 | Midea | AC | Midea AC protocol |
| 24 | Mitsubishi | AC | Mitsubishi standard protocol |
| 25 | Mitsubishi | 136 | Mitsubishi 136 protocol |
| 26 | Mitsubishi | 112 | Mitsubishi 112 protocol |
| 27 | Mitsubishi | Heavy88 | Mitsubishi Heavy 88 protocol |
| 28 | Mitsubishi | Heavy152 | Mitsubishi Heavy 152 protocol |
| 29 | Panasonic | AC | Panasonic standard protocol |
| 30 | Panasonic | AC32 | Panasonic AC32 protocol |
| 31 | Samsung | AC | Samsung AC protocol |
| 32 | Sharp | AC | Sharp AC protocol |
| 33 | TCL | 112AC | TCL 112AC protocol |
| 34 | Toshiba | AC | Toshiba AC protocol |
| 35 | Trotec | Standard | Trotec protocol |
| 36 | Vestel | AC | Vestel AC protocol |
| 37 | Whirlpool | AC | Whirlpool AC protocol |

## Usage Examples

### Home Assistant Integration

```yaml
# configuration.yaml
rest_command:
  # Set AC model once (configure your AC)
  set_ac_model:
    url: "http://accontrol.local/set?model=4&mode=1&temp=24"
    method: "GET"
  
  # Simple commands (uses saved model)
  ac_cool:
    url: "http://accontrol.local/set?mode=1&temp={{ temp }}"
    method: "GET"
  
  ac_heat:
    url: "http://accontrol.local/set?mode=2&temp={{ temp }}"
    method: "GET"
  
  ac_off:
    url: "http://accontrol.local/set?mode=0&temp=0"
    method: "GET"
  
  # Full control with model override
  ac_full_control:
    url: "http://accontrol.local/set?model={{ model }}&mode={{ mode }}&temp={{ temp }}&fan={{ fan }}&swing={{ swing }}"
    method: "GET"
```

### Node.js/JavaScript

```javascript
const axios = require('axios');

// Set AC model to Daikin
await axios.get('http://accontrol.local/set?model=4&mode=1&temp=24');

// Use saved model for subsequent commands
await axios.get('http://accontrol.local/set?mode=1&temp=22');
await axios.get('http://accontrol.local/set?mode=2&temp=26&fan=3');
```

### Python

```python
import requests

# Set AC model to Mitsubishi
requests.get('http://accontrol.local/set?model=24&mode=1&temp=24')

# Use saved model for subsequent commands
requests.get('http://accontrol.local/set?mode=1&temp=22')
requests.get('http://accontrol.local/set?mode=2&temp=26&fan=3')
```

### Shell Scripts

```bash
#!/bin/bash

# Set AC model to Panasonic
curl "http://accontrol.local/set?model=29&mode=1&temp=24"

# Use saved model for automation
curl "http://accontrol.local/set?mode=1&temp=22"
curl "http://accontrol.local/set?mode=0&temp=0"
```

## Error Handling

### Common Error Responses

**400 Bad Request:**
```json
{
  "error": "Missing required parameters: mode, temp"
}
```

**500 Internal Server Error:**
```json
{
  "error": "Failed to send AC command"
}
```

### Troubleshooting

1. **Device not responding**: Check if device is powered and connected to WiFi
2. **Wrong AC model**: Verify model ID in the supported models list
3. **Commands not working**: Check IR LED connection and AC compatibility
4. **Network issues**: Try accessing via IP address instead of hostname

## Rate Limiting

No rate limiting is implemented. However, it's recommended to:
- Wait 1-2 seconds between commands
- Avoid sending commands faster than your AC can process them
- Use appropriate delays in automation scripts

## Security Considerations

- The API is designed for local network use only
- No authentication is required
- Consider network security for production deployments
- Use HTTPS if exposing to external networks (requires additional setup)

## Version History

- **v2.0**: Added persistent AC model selection and 38+ AC models
- **v1.0**: Original Tadiran-only implementation
