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

// AC Models - IRremoteESP8266 supported protocols
enum ACModel {
    AC_MODEL_TADIRAN = 0,        // Tadiran (current implementation)
    AC_MODEL_CARRIER_AC64 = 1,   // Carrier AC64
    AC_MODEL_CARRIER_AC84 = 2,   // Carrier AC84
    AC_MODEL_CARRIER_AC128 = 3,  // Carrier AC128
    AC_MODEL_DAIKIN = 4,         // Daikin
    AC_MODEL_DAIKIN2 = 5,        // Daikin2
    AC_MODEL_DAIKIN216 = 6,      // Daikin216
    AC_MODEL_DAIKIN64 = 7,       // Daikin64
    AC_MODEL_DAIKIN128 = 8,      // Daikin128
    AC_MODEL_DAIKIN152 = 9,      // Daikin152
    AC_MODEL_DAIKIN160 = 10,     // Daikin160
    AC_MODEL_DAIKIN176 = 11,     // Daikin176
    AC_MODEL_DAIKIN200 = 12,     // Daikin200
    AC_MODEL_DAIKIN312 = 13,     // Daikin312
    AC_MODEL_FUJITSU_AC = 14,    // Fujitsu AC
    AC_MODEL_GREE = 15,          // Gree
    AC_MODEL_HITACHI_AC = 16,    // Hitachi AC
    AC_MODEL_HITACHI_AC1 = 17,   // Hitachi AC1
    AC_MODEL_HITACHI_AC2 = 18,   // Hitachi AC2
    AC_MODEL_HITACHI_AC3 = 19,   // Hitachi AC3
    AC_MODEL_HITACHI_AC4 = 20,   // Hitachi AC4
    AC_MODEL_HITACHI_AC424 = 21, // Hitachi AC424
    AC_MODEL_KELVINATOR = 22,    // Kelvinator
    AC_MODEL_MIDEA = 23,         // Midea
    AC_MODEL_MITSUBISHI_AC = 24, // Mitsubishi AC
    AC_MODEL_MITSUBISHI_136 = 25, // Mitsubishi 136
    AC_MODEL_MITSUBISHI_112 = 26, // Mitsubishi 112
    AC_MODEL_MITSUBISHI_HEAVY_88 = 27, // Mitsubishi Heavy 88
    AC_MODEL_MITSUBISHI_HEAVY_152 = 28, // Mitsubishi Heavy 152
    AC_MODEL_PANASONIC_AC = 29,  // Panasonic AC
    AC_MODEL_PANASONIC_AC32 = 30, // Panasonic AC32
    AC_MODEL_SAMSUNG_AC = 31,    // Samsung AC
    AC_MODEL_SHARP_AC = 32,      // Sharp AC
    AC_MODEL_TCL112AC = 33,      // TCL 112 AC
    AC_MODEL_TOSHIBA_AC = 34,    // Toshiba AC
    AC_MODEL_TROTEC = 35,        // Trotec
    AC_MODEL_VESTEL_AC = 36,     // Vestel AC
    AC_MODEL_WHIRLPOOL_AC = 37,  // Whirlpool AC
    AC_MODEL_COUNT = 38
};

// AC Model Names (for web interface)
extern const char* AC_MODEL_NAMES[AC_MODEL_COUNT];

// Default AC Model
#define DEFAULT_AC_MODEL AC_MODEL_TADIRAN

#endif // CONFIG_H