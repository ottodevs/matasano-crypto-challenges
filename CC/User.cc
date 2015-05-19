#include "User.hh"


User::User(string email){

    if(id_gen == 0){
        globKey = Block::gen_random_block();
        id_gen++;
    }
    
    for(int i = 0; i < email.size(); ++i){
        if(email[i] == '&' or email[i] == '=')
            email[i] = '*';
    }

    properties["email"] = email;
    int uid = id_gen++;
    ostringstream convert;
    convert << uid;
    properties["uid"] = convert.str();
    properties["role"] = "user";

}

User::User(const vector<byte>& cipher){
    if(id_gen == 0){
        globKey = Block::gen_random_block();
        id_gen++;
    }
    vector<byte> v = Aes::aes_128_ECB_de(cipher, &globKey[0]);
    string str(v.begin(), v.end());
    properties = parse(str);
}

User::~User(){}

void User::update(const vector<byte>& cipher){
    vector<byte> v = Aes::aes_128_ECB_de(cipher, &globKey[0]);
    string str(v.begin(), v.end());
    properties = parse(str);
 
}

map<string,string> User::parse(string input){
    map<string,string> out;
    string keyS;
    vector<char> acum(input.size() + 1);
    int n = 0;

    for(int i= 0; i < input.size(); ++i){
        
        if(input[i] == '='){
            vector<char> key (n);
            for(int c = 0; c < n; ++c)
                key[c] = acum[c];
            string keyStr(key.begin(), key.end());
            keyS = keyStr;
            n = 0;
        }

        else if(input[i] == '&'){
            vector<char> value (n);
            for(int c = 0; c < n; ++c)
                value[c] = acum[c];
            string valueS(value.begin(), value.end());
            out[keyS] = valueS;
            n = 0;
        }

        else if(i == input.size() - 1){
            vector<char> value (n+1);
            for(int c = 0; c < n; ++c)
                value[c] = acum[c];
            value[n] = input[input.size()-1];
            string valueS(value.begin(), value.end());
            out[keyS] = valueS;
        }

        else{
            acum[n] = input[i];
            ++n;
        }
    }
    return out;
}

string User::getString(){
    return "email=" + properties["email"] + "&uid=" + properties["uid"] + "&role=" + properties["role"];
}

vector<byte> User::getEncryptedProfile(){
    string str = getString();
    vector<byte> v = Conversion::stringToByteArray(str);
    return Aes::aes_128_ECB_en(v, &globKey[0]);
}

vector<byte> User::encryptData(const string& in){

    vector<byte> v = Conversion::stringToByteArray(in);

    for(int i = 0; i < in.size(); ++i){
        if(in[i] == ';' or in[i] == '=') v[i] = (byte)'*';
    }

    vector<byte> append = Conversion::stringToByteArray(app);
    vector<byte> prepend = Conversion::stringToByteArray(pre);

    vector<byte> data = Block::append_arrays(v, append);
    data = Block::append_arrays(prepend, data);
    return (Aes::aes_128_CBC_en(data, &globKey[0], globKey));
 
}

bool User::searchString(vector<byte> v){
    vector<byte> b = Aes::aes_128_CBC_de(v, &globKey[0], globKey);

    string str (b.begin(), b.end());
    size_t found = str.find(";admin=true;");
    return (found!=string::npos);
}

int User::getUid(){
    int out;
    stringstream(properties["uid"]) >> out;
    return out;
}

vector<byte> User::globKey(16, 0);
int User::id_gen = 0;

