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
      Serial.println("We are here!");
      if (arrayResult[7] == 1) {
        arrayResult[7] = 0;
        carry = 0;
        Serial.println("We are here!");
      }
      else if (arrayResult[7] == 0) {
        Serial.println("We are here!");
        arrayResult[7] = 1;
        carry = 0;
      }
    }

  }
}  




void setup() {
  Serial.begin(9600);

  int arrayEt[8] = {1, 1, 0, 0, 1, 0, 1, 0};
  int arrayTo[8] = {1, 0, 1, 1, 1, 1, 1, 1};
  int arrayResult[8];

  binaryAddition(arrayEt, arrayTo, arrayResult);

  for (int i = 0; i < 8; i++) {
    Serial.print(arrayResult[i]);
  }
  
  Serial.println();

}

void loop()
{
}
