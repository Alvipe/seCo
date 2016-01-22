#define header 0xAA
#define footer 0xBB
#define dataSize 4

typedef union {
    float floating;
    byte binary[dataSize];
} binaryFloat;

byte check = 0x00;

int led1 = 9, led2 = 13;

void setup() {
    Serial.begin(1000000);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
}

boolean waitHeader() {
    byte rec;
    while(Serial.available()>0) {
        rec = Serial.read();
        delayMicroseconds(100);
        if(rec==header) {
            return true;
        }
        else return false;
    }
}

byte * inMsg() {
    byte rec;
    static byte inBuff[4];
    int i = 0;
    while(Serial.available()>0) {
        rec = Serial.read();
        delayMicroseconds(100);
        if((rec!=footer)&&(i<dataSize)) {
            inBuff[i] = rec;
            i++;
            if(i==4) {
                check = Serial.read();
                delayMicroseconds(100);
            }
        }
        if(rec==footer) {
            i = 0;
            return inBuff;
        }
    }
}

boolean checkMsg(byte *buff) {
    byte xorChk = 0x00;
    int i;
    for(i=0;i<4;i++) {
        xorChk = xorChk^buff[i];
    }
    if(xorChk==check) return true;
    else return false;
}

void blinkLed(int repeat,int bTime)
{
    int i;
    for(i=0;i<repeat;i++)
    {
        digitalWrite(led2,HIGH);
        delay(bTime);
        digitalWrite(led2,LOW);
        delay(bTime);
    }
}

void secoRx() {
    byte *inBuff;
    binaryFloat data;
    int i;
    while(!waitHeader()) {}
    inBuff = inMsg();
    if(checkMsg(inBuff)) {
        for(i=0;i<4;i++) {
            data.binary[i] = inBuff[i];
        }
        Serial.write(data.binary,4);
        analogWrite(led1,map(data.floating,0,100,0,255));
    }
}

void loop() {
    secoRx();
}
