#include <Arduino.h>
#include "config.h"
#include "datalink-utils.h"

void generate(unsigned int input_dest, int input_packettype, int input_data[], int arraySize, int result[]) {
    int dest[16], src[16], packetType[8], length[8];
    intToBin(input_dest, 16, dest);
    /* intToBin(DEVICE_ID, 16, src); */
    intToBin(0, 16, src);
    intToBin(input_packettype, 8, packetType);
    intToBin(arraySize, 8, length);

    int header[56];
    appendArray(dest, 16, 0, header);
    appendArray(src, 16, 16, header);
    appendArray(packetType, 8, 32, header);
    appendArray(length, 8, 40, header);

    // calculate crc and append
    int crc[8];
	sumBin(header, 8, 48, crc);
	complementBin(crc, 8, crc);
    appendArray(crc, 8, 48, header);

    // data and datacrc
    int crcbody[8];
	sumBin(input_data, 8, arraySize, crcbody);
	complementBin(crcbody, 8, crcbody);

    // append to our result
    appendArray(header, 56, 0, result);
    appendArray(input_data, arraySize, 56, result);
    appendArray(crcbody, 8, 56+arraySize, result);
}
