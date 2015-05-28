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

vector<byte> aes_128_ECB_en(const vector<byte>& plainData, const byte keyS[]){
    int n = plainData.size() % 16;
    n = 16 - n;

    vector<byte> b = pkcs7_pad(plainData, plainData.size() + n);

    AES_KEY key_struct;
    AES_set_encrypt_key(keyS, 128, &key_struct);

    for(int i = 0; i < b.size(); i += 16){
        AES_encrypt(&b[i] , &b[i], &key_struct);
    }
    return b;
}

vector<byte> aes_128_ECB_de(const vector<byte>& v, const byte keyS[]){
    AES_KEY key_struct;

    AES_set_decrypt_key(keyS, 128, &key_struct);

    vector<byte> b (v.size());

    for(int i = 0; i < v.size(); i += 16){
        AES_decrypt(&v[i] , &b[i], &key_struct);
    }
    remove_padding(b);
    return b;
}

vector<byte> aes_128_CBC_en(vector<byte>& plainData, const byte keyS[], vector<byte>& iv){
    //cout << "start encryption" << endl;
    vector<byte> carrier;
    int n = plainData.size() % 16;
    n = 16 - n;
    vector<byte> out = pkcs7_pad(plainData, plainData.size() + n);

    carrier = iv;
    for(int i = 0; i < out.size(); i += 16){
        carrier = block_xor(&out[i], &carrier[0]);
        carrier = aes_encrypt(&carrier[0], keyS);
        for(int j = 0; j < 16; ++j)
            out[i + j] = carrier[j];
    }
    return out;
}

vector<byte> aes_128_CBC_de(vector<byte>& v, const byte keyS[], vector<byte>& iv){
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
    remove_padding(out);
    return out;
}

vector<byte> aes_128_CBC_de_NP(vector<byte>& v, const byte keyS[], vector<byte>& iv){
    vector<byte> b;
    vector<byte> out (v.size());
    for(int i = v.size() - 16; i >= 16; i -= 16){
        b = aes_decrypt(&v[i], keyS);
        b = block_xor(&v[i - 16], &b[0]);
        for(int j = 0; j < 16; ++j)
            out[i + j] = b[j];
    }
    b = aes_decrypt(&v[0], keyS);
    //I
    b = block_xor(&iv[0], &b[0]);
    if(v.size() == 16)
        return b;
    for(int i = 0; i < 16; ++i)
        out[i] = b[i];
    return out;
}

vector<byte> aes_128_CTR(const vector<byte>& v, const byte keyS[], const vector<byte>& nonce){
    vector<byte> ret(v.size());
    unsigned long int counter = 0;
    unsigned long int size = (unsigned long int)v.size()/16;
    if(v.size()%16 != 0) size++;
    for (counter = 0; counter < size; ++counter){
        vector<byte> run = append_arrays(nonce, intToByteArray(counter, false));
        run = aes_encrypt(&run[0], keyS);
        for(int i = 0; i < 16; ++i){
            if(counter*16 + i < ret.size())
                ret[counter*16 + i] = run[i] xor v[counter*16 + i];
        }
    }
    return ret;
}






