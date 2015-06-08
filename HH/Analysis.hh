#ifndef ANALYSIS_HH
#define ANALYSIS_HH

#include "utils.cpp"
#include "Block.hh"
#include "RNG.hh"
#include <algorithm>


/*
 * Functions and methods for
 * analysing data
 */

struct Letter{
    char value;
    int occurences;
}; 

extern int bitsInByte(byte b); //returns '1' bits in a byte

extern bool comp(Letter l1, Letter l2); //comparation for the sort function

extern int hamming_distance(const vector<byte>& x, const vector<byte>& y); //returns hamming/edit distance

extern int find_key_size(const vector<byte>& blockArray, int a, int b); //prints normalized values (a to b) in order

extern int frequency_evaluation(const vector<byte>& v); //gives a score to plaintext

extern vector< vector<byte> > groupBlock(vector<byte> blockArray, int keySize); //groups a block to find the repeating key

extern bool is_ECB(const vector<byte>& data);
//searches for repeated blocks. Useful only with certain types of data which tend to repeate, like images, or pop song lyrics

extern bool mode_detector(vector<byte> (*f)(const vector<byte>&));
/* Detects the mode (ECB/CBC) the pointed function is encrypting in true:ECB*/

extern bool is_time_seeded(unsigned int n, int threshold);

#endif
