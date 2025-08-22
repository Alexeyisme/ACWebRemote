/*
 * IRTadiran.cpp
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

#include "IRTadiran.h"

// IR Protocol Constants
#define IR_BUFFER_SIZE 264
#define IR_FREQUENCY 38

IRTadiran::IRTadiran(IRsend* remote) : _remote(remote) {
    // Initialize default values
    code[0] = 0x1;
    code[1] = 0x14; // fan
    code[2] = 0x30; // temp
    code[3] = 0;
    code[4] = 0;
    code[5] = 0x30; // on
    code[6] = 0;
    code[7] = 0xb;  // checksum
}

IRTadiran::~IRTadiran() {
}

bool IRTadiran::send(bool power, int mode, int fan, int temperature, bool swing) {
    if (power) {
        code[2] = 2 * temperature;
        code[1] = ((1 + fan) << 4) | (mode & 0xf);
        code[6] = (swing ? (code[6] | 0xc0) : (~(0xc0) & code[6]));
        updateChecksum();
    } else {
        code[0] = 0x1;
        code[1] = 0x14;
        code[2] = 0x30;
        code[6] = code[4] = code[3] = 0x0;
        code[5] = 0xc0;
        code[7] = 0x15;
    }
    
    uint16_t* buff = createBuffer();
    _remote->sendRaw(buff, IR_BUFFER_SIZE, IR_FREQUENCY);
    delete (buff);
    return true;
}

uint16_t* IRTadiran::createBuffer() {
    uint16_t* buff = new uint16_t[IR_BUFFER_SIZE];
    int i = 0, repeat = 2;
    
    for (int l = 0; l < repeat; l++) {
        buff[i++] = 8000;   // Header mark
        buff[i++] = 4000;   // Header space
        
        for (int j = 0; j < 8; j++) {
            for (int mask = 1; mask < 256; mask <<= 1) {
                buff[i++] = mask & code[j] ? 1618 : 545;  // Data mark
                buff[i++] = mask & code[j] ? 545 : 1618;  // Data space
            }
        }
        
        if (l < repeat - 1) {
            buff[i++] = 1618;   // Gap mark
            buff[i++] = 31000;  // Gap space
        }
    }
    
    buff[i++] = 1618;  // Final mark
    buff[i++] = 1618;  // Final space
    
    return buff;
}

void IRTadiran::updateChecksum() {
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        sum += code[i];
    }
    
    int temp = code[2] / 2;
    int fan = (code[1] & 0xf0) >> 4;
    bool swing = (code[6] & 0xc0) != 0;
    
    code[7] = sum - (0xf * (3 + temp / 8) + (fan) * 0xf + (swing ? 0xb4 : 0));
}

void IRTadiran::setTemp(uint8_t temp) {
    code[2] = 2 * temp;
    updateChecksum();
}

void IRTadiran::setFan(uint8_t fan) {
    code[1] = ((1 + fan) << 4) | (code[1] & 0xf);
    updateChecksum();
}

void IRTadiran::setMode(uint8_t mode) {
    code[1] = (code[1] & 0xf0) | (mode & 0xf);
    updateChecksum();
}

void IRTadiran::setOn(bool isOn) {
    code[5] = ((isOn ? 0x30 : 0xc0)) | (code[5] & 0xf);
    updateChecksum();
}
