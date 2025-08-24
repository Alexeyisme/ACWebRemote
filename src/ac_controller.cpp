#include <Arduino.h>
#include "ac_controller.h"
#include "IRTadiran.h"
#include <IRremoteESP8266.h>
#include <IRac.h>

// AC Model Names definition
const char* AC_MODEL_NAMES[AC_MODEL_COUNT] = {
    "Tadiran",
    "Carrier AC64",
    "Carrier AC84", 
    "Carrier AC128",
    "Daikin",
    "Daikin2",
    "Daikin216",
    "Daikin64",
    "Daikin128",
    "Daikin152",
    "Daikin160",
    "Daikin176",
    "Daikin200",
    "Daikin312",
    "Fujitsu AC",
    "Gree",
    "Hitachi AC",
    "Hitachi AC1",
    "Hitachi AC2",
    "Hitachi AC3",
    "Hitachi AC4",
    "Hitachi AC424",
    "Kelvinator",
    "Midea",
    "Mitsubishi AC",
    "Mitsubishi 136",
    "Mitsubishi 112",
    "Mitsubishi Heavy 88",
    "Mitsubishi Heavy 152",
    "Panasonic AC",
    "Panasonic AC32",
    "Samsung AC",
    "Sharp AC",
    "TCL 112 AC",
    "Toshiba AC",
    "Trotec",
    "Vestel AC",
    "Whirlpool AC"
};

// Protocol mapping table - maps AC_MODEL_* to IRremoteESP8266 protocols
const ACProtocol AC_PROTOCOLS[AC_MODEL_COUNT] = {
    {decode_type_t::UNKNOWN, true, "Tadiran (Custom Library)"},           // AC_MODEL_TADIRAN
    {decode_type_t::CARRIER_AC64, true, "Carrier AC64"},                 // AC_MODEL_CARRIER_AC64
    {decode_type_t::CARRIER_AC84, true, "Carrier AC84"},                 // AC_MODEL_CARRIER_AC84
    {decode_type_t::CARRIER_AC128, true, "Carrier AC128"},               // AC_MODEL_CARRIER_AC128
    {decode_type_t::DAIKIN, true, "Daikin"},                            // AC_MODEL_DAIKIN
    {decode_type_t::DAIKIN2, true, "Daikin2"},                          // AC_MODEL_DAIKIN2
    {decode_type_t::DAIKIN216, true, "Daikin216"},                      // AC_MODEL_DAIKIN216
    {decode_type_t::DAIKIN64, true, "Daikin64"},                        // AC_MODEL_DAIKIN64
    {decode_type_t::DAIKIN128, true, "Daikin128"},                      // AC_MODEL_DAIKIN128
    {decode_type_t::DAIKIN152, true, "Daikin152"},                      // AC_MODEL_DAIKIN152
    {decode_type_t::DAIKIN160, true, "Daikin160"},                      // AC_MODEL_DAIKIN160
    {decode_type_t::DAIKIN176, true, "Daikin176"},                      // AC_MODEL_DAIKIN176
    {decode_type_t::DAIKIN200, true, "Daikin200"},                      // AC_MODEL_DAIKIN200
    {decode_type_t::DAIKIN312, true, "Daikin312"},                      // AC_MODEL_DAIKIN312
    {decode_type_t::FUJITSU_AC, true, "Fujitsu AC"},                    // AC_MODEL_FUJITSU_AC
    {decode_type_t::GREE, true, "Gree"},                                // AC_MODEL_GREE
    {decode_type_t::HITACHI_AC, true, "Hitachi AC"},                    // AC_MODEL_HITACHI_AC
    {decode_type_t::HITACHI_AC1, true, "Hitachi AC1"},                  // AC_MODEL_HITACHI_AC1
    {decode_type_t::HITACHI_AC2, true, "Hitachi AC2"},                  // AC_MODEL_HITACHI_AC2
    {decode_type_t::HITACHI_AC3, true, "Hitachi AC3"},                  // AC_MODEL_HITACHI_AC3
    {decode_type_t::HITACHI_AC, true, "Hitachi AC4"},                   // AC_MODEL_HITACHI_AC4 (using HITACHI_AC)
    {decode_type_t::HITACHI_AC424, true, "Hitachi AC424"},              // AC_MODEL_HITACHI_AC424
    {decode_type_t::KELVINATOR, true, "Kelvinator"},                    // AC_MODEL_KELVINATOR
    {decode_type_t::MIDEA, true, "Midea"},                              // AC_MODEL_MIDEA
    {decode_type_t::MITSUBISHI_AC, true, "Mitsubishi AC"},              // AC_MODEL_MITSUBISHI_AC
    {decode_type_t::MITSUBISHI_AC, true, "Mitsubishi 136"},             // AC_MODEL_MITSUBISHI_136 (using MITSUBISHI_AC)
    {decode_type_t::MITSUBISHI_AC, true, "Mitsubishi 112"},             // AC_MODEL_MITSUBISHI_112 (using MITSUBISHI_AC)
    {decode_type_t::MITSUBISHI_HEAVY_88, true, "Mitsubishi Heavy 88"},  // AC_MODEL_MITSUBISHI_HEAVY_88
    {decode_type_t::MITSUBISHI_HEAVY_152, true, "Mitsubishi Heavy 152"}, // AC_MODEL_MITSUBISHI_HEAVY_152
    {decode_type_t::PANASONIC_AC, true, "Panasonic AC"},                // AC_MODEL_PANASONIC_AC
    {decode_type_t::PANASONIC_AC32, true, "Panasonic AC32"},            // AC_MODEL_PANASONIC_AC32
    {decode_type_t::SAMSUNG_AC, true, "Samsung AC"},                    // AC_MODEL_SAMSUNG_AC
    {decode_type_t::SHARP_AC, true, "Sharp AC"},                        // AC_MODEL_SHARP_AC
    {decode_type_t::TCL112AC, true, "TCL 112 AC"},                      // AC_MODEL_TCL112AC
    {decode_type_t::TOSHIBA_AC, true, "Toshiba AC"},                    // AC_MODEL_TOSHIBA_AC
    {decode_type_t::TROTEC, true, "Trotec"},                            // AC_MODEL_TROTEC
    {decode_type_t::VESTEL_AC, true, "Vestel AC"},                      // AC_MODEL_VESTEL_AC
    {decode_type_t::WHIRLPOOL_AC, true, "Whirlpool AC"}                 // AC_MODEL_WHIRLPOOL_AC
};

