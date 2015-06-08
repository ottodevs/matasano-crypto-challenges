#include "Hash.hh"

vector<byte> sha1_mac(const vector<byte>& prefix, const vector<byte>& message){
	vector<byte> v = append_arrays(prefix, message);
	return sha1(v);
}

vector<byte> sha1(vector<byte> message){
	unsigned int h0 = 0x67452301;
	unsigned int h1 = 0xEFCDAB89;
	unsigned int h2 = 0x98BADCFE;
	unsigned int h3 = 0x10325476;
	unsigned int h4 = 0xC3D2E1F0;
	
	unsigned long int ml = message.size()*8;
	
	//pre-processing
	message.push_back(0x80);
	while((message.size()*8)%512 != 448)
		message.push_back(0x00);
	byte* p = (byte*)&ml + 7;
	for(int i = 0; i < 8; ++i)
		message.push_back(*(p - i));
	
	//process
	vector< vector<byte> > chunks;
	for(int i = 0; i < message.size(); i += 64)
		chunks.push_back(copyFrom(&message[i], 64));
	for(int n = 0; n < chunks.size(); ++n){ //chunks[n]
		//16 32bit words w[i]
		vector<unsigned int> w (80);
		for(int i = 0; i < 16; ++i){
			int aux = big_end_from_array(&chunks[n][i*4]);
			change_endian(aux);
			w[i] = aux;
		}
		for(int i = 16; i < 80; ++i)
			w[i] = leftrotate((unsigned int)(w[i-3] xor w[i-8] xor w[i-14] xor w[i-16]), 1);
		
		unsigned int a = h0;
	    unsigned int b = h1;
	    unsigned int c = h2;
	    unsigned int d = h3;
	    unsigned int e = h4;
		
		//main loop
		for(int i = 0; i < 80; ++i){
			unsigned int f, k;
			if (0 <= i and i <= 19){
	            f = (b & c) | ((b xor 0xFFFFFFFF) & d);
	            k = 0x5A827999;
			}
	        else if (20 <= i and i <= 39){
	            f = (b xor c) xor d;
	            k = 0x6ED9EBA1;
			}
	        else if (40 <= i and i <= 59){
	            f = (b & c) | (b & d) | (c & d);
	            k = 0x8F1BBCDC;
			}
	        else if (60 <= i <= 79){
	            f = (b xor c) xor d;
	            k = 0xCA62C1D6;
			}
			
			unsigned int temp = leftrotate((unsigned int)a, 5) + f + e + k + w[i];
	        e = d;
	        d = c;
	        c = leftrotate((unsigned int)b, 30);
	        b = a;
	        a = temp;
		}
		h0 = h0 + a;
	    h1 = h1 + b;
	    h2 = h2 + c;
	    h3 = h3 + d;
	    h4 = h4 + e;
	}
	//hh = (h0 leftshift 128) or (h1 leftshift 96) or (h2 leftshift 64) or (h3 leftshift 32) or h4
	vector<byte> hh (20);
	change_endian(h0);
	p = (byte*)&h0;
	for(int i = 0; i < 4; ++i)
		hh[i] = *(p + i);
	change_endian(h1);
	p = (byte*)&h1;
	for(int i = 0; i < 4; ++i)
		hh[i + 4] = *(p + i);
	change_endian(h2);
	p = (byte*)&h2;
	for(int i = 0; i < 4; ++i)
		hh[i + 8] = *(p + i);
	change_endian(h3);
	p = (byte*)&h3;
	for(int i = 0; i < 4; ++i)
		hh[i + 12] = *(p + i);
	change_endian(h4);
	p = (byte*)&h4;
	for(int i = 0; i < 4; ++i)
		hh[i + 16] = *(p + i);
	return hh;
}

