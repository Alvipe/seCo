#ifndef SeCo_h
#define SeCo_h

#include <Arduino.h>

class SeCo {
    public:
        SeCo(Stream &serial);
        void readData(float* dataIn);
        void readArray(float* dataArrayIn, unsigned int dataPoints);
        void writeData(float* dataOut);
        void writeArray(float* dataArrayOut, unsigned int dataPoints);
    private:
        bool waitHeader();
        bool checkMessage(uint8_t* message, unsigned int messageSize);
        void getMessage(uint8_t* message, unsigned int messageSize);
        Stream* _serial;
        uint8_t check;
};

#endif