ACController::ACController(IRsend* irsend) : _irsend(irsend) {
}

ACController::~ACController() {
}

bool ACController::sendCommand(int model, int mode, int temp, int fan, bool swing) {
    Serial.printf("Sending command for model %d: mode=%d, temp=%d, fan=%d, swing=%s\n", 
                  model, mode, temp, fan, swing ? "ON" : "OFF");
    
    // Validate model index
    if (model < 0 || model >= AC_MODEL_COUNT) {
        Serial.printf("Invalid AC model: %d (using Tadiran as fallback)\n", model);
        return sendTadiran(mode, temp, fan, swing);
    }
    
    // Validate temperature range
    if (temp < AC_TEMP_MIN || temp > AC_TEMP_MAX) {
        Serial.printf("Invalid temperature: %d°C (using 24°C as default)\n", temp);
        temp = 24;
    }
    
    // Validate mode (0=OFF, 1=COOL, 2=HEAT, 3=FAN, 4=DRY, 5=AUTO)
    if (mode < AC_MODE_MIN || mode > AC_MODE_MAX) {
        Serial.printf("Invalid mode: %d (using COOL as default)\n", mode);
        mode = AC_MODE_COOL;
    }
    
    // Validate fan speed
    if (fan < AC_FAN_MIN || fan > AC_FAN_MAX) {
        Serial.printf("Invalid fan speed: %d (using 3 as default)\n", fan);
        fan = 3;
    }
    
    // Special case for Tadiran (uses custom library)
    if (model == AC_MODEL_TADIRAN) {
        return sendTadiran(mode, temp, fan, swing);
    }
    
    // Get protocol info
    const ACProtocol& protocol = AC_PROTOCOLS[model];
    
    // Check if protocol is implemented
    if (!protocol.implemented) {
        Serial.printf("AC model %d (%s) not yet implemented - using Tadiran as fallback\n", 
                     model, protocol.description);
        return sendTadiran(mode, temp, fan, swing);
    }
    
    // Send using unified protocol handler
    return sendViaProtocol(protocol.protocol, model, mode, temp, fan, swing);
}

// Tadiran implementation (using existing IRTadiran library)
bool ACController::sendTadiran(int mode, int temp, int fan, bool swing) {
    bool power = (mode != AC_MODE_OFF);
    IRTadiran ir(_irsend);
    return ir.send(power, mode, fan, temp, swing);
}

// Unified protocol handler for all IRremoteESP8266 protocols
bool ACController::sendViaProtocol(decode_type_t protocol, int model, int mode, int temp, int fan, bool swing) {
    Serial.printf("Sending via protocol %d for model %d\n", protocol, model);
    
    // Helper functions for parameter mapping
    auto mapOpMode = [](int mode) -> stdAc::opmode_t {
        switch (mode) {
            case AC_MODE_COOL: return stdAc::opmode_t::kCool;
            case AC_MODE_HEAT: return stdAc::opmode_t::kHeat;
            case AC_MODE_CIRCULATE: return stdAc::opmode_t::kFan;
            case AC_MODE_DRY: return stdAc::opmode_t::kDry;
            default: return stdAc::opmode_t::kOff;
        }
    };
    
    auto mapFan = [](int fan) -> stdAc::fanspeed_t {
        switch (fan) {
            case 1: return stdAc::fanspeed_t::kLow;
            case 2: return stdAc::fanspeed_t::kMedium;
            case 3: return stdAc::fanspeed_t::kHigh;
            case 4: return stdAc::fanspeed_t::kMax;
            default: return stdAc::fanspeed_t::kAuto;
        }
    };
    
    // Create IRac instance
    IRac ac(IR_LED_PIN);
    
    // Set common state parameters
    ac.next.protocol = protocol;
    ac.next.power = (mode != AC_MODE_OFF);
    ac.next.mode = mapOpMode(mode);
    ac.next.degrees = temp;
    ac.next.fanspeed = mapFan(fan);
    ac.next.swingv = swing ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
    ac.next.swingh = stdAc::swingh_t::kOff;
    
    // Send the command
    ac.sendAc();
    
    Serial.printf("Sent AC command: power=%s, mode=%d, temp=%d, fan=%d, swing=%s\n",
                  ac.next.power ? "ON" : "OFF", mode, temp, fan, swing ? "ON" : "OFF");
    
    return true;
}


