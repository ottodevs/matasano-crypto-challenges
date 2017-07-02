#ifndef CONVERSION_HH
#define CONVERSION_HH

#include "utils.cpp"

/*
 * conversion functions and methods
 */

extern int hexToDec(char hex); // hex digit to dec

extern byte charToB64(char c); // returns the byte for a char on base64

extern char letter(byte c); // returns the letter if it is a letter, '$' for symbols, '?' for non ascii values

extern vector<byte> hexToByteArray(const string& hex); // hex string to byte array

extern vector<byte> byteArrayToB64(const vector<byte>& v); // byte array into base64 byte array

extern vector<byte> b64ToByteArray(const vector<byte>& v); // b64 array to byte array

extern vector<byte> stringToByteArray(const string& str); // ascii string to byte array.

extern vector<byte> b64StringToArray(const string& str);

extern vector<byte> b64StringToByteArray(const string& str);

extern vector<byte> intToByteArray(const uint64_t &num, bool invert);
/* turns 8byte long int into byte array. invert true to turn little endian to big endian and viceversa */


#endif // ifndef CONVERSION_HH
