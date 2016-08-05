#include "Arduino.h"
#include "SeCo.h"

static const uint8_t header = 0xAA;
static const uint8_t footer = 0xBB;
static const unsigned int floatSize = 4;
uint8_t check = 0x00;

typedef union {
    float floating;
    uint8_t binary[floatSize];
} binaryFloat;

bool SeCo::waitHeader() {
    uint8_t inByte;
    while(Serial.available()>0) {
        inByte = Serial.read();
        delayMicroseconds(100);
        if(inByte==header) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool SeCo::checkMessage(uint8_t *message, unsigned int messageSize) {
    uint8_t xorCheck = 0x00;
    unsigned int i;
    for(i=0;i<messageSize;i++) {
        xorCheck = xorCheck^message[i];
    }
    if(xorCheck==check) {
        return true;
    }
    else {
        return false;
    }
}

void SeCo::getMessage(uint8_t* message, unsigned int messageSize) {
    uint8_t inByte;
    unsigned int i=0;
    while(!waitHeader()) {}
    while(Serial.available()>0) {
        inByte= Serial.read();
        delayMicroseconds(100);
        if((inByte!=footer)&&(i<messageSize)) {
            message[i] = inByte;
            i++;
        }
        else if((inByte!=footer)&&(i==messageSize)) {
            check = inByte;
        }
        else if(inByte==footer) {
            return;
        }
    }
}

void SeCo::receiveData(float* dataIn) {
    unsigned int messageSize = floatSize;
    uint8_t message[messageSize];
    binaryFloat data;
    unsigned int i;
    getMessage(message, messageSize);
    if(checkMessage(message, messageSize)) {
        for(i=0;i<4;i++) {
            data.binary[i] = message[i];
        }
        *dataIn = data.floating;
        return;
    }
}

void SeCo::receiveArray(float* dataArrayIn, unsigned int dataPoints) {
    unsigned int messageSize = floatSize*dataPoints;
    uint8_t message[messageSize];
    binaryFloat data;
    unsigned int i=0, j, k;
    getMessage(message, messageSize);
    if(checkMessage(message, messageSize)) {
        for(j=0;j<dataPoints;j++) {
            for(k=0;k<floatSize;k++) {
                data.binary[k] = message[i];
                i++;
            }
            dataArrayIn[j] = data.floating;
        }
        return;
    }
}

void SeCo::transmitData(float* dataOut) {
    binaryFloat data;
    data.floating = *dataOut;
    uint8_t message[7];
    uint8_t xorCheck = 0x00;
    unsigned int i;
    message[0] = header;
    for(i=1;i<5;i++) {
        message[i] = data.binary[i-1];
        xorCheck = xorCheck^data.binary[i-1];
    }
    message[5] = xorCheck;
    message[6] = footer;
    Serial.write(message,7);
}

void SeCo::transmitArray(float* dataArrayOut, unsigned int dataPoints) {
    binaryFloat data;
    unsigned int messageSize = floatSize*dataPoints;
    uint8_t message[messageSize+3];
    uint8_t xorCheck = 0x00;
    message[0] = header;
    unsigned int i=1, j, k;
    for(j=0;j<dataPoints;j++) {
        data.floating = dataArrayOut[j];
        for(k=0;k<floatSize;k++) {
            message[i] = data.binary[k];
            xorCheck = xorCheck^data.binary[k];
            i++;
        }
    }
    message[messageSize+1] = xorCheck;
    message[messageSize+2] = footer;
    Serial.write(message,messageSize+3);
}
