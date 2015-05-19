#ifndef BLOCK_HH
#define BLOCK_HH

#include "utils.cc"
#include "Conversion.hh"
#include "Analysis.hh"
#include <cstdlib>
#include <time.h>
#include <fstream>


class Block {
    /*
     * Tools for working with blocks and block crypto
     */

private:

public:

    static void find_ECB_line(string filename); //Searches for an ECB encrypted line with function is_ECB and prints repeating strings

    static bool compareBlocks(const vector<byte>& block1, const vector<byte>& block2); //returns true for equal blocks

    static bool compareBlocks(byte *block1, byte *block2, int size);

    static vector<byte> gen_random_block(); //returns a random 128-bit block

    static vector<byte> gen_random_bytes(int n);
    /* returns n random bytes. if n < 0 randomizes n between 0 and 15 */

    static void shiftBytes(vector<byte>& v, byte last); //shifts left and assigns last

    static vector<byte> copyFrom(byte *in, int size);

    static vector<byte> append_arrays(const vector<byte>& array1, const vector<byte>& array2);

};
#endif
