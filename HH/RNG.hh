#ifndef RNG_HH
#define RNG_HH

#include "utils.cpp"

class rng {
    /* Implementation of the mt19937 mersenne twister RNG */

private:

    int index;
    unsigned int MT[624];

public:

    rng();

    //create rng from seed
    rng(int seed);

    //create rng from state
    rng(unsigned int* s);

    void seed(int seed);

    unsigned int extract_number();

    void generate_numbers();

    void set_index(int i);

    unsigned int* get_state();

};
#endif
