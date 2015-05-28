#ifndef FILE_HH
#define FILE_HH

#include "utils.cpp"
#include "Conversion.hh"
#include "Attack.hh"
#include <fstream>


extern void searchOnFile(const string filename, int thres);
    /* Uses the Decryption::testBytes function on every line of the file and prints results passing the threshold */

extern vector<byte> fetchFromFile(const string filename); //returns contents of multi-line b64 encoded file in a single byte array

extern vector<string> getStrings(const string filename); //all lines on file are put into a string vector

#endif
