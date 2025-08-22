#ifndef AC_CONTROLLER_H
#define AC_CONTROLLER_H

#include <IRsend.h>
#include <IRremoteESP8266.h>
#include "config.h"

// Protocol mapping structure
struct ACProtocol {
    decode_type_t protocol;
    bool implemented;
    const char* description;
};

class ACController {
public:
    ACController(IRsend* irsend);
    ~ACController();
    
    // Main control function
    bool sendCommand(int model, int mode, int temp, int fan, bool swing);
    
    // Model-specific control functions
    bool sendTadiran(int mode, int temp, int fan, bool swing);
    bool sendViaProtocol(decode_type_t protocol, int model, int mode, int temp, int fan, bool swing);

private:
    IRsend* _irsend;
};

#endif // AC_CONTROLLER_H
