#include "Arduino.h"
#include "SeCo.h"

#define header 0xAA
#define footer 0xBB
#define dataSize sizeof(float)

typedef union {
    float floating;
    uint8_t binary[dataSize];
} binaryFloat;

uint8_t check = 0x00;

bool SeCo::waitHeader() {
    uint8_t rec;
    while(Serial.available()>0) {
        rec = Serial.read();
        delayMicroseconds(100);
        if(rec==header) {
            return true;
        }
        else return false;
    }
}

uint8_t* SeCo::inMsg() {
    uint8_t rec;
    static uint8_t inBuff[4];
    unsigned int i = 0;
    while(Serial.available()>0) {
        rec = Serial.read();
        delayMicroseconds(100);
        if((rec!=footer)&&(i<dataSize)) {
            inBuff[i] = rec;
            i++;
            if(i==4) {
                check = Serial.read();
                delayMicroseconds(100);
            }
        }
        if(rec==footer) {
            i = 0;
            return inBuff;
        }
    }
}

bool SeCo::checkMsg(uint8_t *buff) {
    uint8_t xorChk = 0x00;
    unsigned int i;
    for(i=0;i<4;i++) {
        xorChk = xorChk^buff[i];
    }
    if(xorChk==check) return true;
    else return false;
}

float SeCo::receive() {
    uint8_t *inBuff;
    binaryFloat data;
    unsigned int i;
    while(!waitHeader()) {}
    inBuff = inMsg();
    if(checkMsg(inBuff)) {
        for(i=0;i<4;i++) {
            data.binary[i] = inBuff[i];
        }
        return data.floating;
    }
}

void SeCo::transmit(float dataToSend) {
    binaryFloat data;
    data.floating = dataToSend;
    uint8_t message[7];
    uint8_t xorChk = 0x00;
    unsigned int i, j;
    for(i=0;i<4;i++) {
        xorChk = xorChk^data.binary[i];
    }
    message[0] = header;
    for(j=1;j<5;j++) {
        message[j] = data.binary[j-1];
    }
    message[5] = xorChk;
    message[6] = footer;
    // Serial.write(header);
    // Serial.write(data.binary,4);
    // Serial.write(xorChk);
    // Serial.write(footer);
    Serial.write(message,7);
}
