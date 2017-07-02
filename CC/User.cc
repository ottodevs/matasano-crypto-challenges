#include "User.hh"

vector<byte> User::globKey = gen_random_block();
int User::id_gen = 0;

User::User(string email){

    if (id_gen == 0) {
        globKey = gen_random_block();
        id_gen++;
    }

    for (int i = 0; i < email.size(); ++i) {
        if (email[i] == '&' or email[i] == '=')
            email[i] = '*';
    }

    properties["email"] = email;
    int uid = id_gen++;
    ostringstream convert;
    convert << uid;
    properties["uid"] = convert.str();
    properties["role"] = "user";

}

User::User(const Data& cipher){
    if (id_gen == 0) {
        globKey = gen_random_block();
        id_gen++;
    }
    Data d = aes_128_ECB_de(cipher, &globKey[0]);
    d.pkcs7_strip_padding();
    vector<byte> v = d.getData();
    string str(v.begin(), v.end());
    properties = parse(str);
}

User::~User(){
}

void User::update(const Data& cipher){
    Data d = aes_128_ECB_de(cipher, &globKey[0]);

    d.pkcs7_strip_padding();
    vector<byte> v = d.getData();
    string str(v.begin(), v.end());
    properties = parse(str);

}

map<string, string> User::parse(string input){
    map<string, string> out;
    string keyS;
    vector<char> acum(input.size() + 1);
    int n = 0;

    for (int i = 0; i < input.size(); ++i) {

        if (input[i] == '=') {
            vector<char> key(n);
            for (int c = 0; c < n; ++c) {
                key[c] = acum[c];
            }
            string keyStr(key.begin(), key.end());
            keyS = keyStr;
            n = 0;
        } else if (input[i] == '&') {
            vector<char> value(n);
            for (int c = 0; c < n; ++c) {
                value[c] = acum[c];
            }
            string valueS(value.begin(), value.end());
            out[keyS] = valueS;
            n = 0;
        } else if (i == input.size() - 1) {
            vector<char> value(n + 1);
            for (int c = 0; c < n; ++c) {
                value[c] = acum[c];
            }
            value[n] = input[input.size() - 1];
            string valueS(value.begin(), value.end());
            out[keyS] = valueS;
        } else {
            acum[n] = input[i];
            ++n;
        }
    }
    return out;
} // User::parse

string User::getString(){
    return "email=" + properties["email"] + "&uid=" + properties["uid"] + "&role=" + properties["role"];
}

Data User::getEncryptedProfile(){
    string str = getString();
    Data d(str, 2);

    return aes_128_ECB_en(d, &User::globKey[0]);
}

Data User::encryptData(const string& in){

    Data d(in, 2);

    for (int i = 0; i < in.size(); ++i) {
        if (in[i] == ';' or in[i] == '=') d[i] = (byte)'*';
    }

    Data append(app, 2);
    Data prepend(pre, 2);

    Data data = prepend + d + append;
    return (aes_128_CBC_en(data, &User::globKey[0], globKey));

}

Data User::encryptData_CTR(const string& in){

    Data d(in, 2);

    vector<byte> nonce = intToByteArray(12530, false);
    for (int i = 0; i < in.size(); ++i) {
        if (in[i] == ';' or in[i] == '=') d[i] = (byte)'*';
    }

    Data append(app, 2);
    Data prepend(pre, 2);

    Data data = prepend + d + append;
    return (aes_128_CTR(data, &User::globKey[0], nonce));

}

bool User::searchString(const Data& d){
    Data b = aes_128_CBC_de(d, &User::globKey[0], globKey);

    vector<byte> v = b.getData();
    string str(v.begin(), v.end());
    size_t found = str.find(";admin=true;");
    return (found != string::npos);
}

bool User::searchString_CTR(const Data& d){
    vector<byte> nonce = intToByteArray(12530, false);
    Data b = aes_128_CTR(d, &User::globKey[0], nonce);

    vector<byte> v = b.getData();
    string str(v.begin(), v.end());
    size_t found = str.find(";admin=true;");
    return (found != string::npos);
}

Data User::check_ascii(const Data& d){
    Data b = aes_128_CBC_de(d, &User::globKey[0], globKey);
    bool high_ascii = false;

    for (int i = 0; i < b.size(); ++i) {
        if (b[i] > 127) high_ascii = true;
    }
    if (high_ascii) return b;
    else return vector<byte>();
}

int User::getUid(){
    int out;

    stringstream(properties["uid"]) >> out;
    return out;
}
