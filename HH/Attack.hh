#ifndef ATTACK_HH
#define ATTACK_HH

#include "utils.cpp"
#include "Block.hh"
#include "Analysis.hh"
#include "Output.hh"
#include "Xor.hh"
#include "Target.hh"
#include "RNG.hh"
#include <ctime>


/*
 * Tools for attacking and decrypting crypto
 */

extern vector<byte> break_block(const vector<byte>& b1, const vector<byte>& b2, Target tgt);

extern byte testBytes(const vector<byte> v, int thres);
    /* Tries all 256 bytes as single-XOR keys and prints strings with score >= thres (use const THRES in utils):
     * (score)String [key]
     * returns best key
     */

extern vector<byte> findRepeatingKey(const vector< vector<byte> >& groupedBlock); //analyses a grouped block byte by byte to find the repeating key

extern void byte_at_a_time(vector<byte> (*f)(const vector<byte>&));

extern vector<byte> padding_oracle_attack(const vector<byte>& c, const vector<byte> iv, Target tgt);
    /* returns unencrypted vector */

extern unsigned int mt19937_untemper(unsigned int n);

extern bool is_time_seeded(unsigned int n, int threshold);

#endif
