#include "Block.hh"


void Block::find_ECB_line(string filename){
    vector<byte> v;
    ifstream file;
    file.open(&filename[0]);
    string line;
    if(file.is_open()){
        int lineNum = 1;
        while(getline(file,line)){
            v = Conversion::hexToByteArray(line);
            if(Analysis::is_ECB(v)){
                cout << "Repeating block found at line " << lineNum << ":" << endl;
                cout << line << endl << endl; 
            }
            lineNum++;
        }
    }else{
        cout << "error opening file" << endl;
    }
}

bool Block::compareBlocks(const vector<byte>& block1, const vector<byte>& block2){
    bool equal = true;
    for(int i = 0; (i < block1.size()) and equal; ++i){
        if(block1[i] != block2[i])
            equal = false;
    }
    if(equal){
        //cout << "repeating block:" << endl;
        //printHex(block1);
        //cout << endl;
    }
    return equal;
}

bool Block::compareBlocks(byte *block1, byte *block2, int size){
    bool equal = true;
    for(int i = 0; i < size and equal; ++i){
        if (*block1 != *block2)
            equal = false;
        ++block1;
        ++block2;
    }
    return equal;
}

vector<byte> Block::gen_random_block(){
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);
    vector<byte> out (16);
    for(int i = 0; i < 16; ++i){
        out[i] = (byte) rand() % 256;
    }
    return out;
}

vector<byte> Block::gen_random_bytes(int n){
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);

    int k;
    if(n < 0) k = (int) rand() % 16;
    else k = n;

    vector<byte> out (k);
    for(int i = 0; i < k; ++i){
        out[i] = (byte) rand() % 256;
    }
    return out;
}

void Block::shiftBytes(vector<byte>& v, byte last){
    for(int i = 0; i < v.size() - 1; ++i){
        v[i] = v[i + 1];
    }
    v[v.size() - 1] = last;
}

vector<byte> Block::copyFrom(byte *in, int size){
    vector<byte> out (size);
    for(int i = 0; i < size; ++i){
        out[i] = *in;
        ++in;
    }
    return out;
}

vector<byte> Block::append_arrays(const vector<byte>& array1, const vector<byte>& array2){
    vector<byte> out (array1.size() + array2.size());
    for(int i = 0; i < array1.size(); ++i){
        out[i] = array1[i];
    }
    for(int i = 0; i < array2.size(); ++i){
        out[i + array1.size()] = array2[i];
    }
    return out;
}





