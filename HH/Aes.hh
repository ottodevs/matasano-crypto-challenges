#ifndef AES_HH
#define AES_HH

#include "utils.cpp"
#include "Data.hh"
#include "Xor.hh"
#include "Conversion.hh"
#include "Block.hh"
#include <openssl/aes.h>

/*
 * block encryption/decryption with AES
 */

extern vector<byte> aes_encrypt(byte *v, const byte keyS[]); //encrypts a single 128-bit block

extern vector<byte> aes_decrypt(byte *v, const byte keyS[]); //decrypts a single 128-bit block

extern Data aes_128_ECB_en(const Data& plain, const byte keyS[]); //returns encrypted byte array

extern Data aes_128_ECB_de(const Data& d, const byte keyS[]); //returns decrypted byte array

extern Data aes_128_CBC_en(const Data& plain, const byte keyS[], const vector<byte>& iv);

extern Data aes_128_CBC_de(const Data& d, const byte keyS[], vector<byte>& iv);

extern Data aes_128_CTR(const Data& d, const byte keyS[], const vector<byte>& nonce);

#endif
