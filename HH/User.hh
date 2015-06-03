#ifndef USER_HH
#define USER_HH

#include "utils.cpp"
#include "Aes.hh"
#include "Conversion.hh"
#include "Block.hh"
#include "Output.hh"
#include <sstream>

class User {
    /* user identity with various properties */

private:

    static int id_gen;
    static vector<byte> globKey;

public:

    map<string,string> properties;

    User(string email);

    User(const vector<byte>& cipher);

    ~User();

    void update(const vector<byte>& cipher);

    static map<string,string> parse(string input);

    string getString();

    vector<byte> getEncryptedProfile();

    static vector<byte> encryptData(const string& in);

    static vector<byte> encryptData_CTR(const string& in);

    static bool searchString(vector<byte> v); //unencrypts and searches for string ";admin=true;"
    static bool searchString_CTR(vector<byte> v);
    
    //returns decrypted vector if high ascii is found, else returns an empty vector
    static vector<byte> check_ascii(vector<byte>& v);

    int getUid();

};
#endif
