
bool checkBetween(unsigned long v, unsigned long lower, unsigned long upper) {
    if((v > lower) && (v < upper)) {
        return true;
    }
    return false;
}

void encodeManchester(int data[], int datasize, int result[]) {
    int current_index = 0;
    for(int i = 0; i < datasize; i++) {
        if(data[i] == 1) {
            result[current_index] = 0;
            result[current_index+1] = 1;
        } else {
            result[current_index] = 1;
            result[current_index+1] = 0;
        }
        current_index += 2;
    }
}

bool decodeManchester(int data[], int datasize, int result[]) {
    int current_index = 0;
    for(int i = 0; i < datasize/2; i++) {
        if(data[(i*2)] == 0 && data[(i*2)+1] == 1) {
            result[current_index] = 1;
        } else if(data[(i*2)] == 1 && data[(i*2)+1] == 0) {
            result[current_index] = 0;
        } else {
            return false;
        }
        current_index += 1;
    }
    return true;
}


