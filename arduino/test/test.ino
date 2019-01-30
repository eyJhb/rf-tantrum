#include <Arduino.h>

void setup() {
Serial.begin(9600);
Serial.println("Init");
}

struct basepacket {
    int body[255];
};

bool receiveData(basepacket *pck_ptr) {

    return true;
}

void loop() { 
    basepacket packet = {};
    if(!receiveData(&packet)) {
        delay(5);
        return;
    }

} 

/* 1 */
/* 1780 */
/* 1 */
/* 1780 */
/* 1 */
/* 1780 */


