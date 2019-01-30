//Always use include guards
#ifndef DATALINK_H
#define DATALINK_H

//All function prototypes from LINK.cpp.
bool readData(basepacket *pck_ptr);
void sendData(int dest, int packetType, int data[], int dataSize);

#endif /* DATALINK_H */
