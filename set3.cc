#include "File.hh"
#include "Target.hh"
#include "User.hh"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "Block.hh"
#include "Conversion.hh"
#include "Attack.hh"
#include "Output.hh"
#include "Data.hh"
#include "Aes.hh"
#include "RNG.hh"
#include "Stream.hh"

// The CBC padding oracle
void ch17(){
    Target google;

    Target::globKey = gen_random_block();
    vector<string> lines = getStrings("INPUT/ch17.txt");
    time_t secs;
    time(&secs);
    srand((uint32_t)secs);
    int line = (int)rand() % 10;

    Data cipher(lines[line], 1);
    cipher.toType(2);

    Data c2 = google.encrypt_CBC(cipher);

    Data plain = padding_oracle_attack(c2, Target::globKey, google);
    plain.pkcs7_strip_padding();
    cout << "string found!!" << endl;
    plain.toType(2);
    cout << plain << endl;
} // ch17

// Implement CTR
void ch18(){
    Data v(ch18_str, 1);

    v.toType(2);
    uint64_t nc = 0;
    vector<byte> nonce = intToByteArray(nc, false);
    v = aes_128_CTR(v, keySub, nonce);
    v.toType(2);
    cout << v << endl;
}

// Break fixed-nonce CTR mode (Substitutions)
void ch19(){
    vector<string> strings = getStrings("INPUT/ch19.txt");
    vector<byte> nonce = intToByteArray(0, false);

    int maxSize = 0;
    for (int i = 0; i < strings.size(); ++i) {
        if (strings[i].size() > maxSize)
            maxSize = strings[i].size();
    }
    bool exit = false;
    vector<byte> keys(maxSize, 0);
    while (not exit) {
        for (int i = 0; i < strings.size(); ++i) {
            cout << "[" << i + 1 << "]";
            Data v(strings[i], 1);
            v.toType(0);
            v = aes_128_CTR(v, keySub, nonce);
            for (int l = 0; l < v.size(); ++l) {
                v[l] = v[l] ^ keys[l];
            }
            v.toType(2);
            cout << v << endl;
        }
        cout << endl << "Add substitution:";
        int line_num, key;
        cin >> line_num >> key;
        if (key <= 0)
            keys[line_num]++;
        else
            keys[line_num] = (byte)key;
        cout << "key " << line_num << "-->" << (int)keys[line_num] <<  endl;
    }
} // ch19

// Break fixed-nonce CTR mode (statistically)
void ch20(){

    /*
     * Only breaks until size of smallest string.
     */

    vector<string> strings = getStrings("INPUT/ch20.txt");
    vector<byte> nonce = intToByteArray(0, false);

    vector< Data > ciphers(strings.size());
    for (int i = 0; i < ciphers.size(); ++i) {
        if (i != 23) {
            Data d(strings[i], 1);
            d.toType(2);
            ciphers[i] = (aes_128_CTR(d, keySub, nonce));
        } else ciphers[i] = Data("35a4b16bd6cd3390c3ce61321967e306bdcc1fb9fd4d73feabb36cf2dc9a017758d3aeb8781c5f3fe3f59767928ce1bfa7efd95828940d30f1220311ac1d85d1e7e7b25887613c8a49f7e58e82b5b38ae281fae8b2503480", 0);
    }
    cout << 34343 << endl;

    int min_size = ciphers[0].size();
    for (int i = 1; i < ciphers.size(); ++i) {
        if (ciphers[i].size() < min_size)
            min_size = ciphers[i].size();
    }
    cout << "min: " << min_size << endl;

    for (int i = 0; i < ciphers.size(); ++i) {
        while (ciphers[i].size() != min_size)
            ciphers[i].pop();
    }
    vector< Data > groupedBlock(min_size);

    for (int i = 0; i < groupedBlock.size(); ++i) {
        for (int j = 0; j < ciphers.size(); ++j) {
            groupedBlock[i].push(ciphers[j][i]);
        }
    }

    vector<byte> keyV = findRepeatingKey(groupedBlock);

    cout << endl << "detected key: ";
    printInt(keyV);
    cout << endl << "-------------------------" << endl;
    cout << "i key to change a byte, -1 to end:" << endl;
    int in, ke;
    cin >> in;
    while (in != -1) {
        cin >> ke;
        keyV[in] = (byte)ke;
        cin >> in;
    }
    cout << "new key: ";
    printInt(keyV);

    for (int i = 0; i < strings.size(); ++i) {
        if (i % min_size == 0)
            cout << endl;
        vector<byte> vb = ciphers[i].getData();
        vector<byte> plain = repeating_key_xor(vb, keyV);

        printChar(plain);
        cout << endl;
    }
} // ch20

