#ifndef HASH_HH
#define HASH_HH

#include "utils.cpp"
#include "Block.hh"
//#include <openssl/sha.h>

extern vector<byte> sha1_mac(const vector<byte>& prefix, const vector<byte>& message);
	
extern vector<byte> sha1(const vector<byte> message);
	
extern void change_endian(int& n);
extern void change_endian(unsigned int& n);

extern int big_end_from_array(byte* ini);
	
extern int leftrotate(unsigned int n, int r);

#endif