#ifndef SeCo_h
#define SeCo_h

#include <Arduino.h>

class SeCo {
    public:
        void receiveData(float* dataIn);
        void receiveArray(float* dataArrayIn, unsigned int dataPoints);
        void transmitData(float* dataOut);
        void transmitArray(float* dataArrayOut, unsigned int dataPoints);
    private:
        bool waitHeader();
        bool checkMessage(uint8_t* message, unsigned int messageSize);
        void getMessage(uint8_t* message, unsigned int messageSize);
};

#endif
