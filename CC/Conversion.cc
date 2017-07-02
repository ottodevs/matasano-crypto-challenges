#include "Conversion.hh"

int hexToDec(char hex){
    if ('0' <= hex && hex <= '9') {
        return hex - '0';
    }

    if (hex >= 'A' && hex <= 'Z') {
        return hex - 'A'  + 10;
    }

    return hex - 'a' + 10;
}

byte charToB64(char c){
    if (c == '=') return 64;
    for (int i = 0; i < 64; ++i) {
        if (table_64[i] == c)
            return (byte)i;
    }
    return 63;
}

char letter(byte c){
    char lt = (char)c;

    if (c == ' ') return c;
    if (c >= 'a' and c <= 'z') return lt;
    if (c >= 'A' and c <= 'Z') return lt - 'A' + 'a';
    if ((c < 32 or c > 126) and c != 8 and c != 9 and c != 10) {
        return '?';
    }
    return '$';
}

vector<byte> hexToByteArray(const string& hex){
    int n;
    int zero = (hex[1] == 'x' ? 2 : 0);

    if (hex.size() % 2 == 0) n = hex.size() / 2;
    else n = (hex.size() / 2) + 1;
    vector<byte> v(n, 0);
    for (int i = 0; i < hex.size() - zero; i++) {
        byte aux = (byte)hexToDec(hex[hex.size() - (i + 1)]);
        if ((i + 1) % 2 == 0) {
            aux = aux << 4;
        }
        v[v.size() - 1 - (i / 2)] = v[v.size() - 1 - (i / 2)] | aux;
    }
    return v;
}

vector<byte> byteArrayToB64(const vector<byte>& v){
    double nd = (double)v.size() * 4.0 / 3.0;
    int n;

    if ((int)nd == nd) {
        n = (int)nd;
    } else {
        n = (int)nd + 1;
    }

    int p = 0;
    int step = 0;
    vector<byte> B64(n, 0);
    byte aux = 0;
    for (int i = 0; i < v.size(); ++i) {
        if (step == 0) {
            B64[p] = v[i] >> 2;
            ++p;
            aux = v[i] << 6;
            B64[p] = aux >> 2;
            ++step;
        } else if (step == 1) {
            aux = v[i] >> 4;
            B64[p] = B64[p] | aux;
            ++p;
            aux = v[i] << 4;
            B64[p] = aux >> 2;
            ++step;
        } else if (step == 2) {
            aux = v[i] >> 6;
            B64[p] = B64[p] | aux;
            ++p;
            aux = v[i] << 2;
            B64[p] = aux >> 2;
            ++p;
            step = 0;
        }
    }
    if (step == 2) B64.push_back(64);
    if (step == 1) {
        B64.push_back(64);
        B64.push_back(64);
    }
    return B64;
} // byteArrayToB64

vector<byte> b64ToByteArray(const vector<byte>& v){
    int pad = 0;

    for (int i = v.size() - 1; v[i] == 64; --i) {
        pad++;
    }
    int n = (v.size() * 3 / 4);
    n -= pad;

    int p = 0;
    int step = 0;
    vector<byte> out(n, 0);
    byte aux;
    for (int i = 0; i < v.size() - pad; ++i) {
        if (step == 0) {
            out[p] = v[i] << 2;
            ++step;
        } else if (step == 1) {
            aux = v[i] >> 4;
            out[p] = out[p] | aux;
            ++p;
            out[p] = v[i] << 4;
            ++step;
        } else if (step == 2) {
            aux = v[i] >> 2;
            out[p] = out[p] | aux;
            ++p;
            out[p] = v[i] << 6;
            ++step;
        } else if (step == 3) {
            out[p] = out[p] | v[i];
            ++p;
            step = 0;
        }
    }
    return out;
} // b64ToByteArray

vector<byte> stringToByteArray(const string& str){
    vector<byte> out(str.size());
    for (int i = 0; i < str.size(); ++i) {
        out[i] = str[i];
    }
    return out;
}

vector<byte> intToByteArray(const uint64_t &num, bool invert){
    byte * p = (byte *)&num;

    vector<byte> out(sizeof(unsigned long int));
    for (int i = 1; i <= sizeof(unsigned long int); ++i) {
        if (invert)
            out[out.size() - i] = *p;
        else
            out[i - 1] = *p;
        p++;
    }
    return out;
}

vector<byte> b64StringToArray(const string& str){
    vector<byte> out(str.size());
    for (int i = 0; i < out.size(); ++i) {
        out[i] = charToB64(str[i]);
    }
    return out;
}

vector<byte> b64StringToByteArray(const string& str){
    vector<byte> out(str.size());
    for (int i = 0; i < out.size(); ++i) {
        out[i] = charToB64(str[i]);
    }
    return b64ToByteArray(out);
}
