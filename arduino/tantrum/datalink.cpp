#include <Arduino.h>
#include "config.h"
#include "datalink-utils.h"
#include "physical.h"
#include "packet.h"

void sendData(int dest, int packetType, int data[], int dataSize) {
    int result[320];
    generate(dest, packetType, data, dataSize, result);

    transmitData(result, 72);
}

bool readData(basepacket *pck_ptr) {
    // make sure we are synced and ready for data
    sync();
    Serial.println("We have synched...");

    int header[56];
    if(!receiveData(56, header)) {
        Serial.println("Failed to read header");
        return false;
    }

    // check if the dest is us
    int dest[16];
    sliceArray(header, 0, 16, dest);
    pck_ptr->dest = binToInt(dest, 16);

    /* if(pck_ptr->dest != DEVICE_ID) { */
    /*     Serial.println("Not our device.."); */
    /*     return false; */
    /* } */

    // read our length to read body and bodycrcc
    int length[16];
    sliceArray(header, 40, 48, length);
    pck_ptr->length = binToInt(length, 8);

    // reading body
    if(!receiveData(pck_ptr->length, pck_ptr->body)) {
        Serial.println("Failed to read header");
        return false;
    }

    if(!receiveData(8, pck_ptr->bodycrc)) {
        Serial.println("Failed to read header");
        return false;
    }

    // everything is read, verify our header now!
    // we are doing it now, to not have missed any
    // potential bits... could maybe be done before
    // but this seems like the most save way of 
    // doing it
    int headerPlain[48];
    sliceArray(header, 0, 48, headerPlain);
    sliceArray(header, 48, 56, pck_ptr->headercrc);

    int summed[8], result[8];
    sumBin(header, 8, 48, summed);
    addBin(pck_ptr->headercrc, summed, 8, result);

    if(!complementVerify(result, 8)) {
        Serial.println("Invalid header..");
        return false;
    }

    sumBin(pck_ptr->body, 8, pck_ptr->length, summed);
    addBin(pck_ptr->bodycrc, summed, 8, result);

    if(!complementVerify(result, 8)) {
        Serial.println("Invalid body..");
        return false;
    }

    // decode rest and put into a struct
    int src[16], packetType[8];
    sliceArray(header, 16, 32, src);
    sliceArray(header, 32, 40, packetType);

    pck_ptr->src = binToInt(src, 16);
    pck_ptr->packetType = binToInt(packetType, 8);

    Serial.println("end-of-frame");

    return true;
}
