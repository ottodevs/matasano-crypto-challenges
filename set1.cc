#include "Conversion.hh"
#include "Attack.hh"
#include "Data.hh"
#include "File.hh"
#include "Aes.hh"
#include "Block.hh"
#include "Analysis.hh"
#include "User.hh"
#include "Target.hh"
#include <fstream>

//Convert hex to base64
void ch1(){
    Data d(ch1_str, 0); //string ch1_str located at utils.cpp
    d.toType(1);
    cout << d << endl;
}

//Fixed XOR
void ch2(){
    Data d1(ch2_str1);
    Data d2(ch2_str2);
    Data d3 = d1^d2;
    cout << d3 << endl;
}

//Single-byte XOR cipher
/*
 * SOL: Cooking MC's like a pound of bacon (key:88)
 */
void ch3(){
    Data cipher(ch3_str);
    testBytes(cipher, THRES); //THRES is a constant defined in utils.cpp
}

//Detect single-character XOR
/*
 * SOL: In line 171 with key 53 "Now that the party is jumping"
 */
void ch4(){
    searchOnFile("INPUT/ch4.txt", THRES);
}

//Implement repeating-key XOR
void ch5(){
    Data d(ch5_str, 2);
    d = d ^ "ICE";
    cout << d << endl;
}

//Break repeating-key XOR
/*
 * SOL: keysize is 29 bytes
 * key:"Terminator X: Bring the noise"
 * decrypts to the lyrics for "Play That Funky Music"
 */
void ch6(){
    Data cipher(fetchFromFile("INPUT/ch6.txt"));
    int ks = find_key_size(cipher, 2, 40);
    cout << ">Best Keysize: " << ks << endl;
    cout << "-------------------------" << endl << endl;
    vector<byte> keyV = findRepeatingKey(groupBlock(cipher,ks));

    cout << "detected key: ";
    printChar(keyV);
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
    Data plain = cipher ^ key;
    plain.toType(2);
    cout << plain << endl;
}

//AES in ECB mode
/*
 * SOL: decrypts to same lyrics as challege 6
 */
void ch7(){
    Data d(fetchFromFile("INPUT/ch7.txt"));
    d = aes_128_ECB_de(d, keySub);
    d.toType(2);
    d.pkcs7_strip_padding();
    cout << d << endl;
}

//Detect AES in ECB mode
/*
 * line 133 is probably encrypted with ECB (repeating Block found)
 */
void ch8(){
    find_ECB_line("INPUT/ch8.txt");
}


/*
 * problem 59 from project euler
 */
void ch9(){
    ifstream infile("/home/shierve/Code/Challenges/euler/p059_cipher.txt");
    string val_string;
    vector<byte> cipher;
    while(getline(infile, val_string, ',')){
        cipher.push_back((byte)stoi(val_string));
    }
    string abc = "abcdefghijklmnopqrstuvwxyz";

    int biggest = 0;
    vector<byte> key(3);
    vector<byte> best(3);
    for(int i = 0; i < abc.size(); ++i){
        for(int j = 0; j < abc.size(); ++j){
            for(int k = 0; k < abc.size(); ++k){
                key[0] = abc[i];
                key[1] = abc[j];
                key[2] = abc[k];
                vector<byte> result = repeating_key_xor(cipher, key);
                int n = frequency_evaluation(result);
                if(n > biggest){
                    biggest = n;
                    best[0] = key[0];
                    best[1] = key[1];
                    best[2] = key[2];
                }
            }
        }
    }

    cout << endl << "-------------------------" << endl;
    vector<byte> plain = repeating_key_xor(cipher, best);
    int t = 0;
    for(int i = 0; i < plain.size(); ++i){
        cout << plain[i];
        t += (int)plain[i];
    }
    cout << endl;
    cout << t << endl;
}

/*
 * MAIN
 */
int main(){
    int ch;
    cout << "[1-8]" << endl;
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
        case 9:
            ch9();
            break;
    }
}
