#ifndef XOR_HH
#define XOR_HH

#include "utils.cc"
#include "Conversion.hh"

class Xor {
    /*
     * xor operations with bytes
     */


private:

public:
    
    static vector<byte> fixed_xor(const vector<byte>& data1, const vector<byte>& data2); //produces xor of two byte arrays
    /* Pre: the two arrays are the same size */
    /* Post: bitwise xor of two arrays */

    static vector<byte> single_key_xor(const vector<byte>& buffer1, byte k);
    /* Pre: true */
    /* Post: encrypts every byte with the same key (k) */

    static vector<byte> repeating_key_xor(const vector<byte>& in, string key); //repeating key xor
    
    static vector<byte> block_xor(byte *v, byte *b); //xor of two 128-bit blocks 

};
#endif
