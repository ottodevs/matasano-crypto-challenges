#include "File.hh"
#include "Target.hh"
#include "User.hh"
#include <cstdlib>
#include <time.h>
#include "Block.hh"
#include "Conversion.hh"
#include "Attack.hh"
#include "Output.hh"
#include "Aes.hh"

vector<byte> Target::globKey (16, 0);
vector<byte> Target::prepend (16, 0);
vector<byte> User::globKey (16, 0);
int User::id_gen = 0;

//The CBC padding oracle
void ch17(){
    Target google;
    Target::globKey = Block::gen_random_block();
    vector<string> lines = File::getStrings("INPUT/ch17.txt");
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);
    int line = (int) rand() % 10;

    vector<byte> cipher =  Conversion::b64StringToByteArray(lines[line]);

    cipher = google.encrypt_CBC(cipher);

    vector<byte> plain = Attack::padding_oracle_attack(cipher, Target::globKey, google);
    remove_padding(plain);
    cout << "string found!!" << endl;
    Output::printChar(plain);
    cout << endl;
}

void ch18(){
    vector<byte> v = Conversion::b64StringToByteArray(ch18_str);
    unsigned long int nc = 0;
    vector<byte> nonce = Conversion::intToByteArray(nc, false);
    v = Aes::aes_128_CTR(v, keySub, nonce);
    Output::printChar(v);
    cout << endl;
}

void ch19(){
    vector<string> strings = File::getStrings("INPUT/ch19.txt");
    vector<byte> nonce = Conversion::intToByteArray(0, false);

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
            vector<byte> v = Aes::aes_128_CTR(Conversion::b64StringToByteArray(strings[i]), keySub, nonce);
            for(int l = 0; l < v.size(); ++l)
                v[l] = keys[l] xor v[l];
            Output::printChar(v);
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

/*
 * MAIN
 */
int main(){
    ch19();
}
