#include "Analysis.hh"

//PRIVATE

int Analysis::bitsInByte(byte b){
    int bits = 0;
    for(int i = 0; i < 8; ++i){
        if (b % 2 != 0)
            bits++;
        b = b >> 1;
    }
    return bits;
}

bool Analysis::comp(Letter l1, Letter l2){
    if(l1.occurences > l2.occurences)
        return true;
    return false;
}

//PUBLIC

int Analysis::hamming_distance(const vector<byte>& x, const vector<byte>& y){

    vector<byte> out (x.size());
    for(int i = 0; i < x.size(); ++i){
        out[i] = x[i] xor y[i];
    }

    int total = 0;
    for (int i = 0; i < x.size(); ++i){
        total += bitsInByte(out[i]);
    }
    return total;
}

int Analysis::find_key_size(const vector<byte>& blockArray, int a, int b){

    cout << "Normalized values:" << endl;

    vector<float> values (b - a + 1); //number of keysizes to try

    for(int k = a; k <= b; ++k){
        vector<byte> part1 (k);
        vector<byte> part2 (k);
        float acum = 0;
        int n;
        for(int cc = 0; cc < b; ++cc){
            for(n = 1; n < blockArray.size()/k and n < 1000; ++n){
                for(int i = 0; i < k; ++i){
                    part1[i] = blockArray[i];
                    part2[i] = blockArray[i + (k*n)];
                }
                acum += (float) hamming_distance(part1, part2)/k;
            }
        }
        values[k - a] = (float) acum/(n*b);
    }

    
    int ret;
    for(int i = 0; i < values.size(); ++i){
        int min = values[0];
        for(int i2 = 0; i2 < values.size(); ++i2){
            if (values[i2] < values[min]){
                min = i2;
            }
        }
        if(i == 0) ret = min;
        cout << "keysize " << min + a <<  ": " << values[min] << endl;
        values[min] = 10000;
    }
    return ret + a;
}

int Analysis::frequency_evaluation(const vector<byte>& v){
    vector<Letter> frequency (26 + 1); //26 letters + 1 for symbols
    int score = 0;
    for (int i = 0; i < 26+1; ++i){
        frequency[i].value = frequency_table[26 + 1 - i];
        frequency[i].occurences = 0;
        for (int i2 = 0; i2 < v.size(); ++i2){
            char ch = frequency[i].value;
            if(ch != '&'){
                if (ch == (char)v[i2] or ch == (char)(v[i2] + 32))
                    frequency[i].occurences++;
            }
            else if(((char)v[i2] >= 33 and (char)v[i2] <= 47) or ((char)v[i2] >= 58 and (char)v[i2] <= 64)
                   or ((char)v[i2] >= 91 and (char)v[i2] <= 96) or ((char)v[i2] >= 123 and (char)v[i2] <= 126)){
                frequency[i].occurences++;
            }
        }
    }
    sort (frequency.begin(), frequency.end(), comp);
    //frequency is sorted

    for(int i = 0; i < 6; ++i){
        for(int i2 = 0; i2 < 6; ++i2){
            if(frequency_table[i] == frequency[i2].value)
                score+=3;
        }
    }

    /*
    for(int i = 3; i < 6; ++i){
        for(int i2 = 3; i2 < 6; ++i2){
            if(frequency_table[i] == frequency[i2].value)
                score++;
        }
    }
    */
 
    for(int i = 6; i < 12; ++i){
        for(int i2 = 6; i2 < 12; ++i2){
            if(frequency_table[i] == frequency[i2].value)
                score+=2;
        } 
    }
    
    for(int i = 20; i < 26; ++i){
        for(int i2 = 20; i2 < 26; ++i2){
            if(frequency_table[i] == frequency[i2].value)
                score++;
        }
    }
    
    //eliminate results with strange characters
    for(int i = 0; i < v.size(); ++i){
        if ((v[i] >= 127 or v[i] < 32) and v[i] != 8 and v[i] != 9 and v[i] != 10)
            return -1;
    }

    //penalty for too many symbols
    int max_occ = v.size()/SYMB_FREQ;
    for(int i = 0; i < frequency.size(); ++i){
        if(frequency[i].value == '&'){
            if(frequency[i].occurences >= max_occ)
                return -1;
            else
                return score;
        }
    }

    return score;
}

vector< vector<byte> > Analysis::groupBlock(vector<byte> blockArray, int keySize){
    int n = blockArray.size()/keySize;
    if(blockArray.size() % keySize != 0)
        n++;
    vector< vector<byte> > groupedBlock (keySize, vector<byte> (n));
    for (int i = 0; i < blockArray.size(); ++i){
        groupedBlock[i % keySize][(int) i / keySize] = blockArray[i];
    }
    return groupedBlock;
}

bool Analysis::is_ECB(const vector<byte>& data){
    
    int n = (int) data.size()/16;
    if(data.size() % 16 != 0)
        n++;
    vector< vector<byte> > blockData (n, vector<byte> (16, 0));
    for(int i = 0; i < data.size(); ++i){
        int a = (int) i / 16;
        int b = i % 16;
        blockData[a][b] = data[i];
    }
    bool found = false;
    for(int i = 0; (i < blockData.size()) and (not found); ++i){
        for(int j = i + 1; (j < blockData.size()) and (not found); ++j){
            if (Block::compareBlocks(blockData[i], blockData[j]))
                found = true;
        }
    }
    return found;
}

bool Analysis::mode_detector(vector<byte> (*f)(const vector<byte>&)){
    vector<byte> v (16*3, 65);
    v = f(v);
    bool out;
    if(is_ECB(v)){
        cout << "ECB detected" << endl;
        //cout << "---------------------------" << endl;
        out = true;
    }else{
        cout << "CBC detected" << endl;
        //cout << "---------------------------" << endl;
        out = false;
    }
    //printHex(v);
    return out;

}




