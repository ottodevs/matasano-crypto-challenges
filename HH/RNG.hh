#ifndef RNG_HH
#define RNG_HH

#include "utils.cpp"

class rng {
    /* user identity with various properties */

private:

    int index;
    unsigned long int MT[624];

public:

    rng(int seed);

    unsigned int extract_number();

    void generate_numbers();

};
#endif
