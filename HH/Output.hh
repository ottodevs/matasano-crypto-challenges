#ifndef OUTPUT_HH
#define OUTPUT_HH

#include "utils.cpp"

/*
 * contains methods for printing
 * data in different types
 */

extern void printB64(const vector<byte>& v);
/* byte array is printed as base64 */


extern void printHex(const vector<byte>& v);
/* byte array is printed Hex encoded */

extern void printInt(const vector<byte>& v);
/* byte array is printed as int values */

extern void printChar(const vector<byte>& v);
/* byte array is printed as char array */

extern void printAscii(int i);
extern void printAscii(byte i);
/* Outputs ascii characters if they are letters or common symbols
 * For other characters it outputs as \int */

#endif // ifndef OUTPUT_HH
