#ifndef UTILS_CRYPTO
#define UTILS_CRYPTO

#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

typedef unsigned char byte;

const char table_64[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                          'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                          'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                          'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                          'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                          'w', 'x', 'y', 'z', '0', '1', '2', '3',
                          '4', '5', '6', '7', '8', '9', '+', '/',
                          '=' };

const char table_hex[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

const char frequency_table[] = { 'e', 't', 'a', 'o', 'i', 'n', 's',
                                 'h', 'r', 'd', 'l', 'c', 'u', 'm',
                                 'w', '&', 'f', 'g', 'y', 'p', 'b','v',
                                 'k', 'j', 'x', 'q', 'z' };

const int THRES = 11;
const int SYMB_FREQ = 5; // [Decryption::frequency_evaluation]

const string ch1_str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
const string ch2_str1 = "1c0111001f010100061a024b53535009181c";
const string ch2_str2 = "686974207468652062756c6c277320657965";
const string ch3_str = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"; // Cooking MC's like a pound of bacon
const string line171 = "7b5a4215415d544115415d5015455447414c155c46155f4058455c5b523f"; // Now that the party is jumping
const string ch5_str = "Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal";
const string ch6_key = "Terminator X: Bring the noise";
const byte keySub[] = "YELLOW SUBMARINE";
const string b64append = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
const string ch15_str = "ICE ICE BABY";
const string pre = "comment1=cooking%20MCs;userdata=";
const string app = ";comment2=%20like%20a%20pound%20of%20bacon";
const string ch18_str = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";


// pads data to given size in bytes
inline vector<byte> pkcs7_pad(const vector<byte>& data, int size){
    byte n = size - data.size();

    vector<byte> out(size, n);
    for (int i = 0; i < data.size(); ++i) {
        out[i] = data[i];
    }
    return out;
}

// strips pkcs#7 padding

inline bool remove_padding(vector<byte>& data){
    int num = data[data.size() - 1];

    if (num > data.size()) return false;
    else if (num == 0) return false;
    bool cond = true;
    for (int i = data.size() - num; i < data.size() and cond; ++i) {
        if (data[i] != (byte)num) cond = false;
    }
    if (cond) {
        vector<byte> out(data.size() - num);
        for (int i = 0; i < out.size(); ++i) {
            out[i] = data[i];
        }
        data = out;
    }
    return cond;
}

#endif // ifndef UTILS_CRYPTO
