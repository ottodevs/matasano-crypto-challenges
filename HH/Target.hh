#ifndef TARGET_HH
#define TARGET_HH

#include "utils.cc"
#include "Aes.hh"
#include "Block.hh"
#include "Conversion.hh"
#include <openssl/aes.h>

#include "Output.hh"

class Target {
    /*
     * Simulated target with methods for encrypting
     */
private:
    
    vector<byte> secretKey;

public:

    static vector<byte> globKey;
    static vector<byte> prepend;
    /* needs to be initialized in global scope:
     * vector<byte> globKey(16,0); */

    Target();

    ~Target();

    static vector<byte> rand_encryption(const vector<byte>& data);
    /* Pads and randomly encrypts ECB/CBC */

    static vector<byte> append_and_encrypt(const vector<byte>& data);

    static vector<byte> prepend_and_encrypt(const vector<byte>& data);

    vector<byte> encrypt_CBC(vector<byte>& data); //IV: globKey  AES key: secretKey

    bool padding_oracle(const vector<byte>& v, vector<byte>& iv); //unencrypts

    vector<byte> getSecretKey();

};
#endif
