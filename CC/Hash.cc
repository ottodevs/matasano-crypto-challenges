#include "Hash.hh"

vector<byte> sha1_mac(const vector<byte>& prefix, const vector<byte>& message){
    vector<byte> v = append_arrays(prefix, message);
    return sha1(v);
}

vector<byte> md4_mac(const vector<byte>& prefix, const vector<byte>& message){
    vector<byte> v = append_arrays(prefix, message);
    return md4(v);
}

vector<byte> sha1(vector<byte> message){
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    uint64_t ml = message.size() * 8;

    // pre-processing
    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448)
        message.push_back(0x00);
    byte * p = (byte *)&ml + 7;
    for (int i = 0; i < 8; ++i) {
        message.push_back(*(p - i));
    }

    // process
    vector< vector<byte> > chunks;
    for (int i = 0; i < message.size(); i += 64) {
        chunks.push_back(copyFrom(&message[i], 64).getData());
    }
    for (int n = 0; n < chunks.size(); ++n) {   // chunks[n]
        // 16 32bit words w[i]
        vector<uint32_t> w(80);
        for (int i = 0; i < 16; ++i) {
            uint32_t aux = int_from_array(&chunks[n][i * 4]);
            change_endian(aux);
            w[i] = aux;
        }
        for (int i = 16; i < 80; ++i) {
            w[i] = leftrotate((uint32_t)(w[i - 3] xor w[i - 8] xor w[i - 14] xor w[i - 16]), 1);
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        // main loop
        for (int i = 0; i < 80; ++i) {
            uint32_t f, k;
            if (0 <= i and i <= 19) {
                f = (b & c) | ((b xor 0xFFFFFFFF) & d);
                k = 0x5A827999;
            } else if (20 <= i and i <= 39) {
                f = (b xor c) xor d;
                k = 0x6ED9EBA1;
            } else if (40 <= i and i <= 59) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else if (60 <= i <= 79) {
                f = (b xor c) xor d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = leftrotate((uint32_t)a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = leftrotate((uint32_t)b, 30);
            b = a;
            a = temp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }
    // hh = (h0 leftshift 128) or (h1 leftshift 96) or (h2 leftshift 64) or (h3 leftshift 32) or h4
    vector<byte> hh(20);
    change_endian(h0);
    p = (byte *)&h0;
    for (int i = 0; i < 4; ++i) {
        hh[i] = *(p + i);
    }
    change_endian(h1);
    p = (byte *)&h1;
    for (int i = 0; i < 4; ++i) {
        hh[i + 4] = *(p + i);
    }
    change_endian(h2);
    p = (byte *)&h2;
    for (int i = 0; i < 4; ++i) {
        hh[i + 8] = *(p + i);
    }
    change_endian(h3);
    p = (byte *)&h3;
    for (int i = 0; i < 4; ++i) {
        hh[i + 12] = *(p + i);
    }
    change_endian(h4);
    p = (byte *)&h4;
    for (int i = 0; i < 4; ++i) {
        hh[i + 16] = *(p + i);
    }
    return hh;
} // sha1

vector<byte> sha1(vector<byte> message, uint32_t h0, uint32_t h1, uint32_t h2, uint32_t h3, uint32_t h4, uint64_t ml){

    // pre-processing
    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448)
        message.push_back(0x00);
    byte * p = (byte *)&ml + 7;
    for (int i = 0; i < 8; ++i) {
        message.push_back(*(p - i));
    }

    // process
    vector< vector<byte> > chunks;
    for (int i = 0; i < message.size(); i += 64) {
        chunks.push_back(copyFrom(&message[i], 64).getData());
    }
    for (int n = 0; n < chunks.size(); ++n) {   // chunks[n]
        // 16 32bit words w[i]
        vector<uint32_t> w(80);
        for (int i = 0; i < 16; ++i) {
            w[i] = uint32_from_array(&chunks[n][i * 4]);
        }
        for (int i = 16; i < 80; ++i) {
            w[i] = leftrotate((uint32_t)(w[i - 3] xor w[i - 8] xor w[i - 14] xor w[i - 16]), 1);
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        // main loop
        for (int i = 0; i < 80; ++i) {
            uint32_t f, k;
            if (0 <= i and i <= 19) {
                f = (b & c) | ((b xor 0xFFFFFFFF) & d);
                k = 0x5A827999;
            } else if (20 <= i and i <= 39) {
                f = (b xor c) xor d;
                k = 0x6ED9EBA1;
            } else if (40 <= i and i <= 59) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else if (60 <= i <= 79) {
                f = (b xor c) xor d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = leftrotate((uint32_t)a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = leftrotate((uint32_t)b, 30);
            b = a;
            a = temp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }
    // hh = (h0 leftshift 128) or (h1 leftshift 96) or (h2 leftshift 64) or (h3 leftshift 32) or h4
    vector<byte> hh(20);
    change_endian(h0);
    p = (byte *)&h0;
    for (int i = 0; i < 4; ++i) {
        hh[i] = *(p + i);
    }
    change_endian(h1);
    p = (byte *)&h1;
    for (int i = 0; i < 4; ++i) {
        hh[i + 4] = *(p + i);
    }
    change_endian(h2);
    p = (byte *)&h2;
    for (int i = 0; i < 4; ++i) {
        hh[i + 8] = *(p + i);
    }
    change_endian(h3);
    p = (byte *)&h3;
    for (int i = 0; i < 4; ++i) {
        hh[i + 12] = *(p + i);
    }
    change_endian(h4);
    p = (byte *)&h4;
    for (int i = 0; i < 4; ++i) {
        hh[i + 16] = *(p + i);
    }
    return hh;
} // sha1

vector<byte> md4(vector<byte> message){
    /*
     * From the md4 description in:
     * www.faqs.org/rfcs/rfc1320.html
     */

    uint64_t size = message.size() * 8;

    // 1. Append Padding
    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448) message.push_back(0x00);
    // 2. Append Length (not managing messages longer than 2^64)
    byte * p = (byte *)&size;
    for (int i = 0; i < 8; ++i) {
        message.push_back(*(p + i));
    }
    // 3. Initialize MD buffer
    uint32_t a = 0x67452301;
    uint32_t b = 0xefcdab89;
    uint32_t c = 0x98badcfe;
    uint32_t d = 0x10325476;
    // 4. Process Message in 16-Word Blocks
    vector< vector<byte> > blocks;
    for (int i = 0; i < message.size(); i += 64) {
        blocks.push_back(copyFrom(&message[i], 64).getData());
    }
    for (int n = 0; n < blocks.size(); ++n) {
        uint32_t X[16];
        for (int i = 0; i < 16; i++) {
            X[i] = int_from_array(&blocks[n][i * 4]);
        }
        uint32_t aa = a;
        uint32_t bb = b;
        uint32_t cc = c;
        uint32_t dd = d;

        // ROUND 1
        a = md4_op1(a, b, c, d, X[0], 3);
        d = md4_op1(d, a, b, c, X[1], 7);
        c = md4_op1(c, d, a, b, X[2], 11);
        b = md4_op1(b, c, d, a, X[3], 19);
        a = md4_op1(a, b, c, d, X[4], 3);
        d = md4_op1(d, a, b, c, X[5], 7);
        c = md4_op1(c, d, a, b, X[6], 11);
        b = md4_op1(b, c, d, a, X[7], 19);
        a = md4_op1(a, b, c, d, X[8], 3);
        d = md4_op1(d, a, b, c, X[9], 7);
        c = md4_op1(c, d, a, b, X[10], 11);
        b = md4_op1(b, c, d, a, X[11], 19);
        a = md4_op1(a, b, c, d, X[12], 3);
        d = md4_op1(d, a, b, c, X[13], 7);
        c = md4_op1(c, d, a, b, X[14], 11);
        b = md4_op1(b, c, d, a, X[15], 19);
        // ROUND 2
        a = md4_op2(a, b, c, d, X[0], 3);
        d = md4_op2(d, a, b, c, X[4], 5);
        c = md4_op2(c, d, a, b, X[8], 9);
        b = md4_op2(b, c, d, a, X[12], 13);
        a = md4_op2(a, b, c, d, X[1], 3);
        d = md4_op2(d, a, b, c, X[5], 5);
        c = md4_op2(c, d, a, b, X[9], 9);
        b = md4_op2(b, c, d, a, X[13], 13);
        a = md4_op2(a, b, c, d, X[2], 3);
        d = md4_op2(d, a, b, c, X[6], 5);
        c = md4_op2(c, d, a, b, X[10], 9);
        b = md4_op2(b, c, d, a, X[14], 13);
        a = md4_op2(a, b, c, d, X[3], 3);
        d = md4_op2(d, a, b, c, X[7], 5);
        c = md4_op2(c, d, a, b, X[11], 9);
        b = md4_op2(b, c, d, a, X[15], 13);
        // ROUND 3
        a = md4_op3(a, b, c, d, X[0], 3);
        d = md4_op3(d, a, b, c, X[8], 9);
        c = md4_op3(c, d, a, b, X[4], 11);
        b = md4_op3(b, c, d, a, X[12], 15);
        a = md4_op3(a, b, c, d, X[2], 3);
        d = md4_op3(d, a, b, c, X[10], 9);
        c = md4_op3(c, d, a, b, X[6], 11);
        b = md4_op3(b, c, d, a, X[14], 15);
        a = md4_op3(a, b, c, d, X[1], 3);
        d = md4_op3(d, a, b, c, X[9], 9);
        c = md4_op3(c, d, a, b, X[5], 11);
        b = md4_op3(b, c, d, a, X[13], 15);
        a = md4_op3(a, b, c, d, X[3], 3);
        d = md4_op3(d, a, b, c, X[11], 9);
        c = md4_op3(c, d, a, b, X[7], 11);
        b = md4_op3(b, c, d, a, X[15], 15);

        a += aa;
        b += bb;
        c += cc;
        d += dd;
    }
    // 5. Output
    vector<byte> out(16);
    p = (byte *)&a;
    for (int i = 0; i < 4; ++i) {
        out[i] = *(p + i);
    }
    p = (byte *)&b;
    for (int i = 0; i < 4; ++i) {
        out[i + 4] = *(p + i);
    }
    p = (byte *)&c;
    for (int i = 0; i < 4; ++i) {
        out[i + 8] = *(p + i);
    }
    p = (byte *)&d;
    for (int i = 0; i < 4; ++i) {
        out[i + 12] = *(p + i);
    }
    return out;
} // md4

vector<byte> md4(vector<byte> message, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint64_t size){
    // 1. Append Padding
    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448) message.push_back(0x00);
    // 2. Append Length (not managing messages longer than 2^64)
    byte * p = (byte *)&size;
    for (int i = 0; i < 8; ++i) {
        message.push_back(*(p + i));
    }
    // 4. Process Message in 16-Word Blocks
    vector< vector<byte> > blocks;
    for (int i = 0; i < message.size(); i += 64) {
        blocks.push_back(copyFrom(&message[i], 64).getData());
    }
    for (int n = 0; n < blocks.size(); ++n) {
        uint32_t X[16];
        for (int i = 0; i < 16; i++) {
            X[i] = int_from_array(&blocks[n][i * 4]);
        }
        uint32_t aa = a;
        uint32_t bb = b;
        uint32_t cc = c;
        uint32_t dd = d;

        // ROUND 1
        a = md4_op1(a, b, c, d, X[0], 3);
        d = md4_op1(d, a, b, c, X[1], 7);
        c = md4_op1(c, d, a, b, X[2], 11);
        b = md4_op1(b, c, d, a, X[3], 19);
        a = md4_op1(a, b, c, d, X[4], 3);
        d = md4_op1(d, a, b, c, X[5], 7);
        c = md4_op1(c, d, a, b, X[6], 11);
        b = md4_op1(b, c, d, a, X[7], 19);
        a = md4_op1(a, b, c, d, X[8], 3);
        d = md4_op1(d, a, b, c, X[9], 7);
        c = md4_op1(c, d, a, b, X[10], 11);
        b = md4_op1(b, c, d, a, X[11], 19);
        a = md4_op1(a, b, c, d, X[12], 3);
        d = md4_op1(d, a, b, c, X[13], 7);
        c = md4_op1(c, d, a, b, X[14], 11);
        b = md4_op1(b, c, d, a, X[15], 19);
        // ROUND 2
        a = md4_op2(a, b, c, d, X[0], 3);
        d = md4_op2(d, a, b, c, X[4], 5);
        c = md4_op2(c, d, a, b, X[8], 9);
        b = md4_op2(b, c, d, a, X[12], 13);
        a = md4_op2(a, b, c, d, X[1], 3);
        d = md4_op2(d, a, b, c, X[5], 5);
        c = md4_op2(c, d, a, b, X[9], 9);
        b = md4_op2(b, c, d, a, X[13], 13);
        a = md4_op2(a, b, c, d, X[2], 3);
        d = md4_op2(d, a, b, c, X[6], 5);
        c = md4_op2(c, d, a, b, X[10], 9);
        b = md4_op2(b, c, d, a, X[14], 13);
        a = md4_op2(a, b, c, d, X[3], 3);
        d = md4_op2(d, a, b, c, X[7], 5);
        c = md4_op2(c, d, a, b, X[11], 9);
        b = md4_op2(b, c, d, a, X[15], 13);
        // ROUND 3
        a = md4_op3(a, b, c, d, X[0], 3);
        d = md4_op3(d, a, b, c, X[8], 9);
        c = md4_op3(c, d, a, b, X[4], 11);
        b = md4_op3(b, c, d, a, X[12], 15);
        a = md4_op3(a, b, c, d, X[2], 3);
        d = md4_op3(d, a, b, c, X[10], 9);
        c = md4_op3(c, d, a, b, X[6], 11);
        b = md4_op3(b, c, d, a, X[14], 15);
        a = md4_op3(a, b, c, d, X[1], 3);
        d = md4_op3(d, a, b, c, X[9], 9);
        c = md4_op3(c, d, a, b, X[5], 11);
        b = md4_op3(b, c, d, a, X[13], 15);
        a = md4_op3(a, b, c, d, X[3], 3);
        d = md4_op3(d, a, b, c, X[11], 9);
        c = md4_op3(c, d, a, b, X[7], 11);
        b = md4_op3(b, c, d, a, X[15], 15);

        a += aa;
        b += bb;
        c += cc;
        d += dd;
    }
    // 5. Output
    vector<byte> out(16);
    p = (byte *)&a;
    for (int i = 0; i < 4; ++i) {
        out[i] = *(p + i);
    }
    p = (byte *)&b;
    for (int i = 0; i < 4; ++i) {
        out[i + 4] = *(p + i);
    }
    p = (byte *)&c;
    for (int i = 0; i < 4; ++i) {
        out[i + 8] = *(p + i);
    }
    p = (byte *)&d;
    for (int i = 0; i < 4; ++i) {
        out[i + 12] = *(p + i);
    }
    return out;
} // md4

uint32_t md4_op1(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t xk, int s){
    return leftrotate((a + md4_f(b, c, d) + xk), s);
}

uint32_t md4_op2(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t xk, int s){
    return leftrotate((a + md4_g(b, c, d) + xk + 0x5A827999), s);
}

uint32_t md4_op3(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t xk, int s){
    return leftrotate((a + md4_h(b, c, d) + xk + 0x6ED9EBA1), s);
}

uint32_t md4_f(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) | ((~x) & z);
}

uint32_t md4_g(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) | (x & z) | (y & z);
}

uint32_t md4_h(uint32_t x, uint32_t y, uint32_t z){
    return x xor y xor z;
}

void change_endian(uint32_t& n){
    byte * b = (byte *)&n;
    byte b1 = *b;

    *b = *(b + 3);
    *(b + 3) = b1;
    b1 = *(b + 1);
    *(b + 1) = *(b + 2);
    *(b + 2) = b1;
}

uint32_t uint32_from_array(byte * ini){
    uint32_t a;
    byte * p = (byte *)&a;

    p += 3;
    for (int i = 0; i < 4; ++i, --p, ++ini) {
        *p = *ini;
    }
    return a;
}

uint32_t int_from_array(byte * ini){
    uint32_t a;
    byte * p = (byte *)&a;

    for (int i = 0; i < 4; ++i, ++p, ++ini) {
        *p = *ini;
    }
    return a;
}

uint32_t leftrotate(uint32_t n, int r){
    if (r == 1) {
        if (n >> 31)
            return (n << 1) + 1;
        return n << 1;
    }
    for (int i = 0; i < r; ++i) {
        n = leftrotate(n, 1);
    }
    return n;
}

vector<byte> sha1_padding(int bitlength){
    uint64_t ml = (uint64_t)bitlength;

    vector<byte> padding;
    padding.push_back(0x80);
    while (bitlength % 512 != 448) {
        padding.push_back(0x00);
        bitlength += 8;
    }
    byte * p = (byte *)&ml + 7;
    for (int i = 0; i < 8; ++i) {
        padding.push_back(*(p - i));
    }
}

vector<byte> sha1_glue_padding(int key_length, int message_length){
    vector<byte> padding;
    padding.push_back(0x80);
    while (((padding.size() + message_length + key_length) * 8) % 512 != 448)
        padding.push_back(0x00);
    uint64_t ml = (key_length + message_length) * 8;
    byte * p = (byte *)&ml + 7;
    for (int i = 0; i < 8; ++i) {
        padding.push_back(*(p - i));
    }
    return padding;
}

vector<byte> md4_glue_padding(int key_length, int message_length){
    vector<byte> padding;
    padding.push_back(0x80);
    while (((padding.size() + message_length + key_length) * 8) % 512 != 448)
        padding.push_back(0x00);
    uint64_t ml = (key_length + message_length) * 8;
    byte * p = (byte *)&ml;
    for (int i = 0; i < 8; ++i) {
        padding.push_back(*(p + i));
    }
    return padding;
}
