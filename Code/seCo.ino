typedef union {
    float floating;
    byte binary[4];
} binaryFloat;

byte header = 0xAA, footer = 0xBB;
byte check = 0x00;
byte rec = 0x00;
binaryFloat data;

int led1 = 9, led2 = 13;
int i = 0;

void setup() {
    Serial.begin(115200);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
}

//byte readHeader() {
//    if(Serial.available()>0) {
//        rec = Serial.read();
//    }
//    return rec;
//}

void loop() {
    while(Serial.available()>0) {
        rec = Serial.read();
        delayMicroseconds(100);
        if(rec!=header) {
            digitalWrite(led2, LOW);
        }
        else if(rec==header) {
            check = 0x00;
            digitalWrite(led2, HIGH);
            for(i=0;i<sizeof(data.binary);i++) {
                rec = Serial.read();
                delayMicroseconds(100);
                data.binary[i] = rec;
                check = check^data.binary[i];
            }
            Serial.write(data.binary,4);
            analogWrite(led1,data.floating);
        }
        rec = Serial.read();
        delayMicroseconds(100);
        if(rec == check) {
            for(i=0;i<5;i++) {
                digitalWrite(led2,HIGH);
                delay(100);
                digitalWrite(led2,LOW);
                delay(100);
            }
        }
    }
}

