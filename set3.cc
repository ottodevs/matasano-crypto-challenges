#include "File.hh"
#include "Target.hh"
#include "User.hh"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "Block.hh"
#include "Conversion.hh"
#include "Attack.hh"
#include "Output.hh"
#include "Aes.hh"
#include "RNG.hh"
#include "Stream.hh"

//The CBC padding oracle
void ch17(){
    Target google;
    Target::globKey = gen_random_block();
    vector<string> lines = getStrings("INPUT/ch17.txt");
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);
    int line = (int) rand() % 10;

    vector<byte> cipher =  b64StringToByteArray(lines[line]);

    cipher = google.encrypt_CBC(cipher);

    vector<byte> plain = padding_oracle_attack(cipher, Target::globKey, google);
    remove_padding(plain);
    cout << "string found!!" << endl;
    printChar(plain);
    cout << endl;
}

//Implement CTR
void ch18(){
    vector<byte> v = b64StringToByteArray(ch18_str);
    unsigned long int nc = 0;
    vector<byte> nonce = intToByteArray(nc, false);
    v = aes_128_CTR(v, keySub, nonce);
    printChar(v);
    cout << endl;
}

//Break fixed-nonce CTR mode (Substitutions)
void ch19(){
    vector<string> strings = getStrings("INPUT/ch19.txt");
    vector<byte> nonce = intToByteArray(0, false);

    int maxSize = 0;
    for(int i = 0; i < strings.size(); ++i){
        if(strings[i].size() > maxSize)
            maxSize = strings[i].size();
    }
    bool exit = false;
    vector<byte> keys(maxSize, 0);
    while(not exit){
        for(int i = 0; i < strings.size(); ++i){
            cout << "[" << i + 1 << "]";
            vector<byte> v = aes_128_CTR(b64StringToByteArray(strings[i]), keySub, nonce);
            for(int l = 0; l < v.size(); ++l)
                v[l] = keys[l] xor v[l];
            printChar(v);
            cout << endl;
        }
        cout << endl << "Add substitution:";
        int line_num, key;
        cin >> line_num >> key;
        if(key <= 0)
            keys[line_num]++;
        else
            keys[line_num] = (byte)key;
        cout << "key " << line_num << "-->" << (int)keys[line_num] <<  endl;
    }
}

//Break fixed-nonce CTR mode (statistically)
void ch20(){

    /*
     * the solution found is a little off because the frequency_evaluation function
     * is not perfect, so to get the desired output you need to manually evaluate the output
     * and change some bytes of the detected key
     */

    vector<string> strings = getStrings("INPUT/ch20.txt");
    vector<byte> nonce = intToByteArray(0, false);

    vector< vector<byte> > ciphers;
    for(int i = 0; i < strings.size(); ++i){
        ciphers.push_back(aes_128_CTR(b64StringToByteArray(strings[i]), keySub, nonce));
    }

    int min_size = ciphers[0].size();
    for(int i = 1; i < ciphers.size(); ++i){
        if (ciphers[i].size() < min_size)
            min_size = ciphers[i].size();
    }
    cout << "min: " << min_size << endl;

    for(int i = 0; i < ciphers.size(); ++i){
        while(ciphers[i].size() != min_size)
            ciphers[i].pop_back();
    }
    vector< vector<byte> > groupedBlock(min_size);

    for(int i = 0; i < groupedBlock.size(); ++i){
        for(int j = 0; j < ciphers.size(); ++j)
            groupedBlock[i].push_back(ciphers[j][i]);
    }

    vector<byte> keyV = findRepeatingKey(groupedBlock);

    cout << endl << "detected key: ";
    printInt(keyV);
    cout << endl << "-------------------------" << endl;
    cout << "i key to change a byte, -1 to end:" << endl;
    int in, ke;
    cin >> in;
    while(in != -1){
        cin >> ke;
        keyV[in] = (byte)ke;
        cin >> in;
    }
    cout << "new key: ";
    printInt(keyV);

    for(int i = 0; i < strings.size(); ++i){
        if(i % min_size == 0)
            cout << endl;
        vector<byte> plain = repeating_key_xor(ciphers[i], keyV);

        printChar(plain);
        cout << endl;
    }
}

void ch21(){
    cout << "introduce seed to generate random numbers: ";
    int seed;
    cin >> seed;
    rng mt(seed);
    //100 random numbers:
    for(int i = 0; i < 100; ++i)
        cout << mt.extract_number() << endl;
}

void ch22(){
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);
    int time1 = (int) rand() % 1000; 
    int time2 = (int) rand() % 1000;


    sleep(time1);

    time_t seed = time(0);
    rng mt(seed);

    sleep(time2);

    unsigned int n = mt.extract_number();
    cout << "number generated: " << n << endl;
    cout << "seed: ";
    time_t start = time(0);
    bool found = false;
    for(int d = 0; d <= 1000 and not found; ++d){
        rng r(start - d);
        if(r.extract_number() == n){
            found = true;
            cout << start - d << endl;
        }
    }
}


void ch23(){
    rng rand(453256);
    unsigned int state[624];
    cout << "generator output(numbers 625-645):" << endl;
    for(int i = 0; i < 624; ++i){
        unsigned int n = rand.extract_number();
        state[i] = mt19937_untemper(n);
    }
    for(int i = 0; i < 20; ++i)
        cout << rand.extract_number() << endl;
    rng clone(&state[0]);
    cout << "cloned generator output(first 20 numbers):" << endl;
    for(int i = 0; i < 20; ++i)
        cout << clone.extract_number() << endl;
}

void ch24(){
    string message = "AAAAAAAAAAAAAA";
    vector<byte> v = stringToByteArray(message);
    vector<byte> p = gen_random_bytes(-1);
    v = append_arrays(p, v);
    v = mt19937(v, 33212);
    int key;
    bool found = false;
    for(key = 0; key < 65536 and not found; ++key){
        p = mt19937(v, key);
        bool dec = true;
        for(int j = v.size()-1; j >= v.size()-14 and dec; --j){
            if(p[j] != 'A') dec = false;
        }
        found = dec;
    }
    key--;
    if(found){
        cout << "key: " << key << endl;
        printChar(p);
        cout << endl;
    }
    else cout << "not found" << endl;
    cout << endl << "-----------------------" << endl << endl;

    time_t now = time(0);
    rng gen(now);
    unsigned int n1 = gen.extract_number();
    gen.seed(234321);
    unsigned int n2 = gen.extract_number();

    if(is_time_seeded(n1, 500)) cout << "is time seeded" << endl;
    else cout << "not timeseeded" << endl;
    if(is_time_seeded(n2, 500)) cout << "is time seeded" << endl;
    else cout << "not timeseeded" << endl;
}

/*
 * MAIN
 */
int main(){
    int ch;
    cin >> ch;
    switch(ch){
        case 17:
            ch17();
            break;
        case 18:
            ch18();
            break;
        case 19:
            ch19();
            break;
        case 20:
            ch20();
            break;
        case 21:
            ch21();
            break;
        case 22:
            ch22();
            break;
        case 23:
            ch23();
            break;
        case 24:
            ch24();
            break;
    } 
}
