//Always use include guards
#ifndef CONFIG_H
#define CONFIG_H

#define TX_PIN 12
#define RX_PIN 13
#define SAMPLE_SIZE 16

const int preamble[8] = {1,0,1,0,1,0,1,0};
const int sfd[8] = {1,0,1,0,1,0,1,1};
const int DEVICE_ID = 0;
/* int test[8] = {1,1,1,1,1,1,1,1}; */
/* int test[8] = {0,0,0,0,0,0,0,0}; */

struct basepacket {
    unsigned int dest;
    unsigned int src;
    unsigned int packetType;
    unsigned int length;
    int headercrc[8];
    int body[255];
    int bodycrc[8];
};


#endif /* CONFIG_H */
