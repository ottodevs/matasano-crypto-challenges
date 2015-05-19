#ifndef ANALYSIS_HH
#define ANALYSIS_HH

#include "utils.cc"
#include "Block.hh"
#include <algorithm>

class Analysis {
    /*
     * Functions and methods for
     * analysing data
     */

private:

    struct Letter{
        char value;
        int occurences;
    }; 

    static int bitsInByte(byte b); //returns '1' bits in a byte

    static bool comp(Letter l1, Letter l2); //comparation for the sort function


public:

    static int hamming_distance(const vector<byte>& x, const vector<byte>& y); //returns hamming/edit distance

    static int find_key_size(const vector<byte>& blockArray, int a, int b); //prints normalized values (a to b) in order

    static int frequency_evaluation(const vector<byte>& v); //gives a score to plaintext

    static vector< vector<byte> > groupBlock(vector<byte> blockArray, int keySize); //groups a block to find the repeating key
    
    static bool is_ECB(const vector<byte>& data);
    //searches for repeated blocks. Useful only with certain types of data which tend to repeate, like images, or pop song lyrics

    static bool mode_detector(vector<byte> (*f)(const vector<byte>&));
    /* Detects the mode (ECB/CBC) the pointed function is encrypting in true:ECB*/

};
#endif
