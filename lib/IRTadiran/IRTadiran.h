/*
 * IRTadiran.h
 *
 *  Created on: May 24, 2018
 *      Author: arik
 * 
 *  Original source: https://github.com/arikfe/IRTadiran
 *  License: MIT License
 *  Author: arikfe (https://github.com/arikfe)
 * 
 *  This file is based on the original IRTadiran library and has been
 *  adapted for use in the AC Web Remote project.
 */

#ifndef IRTADIRAN_H
#define IRTADIRAN_H

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <stdint.h>

class IRTadiran {
public:
    IRTadiran(IRsend* remote);
    virtual ~IRTadiran();
    
    // Main control function
    bool send(bool power, int mode, int fan, int temperature, bool swing);
    
    // Individual control functions
    void setTemp(uint8_t temp);
    void setFan(uint8_t fan);
    void setMode(uint8_t mode);
    void setOn(bool isOn);

private:
    uint8_t code[8];
    IRsend* _remote;
    
    uint16_t* createBuffer();
    void updateChecksum();
};

#endif // IRTADIRAN_H
