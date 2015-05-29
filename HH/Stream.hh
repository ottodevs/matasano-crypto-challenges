#ifndef STREAM_HH
#define STREAM_HH

#include "utils.cpp"
#include "RNG.hh"
#include "Xor.hh"
#include "Aes.hh"
#include "Block.hh"
#include "Output.hh"

extern vector<byte> mt19937(vector<byte>& plain, int seed);
/* mt19937 Stream cipher */

extern vector<byte> edit_CTR(vector<byte>& cipher, const byte key[], vector<byte>& nonce, int offset, vector<byte>& newtext);
/* edits the cipher text with the newtext in the offset position */

#endif
