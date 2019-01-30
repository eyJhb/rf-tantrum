#include <Arduino.h>
#define tx_pin 12

/* int freq = (int)1.0/(1000.0)*1000000.0; */
int freq = (int)10000;
int preamble[8] = {1,0,1,0,1,0,1,0};
int sfd[8] = {1,0,1,0,1,0,1,1};
int test[8] = {0,1,0,1,0,1,0,1};
/* int test[8] = {1,1,1,1,1,1,1,1}; */
/* int test[8] = {0,0,0,0,0,0,0,0}; */
int curBit = 2;

void printArray(int data[], int arraySize) {
    Serial.print("[");
    bool first = true;
    for(int i = 0; i < arraySize; i++) {
        if(first == true) {
            first = false;
        } else {
            Serial.print(",");
        }
        Serial.print(data[i]);
    }
    Serial.print("]");
}

void encodeManchester(int data[], int datasize, int result[]) {
    int current_index = 0;
    for(int i = 0; i < datasize; i++) {
        if(data[i] == 1) {
            result[current_index] = 0;
            result[current_index+1] = 1;
        } else {
            result[current_index] = 1;
            result[current_index+1] = 0;
        }
        current_index += 2;
    }
}

/* ----Intializing---- */
/* Freq: 10000 */
/* Preamble: [1,0,1,0,1,0,1,0] */
/* sfd: [1,0,1,0,1,0,1,1] */
/* test: [0,1,0,1,1,0,1,0] */
/* enc: [1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0] */
/* !!!!Intialized!!!! */

void setup() {
    pinMode(tx_pin, OUTPUT);
    Serial.begin(9600);
    
    int encMan[8*2];
    encodeManchester(test, 8, encMan);

    Serial.println("----Intializing----");
    Serial.print("Freq: ");
    Serial.println(freq);
    Serial.print("Preamble: ");
    printArray(preamble, 8);
    Serial.println();
    Serial.print("sfd: ");
    printArray(sfd, 8);
    Serial.println();
    Serial.print("test: ");
    printArray(test, 8);
    Serial.println();
    Serial.print("enc: ");
    printArray(encMan, 16);
    Serial.println();
    Serial.println("!!!!Intialized!!!!");
}

void sendBit(int bit) {
    digitalWrite(tx_pin, bit);
    delay(10);
}

void sendData(int data[], int datasize) {
    // encode our data to manchester encoding
    int manData[datasize*2];
    encodeManchester(data, datasize, manData);

    // send preamble 7 times
    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 8; j++) {
            sendBit(preamble[j]);
        }
    }

    // send our sfd (start frame delimiter)
    for(int i = 0; i < 8; i++) {
        sendBit(sfd[i]);
    }

    // send our data
    for(int i = 0; i < datasize*2; i++) {
        sendBit(manData[i]);
    }

    // pull it down
    sendBit(0);
}

void loop() {
    sendData(test, 8);
    delay(100);
}
