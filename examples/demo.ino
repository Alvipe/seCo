#include <SeCo.h>

unsigned int dataPoints = 50;
long baudRate = 115200;
int led = 13;
SeCo dataLink;

void setup() {
    Serial.begin(baudRate);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

void loop() {
    float dataArray[dataPoints];
    float data;
    dataLink.receiveArray(&dataArray[0], dataPoints);
    delay(100);
    dataLink.transmitArray(&dataArray[0], dataPoints);
    delay(100);
    dataLink.receiveData(&data);
    delay(100);
    dataLink.transmitData(&data);
    delay(100);
}
