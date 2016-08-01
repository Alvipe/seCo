#include "Arduino.h"
#include "SeCo.h"

static const uint8_t header = 0xAA;
static const uint8_t footer = 0xBB;
static const uint8_t floatSize = 4;
uint8_t check = 0x00;

typedef union {
    float floating;
    uint8_t binary[floatSize];
} binaryFloat;

bool SeCo::waitHeader() {
    uint8_t inByte;
    while(Serial.available()>0) {
        inByte = Serial.read();
        // delayMicroseconds(100);
        if(inByte==header) {
            return true;
        }
        else return false;
    }
    return false;
}

bool SeCo::checkMessage(uint8_t *message) {
    uint8_t xorCheck = 0x00;
    unsigned int messageSize = sizeof(message);
    unsigned int i;
    for(i=0;i<messageSize;i++) {
        xorCheck = xorCheck^message[i];
    }
    if(xorCheck==check) return true;
    else return false;
}

uint8_t* SeCo::getMessage(unsigned int dataPoints) {
    unsigned int messageSize = floatSize*dataPoints;
    static uint8_t message[messageSize];
    uint8_t inByte;
    unsigned int i=0;
    while(!waitHeader()) {}
    while(Serial.available()>0) {
        inByte= Serial.read();
        // delayMicroseconds(50);
        if((inByte!=footer)&&(i<messageSize)) {
            message[i] = inByte;
            i++;
        }
        else if((inByte!=footer)&&(i==messageSize)) {
            check = inByte;
        }
        else if(inByte==footer) {
            return message;
        }
        else return NULL;
    }
    return NULL;
}

float SeCo::receiveData() {
    uint8_t *message;
    binaryFloat data;
    unsigned int i;
    message = getMessage(1);
    if(checkMessage(message)) {
        for(i=0;i<4;i++) {
            data.binary[i] = message[i];
        }
        return data.floating;
    }
    else return NULL;
}

float* SeCo::receiveArray() {
    uint8_t* message;
    binaryFloat data;
    unsigned int dataPoints = 100;
    static float dataArray[dataPoints];
    unsigned int i=0, j, k;
    message = getMessage(dataPoints);
    if(checkMessage(message)) {
        for(j=0;j<dataPoints;j++) {
            for(k=0;k<floatSize;k++) {
                data.binary[k] = message[i];
                i++;
            }
            dataArray[j] = data.floating;
        }
        return dataArray;
    }
    else return NULL;
}

void SeCo::transmitData(float dataToSend) {
    binaryFloat data;
    data.floating = dataToSend;
    uint8_t message[7];
    uint8_t xorCheck = 0x00;
    unsigned int i, j;
    message[0] = header;
    for(i=1;i<5;i++) {
        message[i] = data.binary[i-1];
        xorCheck = xorCheck^data.binary[i-1];
    }
    message[5] = xorCheck;
    message[6] = footer;
    Serial.write(message,7);
}

void SeCo::transmitArray(float* arrayToSend) {
    binaryFloat data;
    unsigned int dataPoints = sizeof(arrayToSend);
    unsigned int messageSize = floatSize*dataPoints;
    uint8_t message[messageSize+3];
    uint8_t xorCheck = 0x00;
    message[0] = header;
    unsigned int i=1, j, k;
    for(j=0;j<dataPoints;j++) {
        data.floating = arrayToSend[j];
        for(k=0;k<floatSize;k++) {
            message[i] = data.binary[k];
            xorCheck = xorCheck^data.binary[k];
            i++;
        }
    }
    message[401] = xorCheck;
    message[402] = footer;
    Serial.write(message,403);
}
