#ifndef TARGET_HH
#define TARGET_HH

#include "utils.cpp"
#include "Data.hh"
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

    static Data rand_encryption(const Data& data);
    /* Pads and randomly encrypts ECB/CBC */

    static Data append_and_encrypt(const Data& data);

    static Data prepend_and_encrypt(const Data& data);

    Data encrypt_CBC(const Data& data); //IV: globKey  AES key: secretKey

    bool padding_oracle(const Data v, vector<byte>& iv); //unencrypts

    vector<byte> getSecretKey();

};
#endif
