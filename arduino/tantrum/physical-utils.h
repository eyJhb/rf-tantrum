//Always use include guards
#ifndef PHYSICAL_UTILS_H
#define PHYSICAL_UTILS_H

//All function prototypes from PHYSICAL_UTILS.cpp.
bool checkBetween(unsigned long v, unsigned long lower, unsigned long upper);
void encodeManchester(int data[], int datasize, int result[]);
bool decodeManchester(int data[], int datasize, int result[]);

#endif /* PHYSICAL_UTILS_H */
