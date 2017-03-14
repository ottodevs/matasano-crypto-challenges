#include "Conversion.hh"
#include "Output.hh"
#include "File.hh"
#include "Aes.hh"
#include "Block.hh"
#include "Target.hh"
#include "User.hh"
#include "Xor.hh"
#include "Attack.hh"
#include "Analysis.hh"

//Implement PKCS#7 padding
void ch9(){
    string ys = "YELLOW SUBMARINE";
    vector<byte> v = stringToByteArray(ys);
    v = pkcs7_pad(v, 20);
    printChar(v);
    cout << endl;
}

//Implement CBC mode
/*
 * SOL: same lyrics as in set 1. Somebody really likes this song
 */
void ch10(){
    vector<byte> v = fetchFromFile("INPUT/ch10.txt");
    vector<byte> iv(16, 0);
    v = aes_128_CBC_de(v, keySub, iv);
    printChar(v);
    cout << endl;
}

//An ECB/CBC detection oracle
void ch11(){
    mode_detector(Target::rand_encryption);
}

//Byte-at-a-time ECB decryption(Simple)
/*
 * SOL: more pop lyrics, but different this time
 */
void ch12(){
    Target::globKey = gen_random_block();
    byte_at_a_time(Target::append_and_encrypt);
}

//ECB cut-and-paste
void ch13(){

    vector<byte> admin_str = stringToByteArray("XXXXXXXXXXadmin");
    admin_str = pkcs7_pad(admin_str, 16 + 10);
    string mail(admin_str.begin(), admin_str.end());
    mail += "XXXX"; // only works for uid < 10
    User mike(mail);

    vector<byte> cipher = mike.getEncryptedProfile();

    //cut and paste
    vector<byte> block1 (16);
    for(int i = 0; i < 16; ++i)
        block1[i] = cipher[i+16];
    for(int i = 0; i < 16; ++i){
        cipher[i+16] = cipher[16*3 + i];
        cipher[16*3 + i] = block1[i];
    }

    mike.update(cipher);
    string ss = mike.getString();
    vector<byte> v = stringToByteArray(ss);
    printChar(v);
    cout << endl;
}

//Byte-at-a-time ECB decryption(Harder)
void ch14(){
    Target::globKey = gen_random_block();
    Target::prepend = gen_random_bytes(-1);
    byte_at_a_time(Target::prepend_and_encrypt);
}

//PKCS#7 padding validation
void ch15(){
    vector<byte> v = stringToByteArray(ch15_str);
    v = pkcs7_pad(v, 16);
    printChar(v);
    if(remove_padding(v)) cout << endl << "padding removed" << endl;
    else cout << endl << "incorrect padding" << endl;
    vector<byte> b (4,5);
    v = append_arrays(v, b);
    printChar(v);
    if(remove_padding(v)) cout << endl << "padding removed" << endl;
    else cout << endl << "incorrect padding" << endl;
}

//CBC bitflipping attacks
void ch16(){
    //we use the previous chars to the forbidden ones on the ascii table and then apply a mask to change the last bit
    string obj = ":admin<true:";

    vector<byte> v = User::encryptData(obj);

    vector<byte> mask (v.size(), 0);
    mask[16] = 1;
    mask[16 + 6] = 1;
    mask[16 + 11] = 1;
    v = fixed_xor(v, mask);

    if(User::searchString(v)) cout << "admin" << endl;
    else cout << "not admin" << endl;
}

/*
 * MAIN
 */
int main(){
    int ch;
    cout << "[9-16]" << endl;
    cin >> ch;
    switch(ch){
        case 9:
            ch9();
            break;
        case 10:
            ch10();
            break;
        case 11:
            ch11();
            break;
        case 12:
            ch12();
            break;
        case 13:
            ch13();
            break;
        case 14:
            ch14();
            break;
        case 15:
            ch15();
            break;
        case 16:
            ch16();
            break;
    }
}
