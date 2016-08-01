#ifndef SeCo_h
#define SeCo_h

#include <Arduino.h>

class SeCo {
    public:
        float receiveData();
        float* receiveArray();
        void transmitData(float dataToSend);
        void transmitArray(float* arrayToSend);
    private:
        bool waitHeader();
        uint8_t* getMessage(unsigned int dataPoints);
        bool checkMessage(uint8_t *message);
};

#endif