vector<byte> sha1(vector<byte> message, unsigned int h0, unsigned int h1, unsigned int h2, unsigned int h3, unsigned int h4, unsigned long int ml){
	
	//pre-processing
	message.push_back(0x80);
	while((message.size()*8)%512 != 448)
		message.push_back(0x00);
	byte* p = (byte*)&ml + 7;
	for(int i = 0; i < 8; ++i)
		message.push_back(*(p - i));
	
	//process
	vector< vector<byte> > chunks;
	for(int i = 0; i < message.size(); i += 64)
		chunks.push_back(copyFrom(&message[i], 64));
	for(int n = 0; n < chunks.size(); ++n){ //chunks[n]
		//16 32bit words w[i]
		vector<unsigned int> w (80);
		for(int i = 0; i < 16; ++i){
			int aux = big_end_from_array(&chunks[n][i*4]);
			change_endian(aux);
			w[i] = aux;
		}
		for(int i = 16; i < 80; ++i)
			w[i] = leftrotate((unsigned int)(w[i-3] xor w[i-8] xor w[i-14] xor w[i-16]), 1);
		
		unsigned int a = h0;
	    unsigned int b = h1;
	    unsigned int c = h2;
	    unsigned int d = h3;
	    unsigned int e = h4;
		
		//main loop
		for(int i = 0; i < 80; ++i){
			unsigned int f, k;
			if (0 <= i and i <= 19){
	            f = (b & c) | ((b xor 0xFFFFFFFF) & d);
	            k = 0x5A827999;
			}
	        else if (20 <= i and i <= 39){
	            f = (b xor c) xor d;
	            k = 0x6ED9EBA1;
			}
	        else if (40 <= i and i <= 59){
	            f = (b & c) | (b & d) | (c & d);
	            k = 0x8F1BBCDC;
			}
	        else if (60 <= i <= 79){
	            f = (b xor c) xor d;
	            k = 0xCA62C1D6;
			}
			
			unsigned int temp = leftrotate((unsigned int)a, 5) + f + e + k + w[i];
	        e = d;
	        d = c;
	        c = leftrotate((unsigned int)b, 30);
	        b = a;
	        a = temp;
		}
		h0 = h0 + a;
	    h1 = h1 + b;
	    h2 = h2 + c;
	    h3 = h3 + d;
	    h4 = h4 + e;
	}
	//hh = (h0 leftshift 128) or (h1 leftshift 96) or (h2 leftshift 64) or (h3 leftshift 32) or h4
	vector<byte> hh (20);
	change_endian(h0);
	p = (byte*)&h0;
	for(int i = 0; i < 4; ++i)
		hh[i] = *(p + i);
	change_endian(h1);
	p = (byte*)&h1;
	for(int i = 0; i < 4; ++i)
		hh[i + 4] = *(p + i);
	change_endian(h2);
	p = (byte*)&h2;
	for(int i = 0; i < 4; ++i)
		hh[i + 8] = *(p + i);
	change_endian(h3);
	p = (byte*)&h3;
	for(int i = 0; i < 4; ++i)
		hh[i + 12] = *(p + i);
	change_endian(h4);
	p = (byte*)&h4;
	for(int i = 0; i < 4; ++i)
		hh[i + 16] = *(p + i);
	return hh;
}

void change_endian(int& n){
	byte *b = (byte*)&n;
	byte b1 = *b;
	*b = *(b + 3);
	*(b + 3) = b1;
	b1 = *(b + 1);
	*(b + 1) = *(b + 2);
	*(b + 2) = b1;
}

void change_endian(unsigned int& n){
	byte *b = (byte*)&n;
	byte b1 = *b;
	*b = *(b + 3);
	*(b + 3) = b1;
	b1 = *(b + 1);
	*(b + 1) = *(b + 2);
	*(b + 2) = b1;
}

int big_end_from_array(byte* ini){
	int a;
	byte* p = (byte*)&a;
	for(int i = 0; i < 4; ++i, ++p, ++ini)
		*p = *ini;
	return a;
}

int leftrotate(unsigned int n, int r){
	if(r == 1){
		if(n >> 31)
			return (n << 1) + 1;
		return n << 1;
	}
	for(int i = 0; i < r; ++i)
		n = leftrotate(n, 1);
	return n;
}

vector<byte> sha1_padding(int bitlength){
	unsigned long int ml = (unsigned long int)bitlength;
	vector<byte> padding;
	padding.push_back(0x80);
	while(bitlength%512 != 448){
		padding.push_back(0x00);
		bitlength += 8;
	}
	byte* p = (byte*)&ml + 7;
	for(int i = 0; i < 8; ++i)
		padding.push_back(*(p - i));
}

vector<byte> sha1_glue_padding(int key_length, int message_length){
	vector<byte> padding;
	padding.push_back(0x80);
	while(((padding.size() + message_length + key_length)*8)%512 != 448)
		padding.push_back(0x00);
	unsigned long int ml = (key_length + message_length)*8;
	byte* p = (byte*)&ml + 7;
	for(int i = 0; i < 8; ++i)
		padding.push_back(*(p - i));
	return padding;
}
