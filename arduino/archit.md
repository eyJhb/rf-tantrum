# Architecture

## Global variables

### Defines
- **Pin**
    - TX_PIN = 12
    - RX_PIN = 13
- **Variables**
    - SAMPLE_SIZE = 16

### Constants
- **Arrays**
    - Preamble
        - preamble[8] = {1,0,1,0,1,0,1,0}
    - SFD
        - sfd[8] = {1,0,1,0,1,0,1,1};

## Functions
- **Receive**
    - readBit()
        - OUTPUT:
            - HIGH or LOW
    -  receiveData()
        - OUTPUT:
            - void
    - checkSync()
        - OUTPUT:
            - avg => unsigned long
    - checkSfd()
        - OUTPUT:
            - true or false
- **Send**
    - sendBit(int bit)
        - INPUT:
            - bit
    - sendData(int data[], int datasize)
        - INPUT:
            - data[] => Array of bits
            - datasize => length  of data
- **Encode**
    - encodeManchester(int data[], int datasize, int result[])
        - INPUT:
            - data[] => Array of bits
            - datasize => length  of data
        - OUTPUT:
            - result[] => Array of bits
- **Check**
    - compareArray(int array1[], int array2[], int arraySize);
        - INPUT:
            - array1[] => Array of bits
            - array2[] => Array of bits
            - arraySize => length  of arrays to compare
        - OUTPUT:
            - true or false    
    - check_between(unsigned long v, unsigned long lower, unsigned long upper)
        - INPUT:
            - v => unsigned long
            - lower => unsigned long
            - upper => unsigned long
        - OUTPUT:
            - true or false
- **Print**
    - printArray(int data[], int arraySize)
        - INPUT
            - data[] => Array of bits
            - arraySize => length  of data
        - OUTPUT:
            - Data in the array
