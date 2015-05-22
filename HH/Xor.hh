#ifndef XOR_HH
#define XOR_HH

#include "utils.cpp"
#include "Conversion.hh"

class Xor {
    /*
     * xor operations with bytes
     */


private:

public:
    
    static vector<byte> fixed_xor(const vector<byte>& data1, const vector<byte>& data2);
    /* Pre: the two arrays are the same size */
    /* Post: bitwise xor of two arrays */

    static vector<byte> single_key_xor(const vector<byte>& buffer1, byte k);
    /* encrypts every byte with the same key (k) */

    static vector<byte> repeating_key_xor(const vector<byte>& in, string key);
    static vector<byte> repeating_key_xor(const vector<byte>& in, vector<byte>& key);
    /* Repeating key XOR*/
    
    static vector<byte> block_xor(byte *v, byte *b); //xor of two 128-bit blocks 

};
#endif