// Implement the MT19937 Mersenne Twister RNG
void ch21(){
    cout << "introduce seed to generate random numbers: ";
    int seed;
    cin >> seed;
    rng mt(seed);
    // 100 random numbers:
    for (int i = 0; i < 100; ++i) {
        cout << mt.extract_number() << endl;
    }
}

// Crack an MT19937 seed
void ch22(){
    time_t secs;

    time(&secs);
    srand((unsigned int)secs);
    int time1 = (int)rand() % 1000;
    int time2 = (int)rand() % 1000;


    sleep(time1);

    time_t seed = time(0);
    rng mt(seed);

    sleep(time2);

    unsigned int n = mt.extract_number();
    cout << "number generated: " << n << endl;
    cout << "seed: ";
    time_t start = time(0);
    bool found = false;
    for (int d = 0; d <= 1000 and not found; ++d) {
        rng r(start - d);
        if (r.extract_number() == n) {
            found = true;
            cout << start - d << endl;
        }
    }
} // ch22

// Clone an MT19937 RNG from its output
void ch23(){
    rng rand(453256);
    uint32_t state[624];

    cout << "generator output(numbers 625-645):" << endl;
    for (int i = 0; i < 624; ++i) {
        uint32_t n = rand.extract_number();
        state[i] = mt19937_untemper(n);
    }
    for (int i = 0; i < 20; ++i) {
        cout << rand.extract_number() << endl;
    }
    rng clone(&state[0]);
    cout << "cloned generator output(first 20 numbers):" << endl;
    for (int i = 0; i < 20; ++i) {
        cout << clone.extract_number() << endl;
    }
}

// Create the MT19937 stream cipher and break it
void ch24(){
    string message = "AAAAAAAAAAAAAA";
    Data v(message, 2);
    Data p(gen_random_bytes(-1));

    v = p + v;
    v = mt19937(v, 33212);
    int key;
    bool found = false;
    for (key = 0; key < 65536 and not found; ++key) {
        p = mt19937(v, key);
        bool dec = true;
        for (int j = v.size() - 1; j >= v.size() - 14 and dec; --j) {
            if (p[j] != 'A') dec = false;
        }
        found = dec;
    }
    key--;
    if (found) {
        cout << "key: " << key << endl;
        p.toType(2);
        cout << p << endl;
    } else cout << "not found" << endl;
    cout << endl << "-----------------------" << endl << endl;

    time_t now = time(0);
    rng gen(now);
    uint32_t n1 = gen.extract_number();
    gen.seed(234321);
    uint32_t n2 = gen.extract_number();

    if (is_time_seeded(n1, 500)) cout << "is time seeded" << endl;
    else cout << "not timeseeded" << endl;
    if (is_time_seeded(n2, 500)) cout << "is time seeded" << endl;
    else cout << "not timeseeded" << endl;
} // ch24

/*
 * MAIN
 */
int main(){
    int ch;

    cout << "[17-24]" << endl;
    cin >> ch;
    switch (ch) {
        case 17:
            ch17();
            break;
        case 18:
            ch18();
            break;
        case 19:
            ch19();
            break;
        case 20:
            ch20();
            break;
        case 21:
            ch21();
            break;
        case 22:
            ch22();
            break;
        case 23:
            ch23();
            break;
        case 24:
            ch24();
            break;
    } // switch
} // main
