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

// create rng from seed
rng(int seed);

// create rng from state
rng(uint32_t * s);

void seed(int seed);

uint32_t extract_number();

void generate_numbers();

void set_index(int i);

uint32_t * get_state();

};
#endif // ifndef RNG_HH
