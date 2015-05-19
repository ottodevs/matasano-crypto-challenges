#include "Xor.hh"


vector<byte> Xor::fixed_xor(const vector<byte>& data1, const vector<byte>& data2){
    //should check if sizes are equal and throw exception
    if(data1.size() != data2.size()){
        cout << "operands are not the same size" << endl;
        return data1;
    }
    vector<byte> out (data1.size(), 0);
    for(int i = 0; i < data1.size(); ++i){
        out[i] = data1[i] xor data2[i];
    }
    return out;
}

vector<byte> Xor::single_key_xor(const vector<byte>& buffer1, byte k){
    vector<byte> out (buffer1.size(), 0);
    for (int i = 0; i < buffer1.size(); ++i){
        out[i] = buffer1[i] xor k;
    }    
    return out;
}

vector<byte> Xor::repeating_key_xor(const vector<byte>& in, string key){
    vector<byte> out (in.size());
    vector<byte> k;
    k = Conversion::stringToByteArray(key);
    int p = 0;
    for(int i = 0; i < in.size(); ++i){
        out[i] = in[i] xor k[p];
        p++;
        if(p >= key.size())
            p = 0;
    }
    return out;
}

vector<byte> Xor::block_xor(byte *v, byte *b){
    vector<byte> x (16);
    vector<byte> y (16);
    for(int i = 0; i < 16; ++i){
        x[i] = *v;
        y[i] = *b;
        ++v;
        ++b;
    }
    x = fixed_xor(x, y);
    return x;
}











