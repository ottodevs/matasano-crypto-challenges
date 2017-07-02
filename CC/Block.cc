#include "Block.hh"


void find_ECB_line(string filename){
    vector<byte> v;
    ifstream file;
    file.open(&filename[0]);
    string line;
    if (file.is_open()) {
        int lineNum = 1;
        while (getline(file, line)) {
            v = hexToByteArray(line);
            if (is_ECB(v)) {
                cout << "Repeating block found at line " << lineNum << ":" << endl;
                cout << line << endl << endl;
            }
            lineNum++;
        }
    } else {
        cout << "error opening file" << endl;
    }
}

bool compareBlocks(const vector<byte>& block1, const vector<byte>& block2){
    bool equal = true;

    for (int i = 0; (i < block1.size()) and equal; ++i) {
        if (block1[i] != block2[i])
            equal = false;
    }
    if (equal) {
        // cout << "repeating block:" << endl;
        // printHex(block1);
        // cout << endl;
    }
    return equal;
}

bool compareBlocks(byte * block1, byte * block2, int size){
    bool equal = true;

    for (int i = 0; i < size and equal; ++i) {
        if (*block1 != *block2)
            equal = false;
        ++block1;
        ++block2;
    }
    return equal;
}

vector<byte> gen_random_block(){
    time_t secs;

    time(&secs);
    srand((unsigned int)secs);
    vector<byte> out(16);
    for (int i = 0; i < 16; ++i) {
        out[i] = (byte)rand() % 256;
    }
    return out;
}

vector<byte> gen_random_bytes(int n){
    time_t secs;

    time(&secs);
    srand((unsigned int)secs);

    int k;
    if (n < 0) k = (int)rand() % 16;
    else k = n;

    vector<byte> out(k);
    for (int i = 0; i < k; ++i) {
        out[i] = (byte)rand() % 256;
    }
    return out;
}

void shiftBytes(Data& d, byte last){
    for (int i = 0; i < d.size() - 1; ++i) {
        d[i] = d[i + 1];
    }
    d[d.size() - 1] = last;
}

Data copyFrom(byte * in, int size){
    Data out(size);

    for (int i = 0; i < size; ++i) {
        out[i] = *in;
        ++in;
    }
    return out;
}

vector<byte> append_arrays(const vector<byte>& array1, const vector<byte>& array2){
    vector<byte> out(array1.size() + array2.size());
    for (int i = 0; i < array1.size(); ++i) {
        out[i] = array1[i];
    }
    for (int i = 0; i < array2.size(); ++i) {
        out[i + array1.size()] = array2[i];
    }
    return out;
}
