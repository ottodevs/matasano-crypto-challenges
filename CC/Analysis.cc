#include "Analysis.hh"

//PRIVATE

int bitsInByte(byte b){
    int bits = 0;
    for(int i = 0; i < 8; ++i){
        if (b % 2 != 0)
            bits++;
        b = b >> 1;
    }
    return bits;
}

string freq_string(vector<int>& v){
    int max = -1;
    int maxi = 0;
    string str = "";
    for(int it = 0; it < 26; ++it){
        for(int i = 0; i < v.size(); ++i){
            if(v[i] > max){
                max = v[i];
                maxi = i;
            }
        }
        str.push_back((char)(maxi+'a'));
        v[maxi] = -1;
        max = -1;
    }
    return str;
}

//PUBLIC

int hamming_distance(const vector<byte>& x, const vector<byte>& y){

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

int find_key_size(const Data& blockArray, int a, int b){

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

int frequency_evaluation(const vector<byte>& v){

    vector<int> frequency(26, 0);
    int symbols = 0;

    int score = 0;
    for (int i = 0; i < v.size(); ++i){
        char lt = letter(v[i]);
        if(lt == '?'){
            return -1;
        }
        else if(lt == '$'){
            symbols++;
        }
        else if(lt != ' '){
            frequency[(int)(lt-'a')]++;
        }
    }
    string fs = freq_string(frequency);

    //6 most frequent characters
    for(int i = 0; i < 6; ++i){
        for(int i2 = 0; i2 < 6; ++i2){
            if(frequency_table[i] == fs[i2])
                score+=3;
        }
    }

    //next 6
    for(int i = 6; i < 12; ++i){
        for(int i2 = 6; i2 < 12; ++i2){
            if(frequency_table[i] == fs[i2])
                score+=2;
        }
    }

    //last 6
    for(int i = 20; i < 26; ++i){
        for(int i2 = 20; i2 < 26; ++i2){
            if(frequency_table[i] == fs[i2])
                score++;
        }
    }

    //penalize results with lots of strange characters
    for(int i = 0; i < v.size(); ++i){
        //this chars penalize
        if(v[i] == '|' or v[i] == ';' or v[i] == '^' or v[i] == '`' or v[i] == '~' or v[i] == '<' or v[i] == '>')
            score -= 2;
    }

    //penalty for too many symbols
    int max_occ = v.size()/SYMB_FREQ;
    if (symbols > max_occ) score -= 7;

    //penalty for too many numbers
    /*int numbers = 0;
    for(int i = 0; i < v.size(); ++i){
        if((char)v[i] >= 48 and (char)v[i] < 58)
            numbers++;
    }
    if(numbers >= max_occ) score -= 7;*/

    return score;
}

vector<Data> groupBlock(const Data& blockArray, int keySize){
    int n = blockArray.size()/keySize;
    if(blockArray.size() % keySize != 0)
        n++;
    vector<Data> groupedBlock (keySize, Data(n));
    for (int i = 0; i < blockArray.size(); ++i){
        groupedBlock[i % keySize][(int) i / keySize] = blockArray[i];
    }
    return groupedBlock;
}

bool is_ECB(const vector<byte>& data){

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
            if (compareBlocks(blockData[i], blockData[j]))
                found = true;
        }
    }
    return found;
}

bool mode_detector(Data (*f)(const Data&)){
    Data v (16*3, 65);
    v = f(v);
    bool out;
    if(is_ECB(v.getData())){
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

bool is_time_seeded(unsigned int n, int threshold){
    rng gen;
    time_t now = time(0);
    bool found = false;
    for(int s = now; s >= now-threshold and not found; --s){
        gen.seed(s);
        if(gen.extract_number() == n)
            found = true;
    }
    return found;
}
