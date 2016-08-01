#include <SeCo.h>

long baudRate = 1000000;
int led = 9;
SeCo dataLink;

void setup() {
    Serial.begin(baudRate);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

void loop() {
    float data = dataLink.receiveData();
    float* dataArray = dataLink.receiveArray();
    // analogWrite(led,map(data,0,100,0,255));
    delay(100);
    dataLink.transmitData(data);
    dataLink.transmitArray(*dataArray);
}
