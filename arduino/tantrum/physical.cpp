#include <Arduino.h>
#include "config.h"
#include "physical-utils.h"

unsigned long syncAverage = 0;
unsigned long syncLast = 0;
unsigned long sentLast = 0;

void sendBit(int bit) {
    digitalWrite(TX_PIN, bit);
    delay(10);
}

int readBit() {
    int state = digitalRead(RX_PIN);
    while((micros()-syncLast) < syncAverage) {
    }
    syncLast = micros();
    return state;
}

bool receiveData(int readings, int result[]) {
    int manBits[readings*2];
    for(int i = 0; i < readings*2; i++) {
        manBits[i] = readBit();
    }

    if(!decodeManchester(manBits, readings*2, result)) {
        Serial.println("decodeManchester failed");
        return false;
    }

    return true;
}

void transmitData(int data[], int datasize) {
    // encode our data to manchester encoding
    int manData[datasize*2];
    encodeManchester(data, datasize, manData);
    sentLast = micros();

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

    // send our sfd (start frame delimiter)
    for(int i = 0; i < 8; i++) {
        sendBit(sfd[i]);
    }
}

// check if we have a sync
// return the avg time between 1 and 0 shifts
// this can be used to calculate the interval
unsigned long checkPreamble() {
    int count = 0;
    /* unsigned long startTime = millis(); */
    unsigned long lastChange = micros();
    int lastState = digitalRead(RX_PIN);
    unsigned long samples[SAMPLE_SIZE];

    while(true) {
        int state = digitalRead(RX_PIN);

        if(state != lastState) {
            unsigned long changeTime = micros()-lastChange;
            
            if(checkBetween(changeTime, 9000, 11000)) {
                samples[count % SAMPLE_SIZE] = changeTime;
                count++;
            } else {
                count = 0;
            }

			// (7*8) we send 7 times our preamble (8 bits)
            if(count == 56) {
                break;
            }

            lastChange = micros();
            lastState = state;
        }
    }

    // calculate the avg time
    unsigned long avg = 0;
    for(int i = 0; i < SAMPLE_SIZE; i++) {
        avg += samples[i];
    }

    avg = avg/SAMPLE_SIZE;
    return avg;
    /* return true; */
}

bool checkSfd() {
    // read 8 bits, should be sfd
    int con = 0;
    for(int i = 0; i < 8; i++) {
        int state = readBit();
        if(state) {
            con++;
        } else {
            con = 0;
        }

        if(con == 2) {
            return true;
        }
    }
    return false;
}

bool sync() {
    syncAverage = checkPreamble();
    // wait so we are reading in the middle
    delayMicroseconds(syncAverage/2);
    Serial.println("Passed sync");
    syncLast = micros();

    if(!checkSfd()) {
        Serial.println("Not sfd");
        return false;
    }
    Serial.println("Passed sfd");
    return true;
}
