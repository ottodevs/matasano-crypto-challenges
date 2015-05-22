#ifndef OUTPUT_HH
#define OUTPUT_HH

#include "utils.cpp"

class Output {
    /*
     * contains methods for printing
     * data in different types
     */
 
public:

    static void printB64(const vector<byte>& v);
    /* byte array is printed as base64 */


    static void printHex(const vector<byte>& v);
    /* byte array is printed Hex encoded */

    static void printInt(const vector<byte>& v);
    /* byte array is printed as int values */

    static void printChar(const vector<byte>& v);
    /* byte array is printed as char array */

    static void printAscii(int i);
    static void printAscii(byte i);
    /* Outputs ascii characters if they are letters or common symbols
     * For other characters it outputs as \int */

};
#endif
