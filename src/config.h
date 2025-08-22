// Configuration file for AC Web Remote
// This file contains default configuration values
// For sensitive data, create src/secrets.h and it will be used instead

#ifndef CONFIG_H
#define CONFIG_H

// Hardware Configuration
#define IR_LED_PIN 33
#define SERIAL_BAUD_RATE 115200

// WiFi Configuration
#define WIFI_HOSTNAME "accontrol"
#define WIFI_AP_SSID "ACWebRemote"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_CONFIG_TIMEOUT 180  // 3 minutes

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define ENDPOINT_SET "/set"
#define ENDPOINT_CONFIG "/config"
#define ENDPOINT_RESET "/reset"

// AC Control Limits
#define AC_TEMP_MIN 16
#define AC_TEMP_MAX 30
#define AC_FAN_MIN 1
#define AC_FAN_MAX 4
#define AC_MODE_MIN 0
#define AC_MODE_MAX 4

// IR Protocol Constants
#define IR_BUFFER_SIZE 264
#define IR_FREQUENCY 38

// AC Modes
enum ACMode {
    AC_MODE_OFF = 0,
    AC_MODE_COOL = 1,
    AC_MODE_HEAT = 2,
    AC_MODE_CIRCULATE = 3,
    AC_MODE_DRY = 4
};

#endif // CONFIG_H