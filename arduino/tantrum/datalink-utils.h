//Always use include guards
#ifndef DATALINK_UTILS_H
#define DATALINK_UTILS_H

//All function prototypes from DATALINK-UTILS.cpp.
bool sumBin(int v[], int splitSize, int sizeOfArray, int res[]);
bool complementVerify(int x[], int arraySize);
void complementBin(int value[], int arraySize, int result[]);
void addBin(int x[], int y[], int arraySize, int result[]);
unsigned int binToInt(int binary[], int binSize);
void intToBin(unsigned int value, int pad, int result[]);
void appendArray(int value[], int arraySize, int start, int result[]);
bool sliceArray(int input[], int start, int end, int result[]);

#endif /* DATALINK_UTILS_H */
