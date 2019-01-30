//Always use include guards
#ifndef PHYSICAL_H
#define PHYSICAL_H

//All function prototypes from PHYSICAL.cpp.
void transmitData(int data[], int datasize);
bool receiveData(int readings, int result[]);
bool sync();

#endif /* PHYSICAL_H */
