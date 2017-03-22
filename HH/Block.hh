#ifndef BLOCK_HH
#define BLOCK_HH

#include "utils.cpp"
#include "Conversion.hh"
#include "Data.hh"
#include "Analysis.hh"
#include <cstdlib>
#include <time.h>
#include <fstream>


/*
 * Tools for working with blocks and block crypto
 */

extern void find_ECB_line(string filename); //Searches for an ECB encrypted line with function is_ECB and prints repeating strings

extern bool compareBlocks(const vector<byte>& block1, const vector<byte>& block2); //returns true for equal blocks

extern bool compareBlocks(byte *block1, byte *block2, int size);

extern vector<byte> gen_random_block(); //returns a random 128-bit block

extern vector<byte> gen_random_bytes(int n);
    /* returns n random bytes. if n < 0 randomizes n between 0 and 15 */

extern void shiftBytes(Data& d, byte last); //shifts left and assigns last

Data copyFrom(byte *in, int size);

extern vector<byte> append_arrays(const vector<byte>& array1, const vector<byte>& array2);

#endif
