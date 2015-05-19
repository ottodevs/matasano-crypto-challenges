#include "Target.hh"


Target::Target(){
    secretKey = Block::gen_random_block();
}

Target::~Target(){}

vector<byte> Target::rand_encryption(const vector<byte>& data){
    vector<byte> iv,key;
    iv = Block::gen_random_block();
    key = Block::gen_random_block();
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);

    //pad(rand)
    int n = rand() % 6;
    n += 5;
    vector<byte> out (data.size() + n);
    for(int i = 0; i < n; ++i){
        out[i] = (byte) n;
    }
    for(int i = 0; i < data.size(); ++i){
        out[n + i] = data[i];
    }
    out = pkcs7_pad(out, data.size() + n);

    if(rand() % 2 == 0){
        out = Aes::aes_128_ECB_en(out, &key[0]);
        //cout << "ECB" << endl;
    }else{
        out = Aes::aes_128_CBC_en(out, &key[0], iv);
        //cout << "CBC" << endl;
    }
    return out;
}

vector<byte> Target::append_and_encrypt(const vector<byte>& data){
    vector<byte> append;
    append = Conversion::b64StringToByteArray(b64append);
    vector<byte> data2 = Block::append_arrays(data, append);
    return (Aes::aes_128_ECB_en(data2, &globKey[0]));
}

vector<byte> Target::prepend_and_encrypt(const vector<byte>& data){
    vector<byte> append = Conversion::b64StringToByteArray(b64append);
    //vector<byte> prepend = Block::gen_random_bytes(-1);

    vector<byte> data2 = Block::append_arrays(data, append);
    data2 = Block::append_arrays(prepend, data2);
    return (Aes::aes_128_ECB_en(data2, &globKey[0]));
}

vector<byte> Target::encrypt_CBC(vector<byte>& data){
    return Aes::aes_128_CBC_en(data, &secretKey[0], globKey);
}

bool Target::padding_oracle(const vector<byte>& v, vector<byte>& iv){
    vector<byte> plain = v;
    plain = Aes::aes_128_CBC_de_NP(plain, &secretKey[0], iv);
    bool ret = remove_padding(plain);
    return ret;
}

vector<byte> Target::getSecretKey(){
    return secretKey;
}

vector<byte> Target::globKey(16, 0);
vector<byte> Target::prepend(16, 0);
