#ifndef AC_CONTROLLER_H
#define AC_CONTROLLER_H

#include <IRsend.h>
#include <IRremoteESP8266.h>
#include "config.h"

// Forward declarations for IRremoteESP8266 classes
class IRCarrierAc64;
class IRCarrierAc84;
class IRCarrierAc128;
class IRDaikinESP;
class IRDaikin2;
class IRDaikin216;
class IRDaikin64;
class IRDaikin128;
class IRDaikin152;
class IRDaikin160;
class IRDaikin176;
class IRDaikin200;
class IRDaikin312;
class IRFujitsuAC;
class IRGreeAC;
class IRHitachiAc;
class IRHitachiAc1;
class IRHitachiAc2;
class IRHitachiAc3;
class IRHitachiAc4;
class IRHitachiAc424;
class IRKelvinatorAC;
class IRMideaAC;
class IRMitsubishiAC;
class IRMitsubishi136;
class IRMitsubishi112;
class IRMitsubishiHeavy88Ac;
class IRMitsubishiHeavy152Ac;
class IRPanasonicAc;
class IRPanasonicAc32;
class IRSamsungAc;
class IRSharpAc;
class IRTcl112Ac;
class IRToshibaAC;
class IRTrotecESP;
class IRVestelAc;
class IRWhirlpoolAc;

class ACController {
public:
    ACController(IRsend* irsend);
    ~ACController();
    
    // Main control function
    bool sendCommand(int model, int mode, int temp, int fan = 1, bool swing = false);
    
    // Model-specific control functions
    bool sendTadiran(int mode, int temp, int fan, bool swing);
    bool sendCarrierAC64(int mode, int temp, int fan, bool swing);
    bool sendCarrierAC84(int mode, int temp, int fan, bool swing);
    bool sendCarrierAC128(int mode, int temp, int fan, bool swing);
    bool sendDaikin(int mode, int temp, int fan, bool swing);
    bool sendDaikin2(int mode, int temp, int fan, bool swing);
    bool sendDaikin216(int mode, int temp, int fan, bool swing);
    bool sendDaikin64(int mode, int temp, int fan, bool swing);
    bool sendDaikin128(int mode, int temp, int fan, bool swing);
    bool sendDaikin152(int mode, int temp, int fan, bool swing);
    bool sendDaikin160(int mode, int temp, int fan, bool swing);
    bool sendDaikin176(int mode, int temp, int fan, bool swing);
    bool sendDaikin200(int mode, int temp, int fan, bool swing);
    bool sendDaikin312(int mode, int temp, int fan, bool swing);
    bool sendFujitsuAC(int mode, int temp, int fan, bool swing);
    bool sendGreeAC(int mode, int temp, int fan, bool swing);
    bool sendHitachiAC(int mode, int temp, int fan, bool swing);
    bool sendHitachiAC1(int mode, int temp, int fan, bool swing);
    bool sendHitachiAC2(int mode, int temp, int fan, bool swing);
    bool sendHitachiAC3(int mode, int temp, int fan, bool swing);
    bool sendHitachiAC4(int mode, int temp, int fan, bool swing);
    bool sendHitachiAC424(int mode, int temp, int fan, bool swing);
    bool sendKelvinatorAC(int mode, int temp, int fan, bool swing);
    bool sendMideaAC(int mode, int temp, int fan, bool swing);
    bool sendMitsubishiAC(int mode, int temp, int fan, bool swing);
    bool sendMitsubishi136(int mode, int temp, int fan, bool swing);
    bool sendMitsubishi112(int mode, int temp, int fan, bool swing);
    bool sendMitsubishiHeavy88AC(int mode, int temp, int fan, bool swing);
    bool sendMitsubishiHeavy152AC(int mode, int temp, int fan, bool swing);
    bool sendPanasonicAC(int mode, int temp, int fan, bool swing);
    bool sendPanasonicAC32(int mode, int temp, int fan, bool swing);
    bool sendSamsungAC(int mode, int temp, int fan, bool swing);
    bool sendSharpAC(int mode, int temp, int fan, bool swing);
    bool sendTcl112AC(int mode, int temp, int fan, bool swing);
    bool sendToshibaAC(int mode, int temp, int fan, bool swing);
    bool sendTrotec(int mode, int temp, int fan, bool swing);
    bool sendVestelAC(int mode, int temp, int fan, bool swing);
    bool sendWhirlpoolAC(int mode, int temp, int fan, bool swing);

private:
    IRsend* _irsend;
};

#endif // AC_CONTROLLER_H
