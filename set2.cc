#include "Conversion.hh"
#include "Output.hh"
#include "File.hh"
#include "Aes.hh"
#include "Block.hh"
#include "Data.hh"
#include "Target.hh"
#include "User.hh"
#include "Xor.hh"
#include "Attack.hh"
#include "Analysis.hh"

// Implement PKCS#7 padding
void ch9(){
    string ys = "YELLOW SUBMARINE";
    Data d(ys, 2);

    d.pkcs7_pad(20);
    cout << d << endl;
}

// Implement CBC mode
/*
 * SOL: same lyrics as in set 1. Somebody really likes this song
 */
void ch10(){
    Data d = Data(fetchFromFile("INPUT/ch10.txt"));

    vector<byte> iv(16, 0);
    d = aes_128_CBC_de(d, keySub, iv);
    d.pkcs7_strip_padding();
    d.toType(2);
    cout << d << endl;
}

// An ECB/CBC detection oracle
void ch11(){
    mode_detector(Target::rand_encryption);
}

// Byte-at-a-time ECB decryption(Simple)
/*
 * SOL: more pop lyrics, but different this time
 */
void ch12(){
    Target::globKey = gen_random_block();
    byte_at_a_time(Target::append_and_encrypt);
}

// ECB cut-and-paste
void ch13(){

    Data admin_str("XXXXXXXXXXadmin", 2);

    admin_str.pkcs7_pad(16 + 10);
    vector<byte> av = admin_str.getData();
    string mail(av.begin(), av.end());
    mail += "XXXX"; // only works for uid < 10
    User mike(mail);

    Data cipher = mike.getEncryptedProfile();

    // cut and paste
    vector<byte> block1(16);
    for (int i = 0; i < 16; ++i) {
        block1[i] = cipher[i + 16];
    }
    for (int i = 0; i < 16; ++i) {
        cipher[i + 16] = cipher[16 * 3 + i];
        cipher[16 * 3 + i] = block1[i];
    }

    mike.update(cipher);
    string ss = mike.getString();
    Data dd(ss, 2);
    cout << dd << endl;
} // ch13

// Byte-at-a-time ECB decryption(Harder)
void ch14(){
    Target::globKey = gen_random_block();
    Target::prepend = gen_random_bytes(-1);
    byte_at_a_time(Target::prepend_and_encrypt);
}

// PKCS#7 padding validation
void ch15(){
    Data d(ch15_str, 2);

    d.pkcs7_pad(16);
    cout << d << endl;
    if (d.pkcs7_validate_padding()) {
        d.pkcs7_strip_padding();
        cout << endl << "padding removed" << endl;
    } else cout << endl << "incorrect padding" << endl;
    Data b(4, 5);
    d += b;
    cout << d << endl;
    if (d.pkcs7_validate_padding()) {
        d.pkcs7_strip_padding();
        cout << endl << "padding removed" << endl;
    } else cout << endl << "incorrect padding" << endl;
}

// CBC bitflipping attacks
void ch16(){
    // we use the previous chars to the forbidden ones on the ascii table and then apply a mask to change the last bit
    string obj = ":admin<true:";

    Data d = User::encryptData(obj);

    vector<byte> mask(d.size(), 0);
    mask[16] = 1;
    mask[16 + 6] = 1;
    mask[16 + 11] = 1;
    d = d ^ mask;

    if (User::searchString(d)) cout << "admin" << endl;
    else cout << "not admin" << endl;
}

/*
 * MAIN
 */
int main(){
    int ch;

    cout << "[9-16]" << endl;
    cin >> ch;
    switch (ch) {
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
    } // switch
} // main
