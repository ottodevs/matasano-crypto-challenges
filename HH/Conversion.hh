#ifndef CONVERSION_HH
#define CONVERSION_HH

#include "utils.cc"

class Conversion {
    /*
     * conversion functions and methods
     */
    
public:
 
    static int hexToDec(char hex); //hex digit to dec

    static byte charToB64(char c); //returns the byte for a char on base64

    static vector<byte> hexToByteArray(const string& hex); //hex string to byte array

    static vector<byte> byteArrayToB64(const vector<byte>& v); //byte array into base64 byte array

    static vector<byte> b64ToByteArray(const vector<byte>& v); //b64 array to byte array

    static vector<byte> stringToByteArray(const string& str); //ascii string to byte array. Kind of stupid, just for simplicity

    static vector<byte> b64StringToByteArray(const string& str);

    static vector<byte> intToByteArray(const unsigned long int &num, bool invert);
    /* turns 8byte long int into byte array. invert true to turn little endian to big endian and viceversa */

};
#endif
