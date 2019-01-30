#include <Arduino.h>

#define rx_pin 12
#define SAMPLE_SIZE 16

// sequences
int sfd[8] = {1,0,1,0,1,0,1,1};

void setup() {
    pinMode(rx_pin, INPUT);
    // Serial.println => 8 microsec
    /* Serial.begin(2000000); */
    // Serial.println => 8000 microsec
    Serial.begin(9600);
}

bool check_between(unsigned long v, unsigned long lower, unsigned long upper) {
    if((v > lower) && (v < upper)) {
        return true;
    }
    return false;
}

// check if we have a sync
// return the avg time between 1 and 0 shifts
// this can be used to calculate the interval
unsigned long check_sync() {
    int count = 0;
    unsigned long last_change = micros();
    int last_state = digitalRead(rx_pin);
    unsigned long samples[SAMPLE_SIZE];

    while(true) {
        int state = digitalRead(rx_pin);

        if(state != last_state) {
            unsigned long change_time = micros()-last_change;
            
            if(check_between(change_time, 9500, 10500)) {
                samples[count % SAMPLE_SIZE] = change_time;
                count++;
            } else {
                count = 0;
            }

			// (7*8) we send 7 times our preamble (8 bits)
            if(count == 56) {
                break;
            }

            last_change = micros();
            last_state = state;
        }
    }

    Serial.println("We have synced!");
    
    // calculate the avg time
    unsigned long avg = 0;
    for(int i = 0; i < SAMPLE_SIZE; i++) {
        avg += samples[i];
    }

    avg = avg/SAMPLE_SIZE;
    return avg;
}

// readBit();
// delay(10);
int readBit(int waitTime) {
    int state = digitalRead(rx_pin);
    delayMicroseconds(waitTime);
    return state;
}

bool compareArray(int array1[], int array2[], int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        if(array1[i] != array2[i]) {
            return false;
        }
    }
    
    return true;
}

void loop() {
    /* Serial.println(digitalRead(rx_pin)); */
    /* return; */
    // wait until we are synced, then
    // we assume, that we are right at the beginning
    // of the first packet after sync, so tage
    // the syncAvg, devide by two, and wait
    // that amount of time in micros, before
    // that we read the data, repeat.
    unsigned long syncAvg = check_sync();
    // wait so we are reading in the middle
    delayMicroseconds(syncAvg/2);

    // read our first 8 bits, should be sfd
    // 10101011
    int check_sfd[8];
    for(int i = 0; i < 8; i++) {
        check_sfd[i] = readBit(syncAvg);
        Serial.print(check_sfd[i]);
    }
    Serial.println();
    
    // check if it is sfd
    if(!compareArray(check_sfd, sfd, 8)) {
        return;
    }
    Serial.println("We got our sfd!");

    /* enc: [1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0] */
    /* 1001100101100110 */
    /* 1001100101100110 */
    /* 1001100110011001 */
    /* 10101011 */
    /*          1001100101100110 */
    /* 101010110100110010110011001010 */
    /* 101010111001100101100110010101 */
    // read the next 16 bits
    for(int i = 0; i < 16; i++) {
        Serial.print(readBit(syncAvg));
    }
    Serial.println();
}
