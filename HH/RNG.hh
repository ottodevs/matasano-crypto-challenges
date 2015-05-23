#ifndef RNG_HH
#define RNG_HH

#include "utils.cpp"

class rng {
    /* user identity with various properties */

private:

    int index;
    unsigned int MT[624];

public:

    //create rng from seed
    rng(int seed);

    //create rng from state
    rng(unsigned int* s);

    unsigned int extract_number();

    void generate_numbers();

    void set_index(int i);

    unsigned int* get_state();

};
#endif
