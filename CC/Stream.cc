#include "Stream.hh"

Data mt19937(Data& plain, int seed){
    rng r(seed);
    Data out;
    for(int i = 0; i < plain.size(); ++i){
        out.push((byte)(r.extract_number() xor plain[i]));
    }
    return out;
}

Data edit_CTR(Data& cipher, const byte key[], vector<byte>& nonce, int offset, Data& newtext){
    //printHex(cipher);
    Data n;

    int start = (offset/16)*16;
    int end = ((offset + newtext.size() - 1)/16)*16;

    for(int i = start; i <= end; i += 16){
        vector<byte> run = append_arrays(nonce, intToByteArray(i/16, false));
        run = aes_encrypt(&run[0], key);
        n = n ^ block_xor(&run[0], &cipher[i]);
    }
    //printChar(n);
    //cout << endl;

    for(int i = 0; i < newtext.size(); ++i)
        n[offset%16 + i] = newtext[i];

    //printChar(n);
    //cout << endl;

    Data block(0);
    Data ret = cipher;

    for(int i = 0; i <= n.size()-16; i += 16){
        vector<byte> run = append_arrays(nonce, intToByteArray((start + i)/16, false));
        run = aes_encrypt(&run[0], key);
        block = block ^ block_xor(&run[0], &n[i]);
    }
    //printHex(block);
    for(int j = 0; start+j < end+16; ++j)
        ret[start + j] = block[j];
    //printHex(ret);
    //cout << start << endl << end << endl;

    return ret;
}
