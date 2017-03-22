#ifndef STREAM_HH
#define STREAM_HH

#include "utils.cpp"
#include "RNG.hh"
#include "Xor.hh"
#include "Aes.hh"
#include "Block.hh"
#include "Output.hh"
#include "Data.hh"

extern Data mt19937(Data& plain, int seed);
/* mt19937 Stream cipher */

extern Data edit_CTR(Data& cipher, const byte key[], vector<byte>& nonce, int offset, Data& newtext);
/* edits the cipher text with the newtext in the offset position */

#endif
