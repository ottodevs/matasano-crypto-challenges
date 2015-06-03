#include "Hash.hh"

vector<byte> sha1_mac(const vector<byte>& prefix, const vector<byte>& message){
	vector<byte> v = append_arrays(prefix, message);
	
	size_t length = v.size();

	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(&v[0], length, hash);
	
	return vector<byte>(hash, hash + SHA_DIGEST_LENGTH);
}