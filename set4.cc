#include "Output.hh"
#include "File.hh"
#include "Aes.hh"
#include "Stream.hh"
#include "Conversion.hh"
#include "User.hh"

void ch25(){
    vector<byte> v = fetchFromFile("INPUT/ch7.txt");
    v = aes_128_ECB_de(v, keySub);
    vector<byte> nonce = intToByteArray(12530, false);
    v = aes_128_CTR(v, keySub, nonce);
    vector<byte> plain;
    
    for(int i = 0; i < v.size(); ++i){
        bool f = false;
        vector<byte> letter;
        letter.push_back(0);
        for(int b = 0; b < 256 and not f; ++b){
            letter[0] = b;
            if(v == edit_CTR(v, keySub, nonce, i, letter)){
               f = true;
               plain.push_back(b);
               cout << (char)b;
            }
        }
    }
    
    // plain has the unencrypted content
    cout << endl;
}

void ch26(){
    string obj = ":admin<true:";

    vector<byte> v = User::encryptData_CTR(obj);

    vector<byte> mask (v.size(), 0);
    mask[32] = 1;
    mask[32 + 6] = 1;
    mask[32 + 11] = 1;
    v = fixed_xor(v, mask);

    if(User::searchString_CTR(v)) cout << "admin" << endl;
    else cout << "not admin" << endl;
}

void ch27(){
    //encrypt url string with User (3 blocks)
    string s = "www.aaaaaaaaaaaa.aaaaaaaaaaaaaaa.aaaaaaaaaaa.com";
    vector<byte> v = User::encryptData(s);
    //modify the message C1, 0, C1
    for(int i = 16; i < 32; ++i){
        v[i] = 0;
        v[i + 16] = v[i - 16];
    }
    //key: P1' xor P3'
    v = User::check_ascii(v);
    if(v.empty())
        cout << "good ascii" << endl;
    else{
        vector<byte> k = block_xor(&v[0], &v[32]);
        cout << "key found: ";
        printHex(k);
    }
}

int main(){
    ch27();
}
