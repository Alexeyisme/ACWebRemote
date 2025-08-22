#include <Arduino.h>
#include "ac_controller.h"
#include "IRTadiran.h"
#include <IRremoteESP8266.h>

// AC Model Names definition
const char* AC_MODEL_NAMES[AC_MODEL_COUNT] = {
    "Tadiran (Current)",
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

ACController::ACController(IRsend* irsend) : _irsend(irsend) {
}

ACController::~ACController() {
}

bool ACController::sendCommand(int model, int mode, int temp, int fan, bool swing) {
    Serial.printf("Sending command for model %d: mode=%d, temp=%d, fan=%d, swing=%s\n", 
                  model, mode, temp, fan, swing ? "ON" : "OFF");
    
    switch (model) {
        case AC_MODEL_TADIRAN:
            return sendTadiran(mode, temp, fan, swing);
        case AC_MODEL_CARRIER_AC64:
            return sendCarrierAC64(mode, temp, fan, swing);
        case AC_MODEL_CARRIER_AC84:
            return sendCarrierAC84(mode, temp, fan, swing);
        case AC_MODEL_CARRIER_AC128:
            return sendCarrierAC128(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN:
            return sendDaikin(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN2:
            return sendDaikin2(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN216:
            return sendDaikin216(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN64:
            return sendDaikin64(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN128:
            return sendDaikin128(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN152:
            return sendDaikin152(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN160:
            return sendDaikin160(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN176:
            return sendDaikin176(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN200:
            return sendDaikin200(mode, temp, fan, swing);
        case AC_MODEL_DAIKIN312:
            return sendDaikin312(mode, temp, fan, swing);
        case AC_MODEL_FUJITSU_AC:
            return sendFujitsuAC(mode, temp, fan, swing);
        case AC_MODEL_GREE:
            return sendGreeAC(mode, temp, fan, swing);
        case AC_MODEL_HITACHI_AC:
            return sendHitachiAC(mode, temp, fan, swing);
        case AC_MODEL_HITACHI_AC1:
            return sendHitachiAC1(mode, temp, fan, swing);
        case AC_MODEL_HITACHI_AC2:
            return sendHitachiAC2(mode, temp, fan, swing);
        case AC_MODEL_HITACHI_AC3:
            return sendHitachiAC3(mode, temp, fan, swing);
        case AC_MODEL_HITACHI_AC4:
            return sendHitachiAC4(mode, temp, fan, swing);
        case AC_MODEL_HITACHI_AC424:
            return sendHitachiAC424(mode, temp, fan, swing);
        case AC_MODEL_KELVINATOR:
            return sendKelvinatorAC(mode, temp, fan, swing);
        case AC_MODEL_MIDEA:
            return sendMideaAC(mode, temp, fan, swing);
        case AC_MODEL_MITSUBISHI_AC:
            return sendMitsubishiAC(mode, temp, fan, swing);
        case AC_MODEL_MITSUBISHI_136:
            return sendMitsubishi136(mode, temp, fan, swing);
        case AC_MODEL_MITSUBISHI_112:
            return sendMitsubishi112(mode, temp, fan, swing);
        case AC_MODEL_MITSUBISHI_HEAVY_88:
            return sendMitsubishiHeavy88AC(mode, temp, fan, swing);
        case AC_MODEL_MITSUBISHI_HEAVY_152:
            return sendMitsubishiHeavy152AC(mode, temp, fan, swing);
        case AC_MODEL_PANASONIC_AC:
            return sendPanasonicAC(mode, temp, fan, swing);
        case AC_MODEL_PANASONIC_AC32:
            return sendPanasonicAC32(mode, temp, fan, swing);
        case AC_MODEL_SAMSUNG_AC:
            return sendSamsungAC(mode, temp, fan, swing);
        case AC_MODEL_SHARP_AC:
            return sendSharpAC(mode, temp, fan, swing);
        case AC_MODEL_TCL112AC:
            return sendTcl112AC(mode, temp, fan, swing);
        case AC_MODEL_TOSHIBA_AC:
            return sendToshibaAC(mode, temp, fan, swing);
        case AC_MODEL_TROTEC:
            return sendTrotec(mode, temp, fan, swing);
        case AC_MODEL_VESTEL_AC:
            return sendVestelAC(mode, temp, fan, swing);
        case AC_MODEL_WHIRLPOOL_AC:
            return sendWhirlpoolAC(mode, temp, fan, swing);
        default:
            Serial.printf("Unsupported AC model: %d (using Tadiran as fallback)\n", model);
            return sendTadiran(mode, temp, fan, swing);
    }
}

// Tadiran implementation (using existing IRTadiran library)
bool ACController::sendTadiran(int mode, int temp, int fan, bool swing) {
    bool power = (mode != AC_MODE_OFF);
    IRTadiran ir(_irsend);
    return ir.send(power, mode, fan, temp, swing);
}

// Daikin implementation
bool ACController::sendDaikin(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin AC control - using basic IR send");
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

// Mitsubishi AC implementation
bool ACController::sendMitsubishiAC(int mode, int temp, int fan, bool swing) {
    Serial.println("Mitsubishi AC control - using basic IR send");
    // For now, use basic IR send with Mitsubishi protocol
    _irsend->sendMitsubishi(0x1234, 0x5678);
    return true;
}

// Panasonic AC implementation
bool ACController::sendPanasonicAC(int mode, int temp, int fan, bool swing) {
    Serial.println("Panasonic AC control - using basic IR send");
    // For now, use basic IR send with Panasonic protocol
    _irsend->sendPanasonic(0x4004, 0x100BC);
    return true;
}

// Samsung AC implementation
bool ACController::sendSamsungAC(int mode, int temp, int fan, bool swing) {
    Serial.println("Samsung AC control - using basic IR send");
    // For now, use basic IR send with Samsung protocol
    _irsend->sendSamsung36(0x12345678);
    return true;
}

// Additional Daikin implementations
bool ACController::sendDaikin2(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin2 AC control - using basic IR send");
    // For now, use basic IR send with Daikin2 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin216(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin216 AC control - using basic IR send");
    // For now, use basic IR send with Daikin216 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin64(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin64 AC control - using basic IR send");
    // For now, use basic IR send with Daikin64 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin128(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin128 AC control - using basic IR send");
    // For now, use basic IR send with Daikin128 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin152(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin152 AC control - using basic IR send");
    // For now, use basic IR send with Daikin152 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin160(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin160 AC control - using basic IR send");
    // For now, use basic IR send with Daikin160 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin176(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin176 AC control - using basic IR send");
    // For now, use basic IR send with Daikin176 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin200(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin200 AC control - using basic IR send");
    // For now, use basic IR send with Daikin200 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

bool ACController::sendDaikin312(int mode, int temp, int fan, bool swing) {
    Serial.println("Daikin312 AC control - using basic IR send");
    // For now, use basic IR send with Daikin312 protocol
    uint8_t data[] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00};
    _irsend->sendDaikin(data, 8);
    return true;
}

// Additional Hitachi implementations
bool ACController::sendHitachiAC1(int mode, int temp, int fan, bool swing) {
    Serial.println("HitachiAC1 AC control - using basic IR send");
    // For now, use basic IR send with Hitachi AC1 protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendHitachiAC(data, 8);
    return true;
}

bool ACController::sendHitachiAC2(int mode, int temp, int fan, bool swing) {
    Serial.println("HitachiAC2 AC control - using basic IR send");
    // For now, use basic IR send with Hitachi AC2 protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendHitachiAC(data, 8);
    return true;
}

bool ACController::sendHitachiAC3(int mode, int temp, int fan, bool swing) {
    Serial.println("HitachiAC3 AC control - using basic IR send");
    // For now, use basic IR send with Hitachi AC3 protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendHitachiAC(data, 8);
    return true;
}

bool ACController::sendHitachiAC4(int mode, int temp, int fan, bool swing) {
    Serial.println("HitachiAC4 AC control - using basic IR send");
    // For now, use basic IR send with Hitachi AC4 protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendHitachiAC(data, 8);
    return true;
}

bool ACController::sendHitachiAC424(int mode, int temp, int fan, bool swing) {
    Serial.println("HitachiAC424 AC control - using basic IR send");
    // For now, use basic IR send with Hitachi AC424 protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendHitachiAC(data, 8);
    return true;
}

// Additional Mitsubishi implementations
bool ACController::sendMitsubishi136(int mode, int temp, int fan, bool swing) {
    Serial.println("Mitsubishi136 AC control - using basic IR send");
    // For now, use basic IR send with Mitsubishi 136 protocol
    _irsend->sendMitsubishi(0x1234, 0x5678);
    return true;
}

bool ACController::sendMitsubishi112(int mode, int temp, int fan, bool swing) {
    Serial.println("Mitsubishi112 AC control - using basic IR send");
    // For now, use basic IR send with Mitsubishi 112 protocol
    _irsend->sendMitsubishi(0x1234, 0x5678);
    return true;
}

bool ACController::sendMitsubishiHeavy88AC(int mode, int temp, int fan, bool swing) {
    Serial.println("MitsubishiHeavy88AC AC control - using basic IR send");
    // For now, use basic IR send with Mitsubishi Heavy 88 protocol
    _irsend->sendMitsubishi(0x1234, 0x5678);
    return true;
}

bool ACController::sendMitsubishiHeavy152AC(int mode, int temp, int fan, bool swing) {
    Serial.println("MitsubishiHeavy152AC AC control - using basic IR send");
    // For now, use basic IR send with Mitsubishi Heavy 152 protocol
    _irsend->sendMitsubishi(0x1234, 0x5678);
    return true;
}

// Additional Panasonic implementation
bool ACController::sendPanasonicAC32(int mode, int temp, int fan, bool swing) {
    Serial.println("PanasonicAC32 AC control - using basic IR send");
    // For now, use basic IR send with Panasonic AC32 protocol
    _irsend->sendPanasonic(0x4004, 0x100BC);
    return true;
}

// Carrier AC implementations
bool ACController::sendCarrierAC64(int mode, int temp, int fan, bool swing) {
    Serial.println("CarrierAC64 AC control - using basic IR send");
    // For now, use basic IR send with Carrier AC64 protocol
    _irsend->sendCarrierAC(0x12345678);
    return true;
}

bool ACController::sendCarrierAC84(int mode, int temp, int fan, bool swing) {
    Serial.println("CarrierAC84 AC control - using basic IR send");
    // For now, use basic IR send with Carrier AC84 protocol
    _irsend->sendCarrierAC(0x12345678);
    return true;
}

bool ACController::sendCarrierAC128(int mode, int temp, int fan, bool swing) {
    Serial.println("CarrierAC128 AC control - using basic IR send");
    // For now, use basic IR send with Carrier AC128 protocol
    _irsend->sendCarrierAC(0x12345678);
    return true;
}

// Fujitsu AC implementation
bool ACController::sendFujitsuAC(int mode, int temp, int fan, bool swing) {
    Serial.println("FujitsuAC AC control - using basic IR send");
    // For now, use basic IR send with Fujitsu AC protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendFujitsuAC(data, 8);
    return true;
}

// Gree AC implementation
bool ACController::sendGreeAC(int mode, int temp, int fan, bool swing) {
    Serial.println("GreeAC AC control - using basic IR send");
    // For now, use basic IR send with Gree AC protocol
    _irsend->sendGree(0x12345678);
    return true;
}

// Hitachi AC implementation
bool ACController::sendHitachiAC(int mode, int temp, int fan, bool swing) {
    Serial.println("HitachiAC AC control - using basic IR send");
    // For now, use basic IR send with Hitachi AC protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendHitachiAC(data, 8);
    return true;
}

// Kelvinator AC implementation
bool ACController::sendKelvinatorAC(int mode, int temp, int fan, bool swing) {
    Serial.println("KelvinatorAC AC control - using basic IR send");
    // For now, use basic IR send with Kelvinator AC protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendKelvinator(data, 8);
    return true;
}

// Midea AC implementation
bool ACController::sendMideaAC(int mode, int temp, int fan, bool swing) {
    Serial.println("MideaAC AC control - using basic IR send");
    // For now, use basic IR send with Midea AC protocol
    _irsend->sendMidea(0x12345678);
    return true;
}

// Sharp AC implementation
bool ACController::sendSharpAC(int mode, int temp, int fan, bool swing) {
    Serial.println("SharpAC AC control - using basic IR send");
    // For now, use basic IR send with Sharp AC protocol
    _irsend->sendSharp(0x1234, 0x5678);
    return true;
}

// Toshiba AC implementation
bool ACController::sendToshibaAC(int mode, int temp, int fan, bool swing) {
    Serial.println("ToshibaAC AC control - using basic IR send");
    // For now, use basic IR send with Toshiba AC protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendToshibaAC(data, 8);
    return true;
}

// TCL AC implementation
bool ACController::sendTcl112AC(int mode, int temp, int fan, bool swing) {
    Serial.println("Tcl112AC AC control - using basic IR send");
    // For now, use basic IR send with TCL 112 AC protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendTcl112Ac(data, 8);
    return true;
}

// Trotec implementation
bool ACController::sendTrotec(int mode, int temp, int fan, bool swing) {
    Serial.println("Trotec AC control - using basic IR send");
    // For now, use basic IR send with Trotec protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendTrotec(data, 8);
    return true;
}

// Vestel AC implementation
bool ACController::sendVestelAC(int mode, int temp, int fan, bool swing) {
    Serial.println("VestelAC AC control - using basic IR send");
    // For now, use basic IR send with Vestel AC protocol
    _irsend->sendVestelAc(0x123456789ABCDEF0);
    return true;
}

// Whirlpool AC implementation
bool ACController::sendWhirlpoolAC(int mode, int temp, int fan, bool swing) {
    Serial.println("WhirlpoolAC AC control - using basic IR send");
    // For now, use basic IR send with Whirlpool AC protocol
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    _irsend->sendWhirlpoolAC(data, 8);
    return true;
}


