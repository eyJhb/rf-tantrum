#include <Arduino.h>

#include "config.h"
#include "datalink.h"
#include "datalink-utils.h"

void setup() {
    // prepare our pins
    pinMode(TX_PIN, OUTPUT);
    pinMode(RX_PIN, INPUT);

    Serial.begin(2000000);
    /* Serial.begin(9600); */
    Serial.println("Initialized");
}

void testSend() {
    int data[8];
    intToBin(1, 8, data);
    sendData(0, 2, data, 8);
    delay(2000);
}

void testRead() {
    // maximum packet length
    basepacket packet = {};
    if(!readData(&packet)) {
        delay(5);
        return;
    }

    Serial.print("Dest address: ");
    Serial.println(packet.dest);
    Serial.print("Src address: ");
    Serial.println(packet.src);

    if(packet.packetType == 1) {
        Serial.println("---Got ack");
    } else if (packet.packetType == 2) {
        Serial.println("---Got resend");
    } else {
        Serial.println("---Other header");
    }
}

void loop() { 
    //testSend();
    testRead();
} 
