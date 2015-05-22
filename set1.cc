#include "Conversion.hh"
#include "Output.hh"
#include "Xor.hh"
#include "Attack.hh"
#include "File.hh"
#include "Aes.hh"
#include "Block.hh"
#include "Analysis.hh"
#include "User.hh"
#include "Target.hh"

//Convert hex to base64
void ch1(){
    vector<byte> v = Conversion::hexToByteArray(ch1_str); //string ch1_str located at utils.cpp
    v = Conversion::byteArrayToB64(v);
    Output::printB64(v);
}

//Fixed XOR
void ch2(){
    vector<byte> v1 = Conversion::hexToByteArray(ch2_str1);
    vector<byte> v2 = Conversion::hexToByteArray(ch2_str2);
    Output::printHex(Xor::fixed_xor(v1,v2));
}

//Single-byte XOR cipher
/*
 * SOL: Cooking MC's like a pound of bacon (key:88)
 */
void ch3(){
    vector<byte> cipher = Conversion::hexToByteArray(ch3_str);
    Attack::testBytes(cipher, THRES); //THRES is a constant defined in utils.cpp
}

//Detect single-character XOR
/*
 * SOL: In line 171 with key 53 "Now that the party is jumping"
 */
void ch4(){
    File::searchOnFile("INPUT/ch4.txt", THRES);
}

//Implement repeating-key XOR
void ch5(){
    vector<byte> v = Conversion::stringToByteArray(ch5_str);
    v = Xor::repeating_key_xor(v, "ICE");
    Output::printHex(v);
}

//Break repeating-key XOR
/*
 * SOL: keysize is 29 bytes
 * key:"Terminator X: Bring the noise"
 * decrypts to the lyrics for "Play That Funky Music"
 */
void ch6(){
    vector<byte> cipher = File::fetchFromFile("INPUT/ch6.txt");
    int ks = Analysis::find_key_size(cipher, 2, 40);
    cout << ">Best Keysize: " << ks << endl;
    cout << "-------------------------" << endl << endl;
    vector<byte> keyV = Attack::findRepeatingKey(Analysis::groupBlock(cipher,ks));
    
    cout << "detected key: ";
    Output::printChar(keyV);
    cout << endl << "Change? (y/n)";
    char ans;
    cin >> ans;

    string key;
    if(ans == 'Y' or ans == 'y'){
        char keyA[ks + 1];
        cout << endl << "introduce repeating key (" << ks << " characters):" << endl << ">>";
        cin.getline(keyA, ks+1);
        cin.getline(keyA, ks+1);
        key = string(keyA);
    }
    else{
        key = string(keyV.begin(), keyV.end());
    }
    cout << endl << "-------------------------" << endl;
    vector<byte> plain = Xor::repeating_key_xor(cipher, key);
    Output::printChar(plain);
    cout << endl;
}

//AES in ECB mode
/*
 * SOL: decrypts to same lyrics as challege 6
 */
void ch7(){
    vector<byte> v = File::fetchFromFile("INPUT/ch7.txt");
    v = Aes::aes_128_ECB_de(v, keySub);
    Output::printChar(v);
    cout << endl;
}

//Detect AES in ECB mode
/*
 * line 133 is probably encrypted with ECB (repeating Block found)
 */
void ch8(){
    Block::find_ECB_line("INPUT/ch8.txt");
}

/*
 * MAIN
 */
int main(){
    int ch;
    cin >> ch;
    switch(ch){
        case 1:
            ch1();
            break;
        case 2:
            ch2();
            break;
        case 3:
            ch3();
            break;
        case 4:
            ch4();
            break;
        case 5:
            ch5();
            break;
        case 6:
            ch6();
            break;
        case 7:
            ch7();
            break;
        case 8:
            ch8();
            break;
    }
}
