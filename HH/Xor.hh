#ifndef XOR_HH
#define XOR_HH

#include "utils.cpp"
#include "Conversion.hh"
#include "Data.hh"

/*
 * xor operations with bytes
 */


extern vector<byte> fixed_xor(const vector<byte>& data1, const vector<byte>& data2);
    /* Pre: the two arrays are the same size */
    /* Post: bitwise xor of two arrays */

extern vector<byte> single_key_xor(const vector<byte>& buffer1, byte k);
    /* encrypts every byte with the same key (k) */

extern vector<byte> repeating_key_xor(const vector<byte>& in, string key);
extern vector<byte> repeating_key_xor(const vector<byte>& in, const vector<byte>& key);
    /* Repeating key XOR*/

extern vector<byte> block_xor(byte *v, byte *b); //xor of two 128-bit blocks

#endif
