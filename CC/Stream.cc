#include "Stream.hh"

Data mt19937(Data& plain, int seed){
    rng r(seed);
    Data out;

    for (int i = 0; i < plain.size(); ++i) {
        out.push((byte)(r.extract_number() xor plain[i]));
    }
    return out;
}

Data edit_CTR(Data& cipher, const byte key[], vector<byte>& nonce, int offset, Data& newtext){
    Data n(0);

    int start = (offset / 16) * 16;
    int end = ((offset + newtext.size() - 1) / 16) * 16;

    // cout << "offset-size: " << offset << "-" << newtext.size() << endl;
    // cout << "start-end: " << start << "-" << end << endl;

    for (int i = start; i <= end; i += 16) {
        // 1. generate encrypted running counter
        vector<byte> run = append_arrays(nonce, intToByteArray(i / 16, false));
        run = aes_encrypt(&run[0], key);
        // 2. get plain block
        n = n + block_xor(&run[0], &cipher[i]);
    }

    for (int i = 0; i < newtext.size(); ++i) {
        n[offset % 16 + i] = newtext[i];
    }

    Data encrypted(0);
    Data ret = cipher;

    for (int i = 0; i <= n.size() - 16; i += 16) {
        vector<byte> run = append_arrays(nonce, intToByteArray((start + i) / 16, false));
        run = aes_encrypt(&run[0], key);
        encrypted = encrypted + block_xor(&run[0], &n[i]);
    }
    for (int j = 0; j < encrypted.size(); ++j) {
        ret[start + j] = encrypted[j];
    }

    return ret;
} // edit_CTR
