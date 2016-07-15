#ifndef SeCo_h
#define SeCo_h

#include <Arduino.h>

class SeCo {
    public:
        float receive();
        void transmit(float dataToSend);
    private:
        bool waitHeader();
        uint8_t* inMsg();
        bool checkMsg(uint8_t *buff);
};

#endif
