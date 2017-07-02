#ifndef USER_HH
#define USER_HH

#include "utils.cpp"
#include "Aes.hh"
#include "Conversion.hh"
#include "Data.hh"
#include "Block.hh"
#include "Output.hh"
#include <sstream>

class User {
/* user identity with various properties */

private:

static int id_gen;
static vector<byte> globKey;

public:

map<string, string> properties;

User(string email);

User(const Data& cipher);

~User();

void update(const Data& cipher);

static map<string, string> parse(string input);

string getString();

Data getEncryptedProfile();

static Data encryptData(const string& in);

static Data encryptData_CTR(const string& in);

static bool searchString(const Data& d);     // unencrypts and searches for string ";admin=true;"
static bool searchString_CTR(const Data& d);

// returns decrypted vector if high ascii is found, else returns an empty vector
static Data check_ascii(const Data& d);

int getUid();

};
#endif // ifndef USER_HH
