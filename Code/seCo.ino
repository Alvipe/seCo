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
    Serial.begin(115200);
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
    int j;
    for(j=0;j<4;j++) {
        xorChk = xorChk^buff[j];
    }
    if(xorChk==check) return true;
    else return false;
}

void blinkLed(int repeat,int bTime)
{
    int j;
    for(j=0;j<repeat;j++)
    {
        digitalWrite(led2,HIGH);
        delay(bTime);
        digitalWrite(led2,LOW);
        delay(bTime);
    }
}

void loop() {
    byte *inBuff;
    binaryFloat data;
    int j;
    while(!waitHeader()) {}
    inBuff = inMsg();
    if(checkMsg(inBuff)) {
        for(j=0;j<4;j++) {
            data.binary[j] = inBuff[j];
        }
        Serial.write(data.binary,4);
        analogWrite(led1,map(data.floating,0,100,0,255));
    }
}

