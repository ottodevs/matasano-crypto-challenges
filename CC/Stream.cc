#include "Stream.hh"

vector<byte> mt19937(vector<byte>& plain, int seed){
    rng r(seed);
    vector<byte> out;
    for(int i = 0; i < plain.size(); ++i){
        out.push_back((byte)(r.extract_number() xor plain[i]));
    }
    return out;
}
