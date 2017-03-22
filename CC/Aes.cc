#include "Aes.hh"


vector<byte> aes_encrypt(byte *v, const byte keyS[]){
    AES_KEY key_struct;

    AES_set_encrypt_key(keyS, 128, &key_struct);

    vector<byte> b (16);
    AES_encrypt(v , &b[0], &key_struct);
    return b;
}

vector<byte> aes_decrypt(byte *v, const byte keyS[]){
    AES_KEY key_struct;

    AES_set_decrypt_key(keyS, 128, &key_struct);

    vector<byte> b (16);
    AES_decrypt(v , &b[0], &key_struct);
    return b;
}

Data aes_128_ECB_en(const Data& plain, const byte keyS[]){
    int n = plain.size() % 16;
    n = 16 - n;
    Data b = plain;
    b.pkcs7_pad(plain.size() + n);

    AES_KEY key_struct;
    AES_set_encrypt_key(keyS, 128, &key_struct);

    for(int i = 0; i < b.size(); i += 16){
        AES_encrypt(&b[i] , &b[i], &key_struct);
    }
    return b;
}

Data aes_128_ECB_de(const Data& d, const byte keyS[]){
    AES_KEY key_struct;

    AES_set_decrypt_key(keyS, 128, &key_struct);
    vector<byte> b (d.size());

    vector<byte> v = d.getData();

    for(int i = 0; i < v.size(); i += 16){
        AES_decrypt(&v[i] , &b[i], &key_struct);
    }
    return b;
}

Data aes_128_CBC_en(const Data& plain, const byte keyS[], const vector<byte>& iv){
    Data out = plain;
    vector<byte> carrier;
    int n = plain.size() % 16;
    n = 16 - n;
    out.pkcs7_pad(out.size() + n);

    carrier = iv;
    for(int i = 0; i < out.size(); i += 16){
        carrier = block_xor(&out[i], &carrier[0]);
        carrier = aes_encrypt(&carrier[0], keyS);
        for(int j = 0; j < 16; ++j)
            out[i + j] = carrier[j];
    }
    return out;
}

Data aes_128_CBC_de(const Data& d, const byte keyS[], vector<byte>& iv){
    vector<byte> v = d.getData();
    vector<byte> b;
    vector<byte> out (v.size());
    for(int i = v.size() - 16; i >= 16; i -= 16){
        b = aes_decrypt(&v[i], keyS);
        b = block_xor(&v[i - 16], &b[0]);
        for(int j = 0; j < 16; ++j)
            out[i + j] = b[j];
    }
    b = aes_decrypt(&v[0], keyS);
    b = block_xor(&iv[0], &b[0]);
    for(int i = 0; i < 16; ++i)
        out[i] = b[i];
    return Data(out);
}

Data aes_128_CTR(const Data& d, const byte keyS[], const vector<byte>& nonce){
    vector<byte> v = d.getData();
    vector<byte> ret(v.size());
    uint64_t counter = 0;
    uint64_t size = (uint64_t)v.size()/16;
    if(v.size()%16 != 0) size++;
    cout << "h" << endl;
    for (counter = 0; counter < size; ++counter){
        vector<byte> run = append_arrays(nonce, intToByteArray(counter, false));
        run = aes_encrypt(&run[0], keyS);
        for(int i = 0; i < 16; ++i){
            if(counter*16 + i < ret.size())
                ret[counter*16 + i] = run[i] xor v[counter*16 + i];
        }
    }
    cout << "e" << endl;
    return Data(ret);
}
