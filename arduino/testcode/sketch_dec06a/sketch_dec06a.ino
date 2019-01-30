// Bit to Binary convertion
// Step 1: Create function taking two inputs, v (value) and pad (padding)
// Step 2: Header scenario, v = 16 bit int and pad = 8 bit int
// Step 3:

void oneComplement(int before[], int after[], int arraySize){
  //Tag et array af størrelse arraySize som input
  //for hvert element i det array, sæt til modsatte værdi i after array
  //returner efter arrayet
  for (int i=0; i < arraySize; i++){
    if (before[i] == 0){
      after[i] = 1;
    }
    else if (before[i] == 1){
      after[i] = 0;
    }
  }
}

void binaryAddition(int arrayEt[], int arrayTo[], int arrayResult[]) {
  int carry = 0;
  int val = 0;

  for (int i = 7; i >= 0; i--) {
    //Serial.print("Index: ");
    //Serial.println(i);

    val = arrayEt[i] + arrayTo[i] + carry; // Add the corresponding values from each array and the carry, and save the number in int val

    if (val == 3) {
      arrayResult[i] = 1;
      carry = 1;
    }

    else if (val == 2) {
      arrayResult[i] = 0;
      carry = 1;
    }

    else if (val == 1) {
      arrayResult[i] = 1;
      carry = 0;
    }

    else if (val == 0) {
      arrayResult[i] = 0;
      carry = 0;
    }

    //Serial.print("Val: ");
    //Serial.println(val);

    if (i == 0 && (val == 2 || val == 3)) {
      //Serial.println("We are here!");
      if (arrayResult[7] == 1) {
        arrayResult[7] = 0;
        carry = 0;
        //Serial.println("We are here!");
      }
      else if (arrayResult[7] == 0) {
        //Serial.println("We are here!");
        arrayResult[7] = 1;
        carry = 0;
      }
    }

  }
}

bool blockDivision(int v[], int splitSize, int sizeOfArray, int res[]) {
  Serial.println(sizeOfArray);
  bool error = true;
  if (sizeOfArray % splitSize != 0) {
    error = false;
  }

  
  int curWord[splitSize];
  int prevWord[splitSize];

  // set initial previous word
  for (int n = 0; n < splitSize; n++) {
    prevWord[n] = v[n];
  }

  for (int i = 1; i < (sizeOfArray / splitSize); i++) { //i = amount of blocks
    Serial.println("Printout of the blocks:");
    int index = 0;
    for (int j = i * splitSize; j < (i * 8) + 8; j++) {
      Serial.print(v[j]); //print the blocks
      curWord[index] = v[j];
      index++;
    }
  binaryAddition(curWord, prevWord, res);
  }
  return error;
}

void convDecBin(int binarySize, unsigned int value, int result[]) {
  for (int i = 0; i < binarySize - 1; i++) {
    result[i] = 0;
  }

  for (int i = binarySize - 1; i >= 0; i--) {
    int valueMod = value % 2;
    value = value / 2;
    result[i] = valueMod;
  }
}

unsigned int convBinDec(int binary[], int binSize) {
  // For loop through binary array[]
  // initiate int conv = 0
  // First multiply 2 * conv + value[i] = conv
  // Continue through loop until i < binarySize + 1
  unsigned int conv = 0;
  for (int i = 0; i < binSize; i++) {
    conv = (2 * conv) + binary[i];
  }
  return conv;
}

void setup()
{
  Serial.begin(9600);

  int binarySize = 16;
  int result[binarySize];
  //convDecBin(binarySize, 65535, result);
  //for (int i = 0; i < binarySize; i++) {
  //Serial.println(result[i]);
  //}

  int binSize  = 16;
  int binary[binSize];
  //unsigned int dec = convBinDec(result, binSize);

  //Serial.println(dec);
  int v[] = {1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0};
  int sizeOfArray = (sizeof(v) / sizeof(int));
  int splitSize = 8;
  int res[splitSize];
  blockDivision(v, splitSize, sizeOfArray, res);
  Serial.println();
  Serial.println("Printout of result after binary addition:");
  for (int m = 0; m < splitSize; m++){
    Serial.print(res[m]);
  }

  int after[splitSize];
  oneComplement(res, after, 8);
  Serial.println();
  Serial.println("Printout of result after ones complement");
  for (int m = 0; m < splitSize; m++){
    Serial.print(after[m]);
  }
  
}



void loop()
{
}
