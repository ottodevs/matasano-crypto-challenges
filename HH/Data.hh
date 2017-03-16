#ifndef DATA_HH
#define DATA_HH

#include "utils.cpp"
#include "Aes.hh"
#include "Conversion.hh"
#include <sstream>

class Data {
    /* byte vector with operations */

private:

    vector<byte> data;
    int type; //0 == byte array, 1 = b64 array, 2 = ASCII array

    void toB64();
    void toByteArray();

public:

    Data();

    Data(int i);

    Data(int i, byte b);

    Data(string input, int type); //0 = "hex" | 1 = "b64" | 2 = "str"

    Data(vector<byte> data);

    ~Data();

    vector<byte> getData() const;

    int size() const;
    int getType() const;
    void setType(int t); //only changes variable, not actual data. (should not be used, see toType)

    void toType(int t);

    void pkcs7_pad(int newSize);
    void pkcs7_strip_padding();
    bool pkcs7_validate_padding();

    void operator+=(Data& d);
    void operator+=(byte b);
    byte& operator[](int i);
    byte operator[](int i) const;
};
extern Data operator+(Data& a, Data& b);
extern Data operator+(Data& a, byte b);
extern ostream &operator<<(ostream& os, const Data& d);

#endif
