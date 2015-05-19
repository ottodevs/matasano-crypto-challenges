#ifndef AES_HH
#define AES_HH

#include "utils.cc"
#include "Xor.hh"
#include "Conversion.hh"
#include "Block.hh"
#include <openssl/aes.h>

class Aes {
    /*
     * block encryption/decryption with AES
     */

private:

public:

    static vector<byte> aes_encrypt(byte *v, const byte keyS[]); //encrypts a single 128-bit block

    static vector<byte> aes_decrypt(byte *v, const byte keyS[]); //decrypts a single 128-bit block

    static vector<byte> aes_128_ECB_en(const vector<byte>& plainData, const byte keyS[]); //returns encrypted byte array

    static vector<byte> aes_128_ECB_de(const vector<byte>& v, const byte keyS[]); //returns decrypted byte array

    static vector<byte> aes_128_CBC_en(vector<byte>& plainData, const byte keyS[], vector<byte>& iv);

    static vector<byte> aes_128_CBC_de(vector<byte>& v, const byte keyS[], vector<byte>& iv);

    static vector<byte> aes_128_CBC_de_NP(vector<byte>& v, const byte keyS[], vector<byte>& iv); //does not remove padding

    static vector<byte> aes_128_CTR(const vector<byte>& v, const byte keyS[], const vector<byte>& nonce);

};
#endif
