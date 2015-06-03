#ifndef HASH_HH
#define HASH_HH

#include "utils.cpp"
#include "Block.hh"
#include <openssl/sha.h>

extern vector<byte> sha1_mac(const vector<byte>& prefix, const vector<byte>& message);

#endif