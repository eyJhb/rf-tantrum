#include <Arduino.h>

bool sliceArray(int input[], int start, int end, int result[]) {
    if (start < 0) {
        return false;
    }

    int index = 0;
    for(int i = start; i < end; i++) {
        result[index] = input[i];
        index++;
    }

    return true;
}

void appendArray(int value[], int arraySize, int start, int result[]) {
    int counter = 0;
    for(int i = start; i < start+arraySize; i++) {
        result[i] = value[counter];
        counter++;
    }
}

void intToBin(unsigned int value, int pad, int result[]) {
    for (int i=0; i < pad; i++){
        result[i] = 0;
    }

    for (int i = pad-1; i>=0; i--){
        int valueMod = value % 2;
        value = value/2;
        result[i] = valueMod;
    }
}

// For loop through binary array[]
// initiate int conv = 0
// First multiply 2 * conv + value[i] = conv
// Continue through loop until i < binarySize + 1
unsigned int binToInt(int binary[], int binSize) {
    unsigned int conv = 0;
    for (int i = 0; i < binSize; i++){
        conv = (2 * conv) + binary[i];
    }
    return conv;
}

void addBin(int x[], int y[], int arraySize, int result[]) {
    int carry = 0;
    int val = 0;

    for (int i = arraySize - 1; i >= 0; i--) {
        // Add the corresponding values from each
        // array and the carry, and save the number
        // in int val
        val = x[i] + y[i] + carry;
        carry = 0;

        if (val == 3) {
            result[i] = 1;
            carry = 1;
        } else if (val == 2) {
            result[i] = 0;
            carry = 1;
        } else {
            result[i] = val; 
        }
    }

    if (carry == 1) {
		if (result[7] == 1) {
			result[7] = 0;
		} else {
			result[7] = 1;
		}
	}
}


//Tag et array af størrelse arraySize som input
//for hvert element i det array, sæt til modsatte værdi i after array
//returner efter arrayet
void complementBin(int value[], int arraySize, int result[]){
    for (int i=0; i < arraySize; i++){
        if (value[i] == 0){
            result[i] = 1;
        } else if (value[i] == 1){
            result[i] = 0;
        }
    }
}

bool complementVerify(int x[], int arraySize){
    for (int i=0; i < arraySize; i++){
        if (x[i] != 1) {
            return false;
        }
    }
    return true;
}

bool sumBin(int v[], int splitSize, int sizeOfArray, int res[]) {
    if (sizeOfArray % splitSize != 0) {
        return false;
    }

    int curWord[splitSize];

    // set initial previous word
    for (int n = 0; n < splitSize; n++) {
        res[n] = v[n];
    }

    /* Serial.println("SUMBIN----"); */
    /* Serial.println(sizeOfArray); */
    /* Serial.println(sizeOfArray / splitSize); */
    for (int i = 1; i < (sizeOfArray / splitSize); i++) { //i = amount of blocks
        int index = 0;

        for (int j = i * splitSize; j < (i * 8) + 8; j++) {
            curWord[index] = v[j];
            index++;
        }

        /* for (int j = 0; j < 8; j++) { */
        /*     Serial.print(res[j]); */
        /* } */
        /* Serial.println(); */
        /* for (int j = 0; j < 8; j++) { */
        /*     Serial.print(curWord[j]); */
        /* } */
        /* Serial.println(); */

        /* Serial.println("--"); */
        
        // calculates curWord+res and puts it into res
        addBin(curWord, res, splitSize, res);
        /* for (int j = 0; j < 8; j++) { */
        /*     Serial.print(res[j]); */
        /* } */
        /* Serial.println(); */
    }
    /* Serial.println("----SUMBIN"); */
    return true;
}
