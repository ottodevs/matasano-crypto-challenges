#include "Target.hh"


Target::Target(){
    secretKey = gen_random_block();
}

Target::~Target(){}

Data Target::rand_encryption(const Data& data){
    vector<byte> iv,key;
    iv = gen_random_block();
    key = gen_random_block();
    time_t secs;
    time(&secs);
    srand((unsigned int) secs);

    //pad(rand)
    int n = rand() % 6;
    n += 5;
    Data out (data.size() + n);
    for(int i = 0; i < n; ++i){
        out[i] = (byte) n;
    }
    for(int i = 0; i < data.size(); ++i){
        out[n + i] = data[i];
    }
    out.pkcs7_pad(data.size() + n);

    if(rand() % 2 == 0){
        out = aes_128_ECB_en(out, &key[0]);
        //cout << "ECB" << endl;
    }else{
        out = aes_128_CBC_en(out, &key[0], iv);
        //cout << "CBC" << endl;
    }
    return out;
}

Data Target::append_and_encrypt(const Data& data){
    vector<byte> append;
    append = b64StringToByteArray(b64append);
    Data data2 = data + append;
    return (aes_128_ECB_en(data2, &globKey[0]));
}

Data Target::prepend_and_encrypt(const Data& data){
    vector<byte> append = b64StringToByteArray(b64append);
    //vector<byte> prepend = gen_random_bytes(-1);

    Data data2 = data + append;
    data2 = Data(prepend) + data2;
    return (aes_128_ECB_en(data2, &globKey[0]));
}

Data Target::encrypt_CBC(const Data& data){
    Data d = aes_128_CBC_en(data, &secretKey[0], globKey);
    //d.toType(0);
    return d;
}

bool Target::padding_oracle(const Data v, vector<byte>& iv){
    Data plain = aes_128_CBC_de(v, &secretKey[0], iv);
    return plain.pkcs7_validate_padding();
}

vector<byte> Target::getSecretKey(){
    return secretKey;
}

vector<byte> Target::globKey(16, 0);
vector<byte> Target::prepend(16, 0);
