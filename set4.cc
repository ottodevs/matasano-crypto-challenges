#include "Output.hh"
#include "File.hh"
#include "Aes.hh"
#include "Stream.hh"
#include "Conversion.hh"

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


int main(){
    ch25();
}
