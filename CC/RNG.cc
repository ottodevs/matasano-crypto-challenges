#include "RNG.hh"

rng::rng(){
    index = 0;
}

rng::rng(int seed){
    index = 0;
    this->seed(seed);
}

rng::rng(uint32_t * s){
    index = 0;

    for (int i = 0; i < 624; ++i, ++s) {
        MT[i] = *s;
    }
}

void rng::seed(int seed){
    MT[0] = seed;
    for (int i = 1; i < 624; ++i) {
        uint64_t a = 1812433253;
        a = a * (MT[i - 1] xor (MT[i - 1] >> 30)) + i;
        a = (a << 32) >> 32;
        MT[i] = a;
    }
    index = 0;
}

uint32_t rng::extract_number(){
    if (index == 0) generate_numbers();

    uint64_t y = MT[index];
    y = y xor (y >> 11);
    y = y xor ((y << 7) bitand (0x9d2c5680));
    y = y xor ((y << 15) bitand (0xefc60000));
    y = y xor (y >> 18);

    index = (index + 1) % 624;
    return y;
}

void rng::generate_numbers(){
    for (int i = 0; i < 624; ++i) {
        uint64_t y = (MT[i] bitand 0x80000000) + (MT[(i + 1) % 624] bitand 0x7fffffff);
        MT[i] = MT[(i + 397) % 624] xor (y >> 1);
        if (y % 2 != 0) MT[i] = MT[i] xor 0x9908b0df;
    }
}

void rng::set_index(int i){
    index = i;
}

uint32_t * rng::get_state(){
    return &MT[0];
}
