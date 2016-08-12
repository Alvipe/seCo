#include <SeCo.h>

unsigned int dataPoints = 50;
long baudRate = 115200;
SeCo dataLink(Serial);

void setup() {
    Serial.begin(baudRate);
}

void loop() {
    float dataArray[dataPoints];
    float data;
    dataLink.readArray(&dataArray[0], dataPoints);
    delay(100);
    dataLink.writeArray(&dataArray[0], dataPoints);
    delay(100);
    dataLink.readData(&data);
    delay(100);
    dataLink.writeData(&data);
    delay(100);
}
