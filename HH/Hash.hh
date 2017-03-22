#ifndef HASH_HH
#define HASH_HH

#include "utils.cpp"
#include "Block.hh"
#include "Data.hh"
#include "Output.hh"
//#include <openssl/sha.h>

extern vector<byte> sha1_mac(const vector<byte>& prefix, const vector<byte>& message);
extern vector<byte> md4_mac(const vector<byte>& prefix, const vector<byte>& message);

extern vector<byte> sha1(vector<byte> message);
extern vector<byte> sha1(vector<byte> message, uint32_t h0, uint32_t h1, uint32_t h2, uint32_t h3, uint32_t h4, uint64_t ml);

extern vector<byte> md4(vector<byte> message);
extern vector<byte> md4(vector<byte> message, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint64_t size);

extern uint32_t md4_op1(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t xk, int s);
extern uint32_t md4_op2(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t xk, int s);
extern uint32_t md4_op3(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t xk, int s);
extern uint32_t md4_f(uint32_t x, uint32_t y, uint32_t z);
extern uint32_t md4_g(uint32_t x, uint32_t y, uint32_t z);
extern uint32_t md4_h(uint32_t x, uint32_t y, uint32_t z);

extern void change_endian(uint32_t& n);

extern uint32_t uint32_from_array(byte* ini);
extern uint32_t int_from_array(byte* ini);

extern uint32_t leftrotate(uint32_t n, int r);

extern vector<byte> sha1_glue_padding(int key_length, int message_length);
extern vector<byte> md4_glue_padding(int key_length, int message_length);

#endif
