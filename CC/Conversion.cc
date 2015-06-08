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
    if(c == '=') return 64;
    for(int i = 0; i < 64; ++i){
        if (table_64[i] == c)
            return (byte) i;
    }
    return 63;
}

vector<byte> hexToByteArray(const string& hex){
	vector<byte> v (hex.size() / 2);
	for(int i = 0; i < hex.size(); i += 2){
		v[i/2] = hexToDec(hex[i])*16 + hexToDec(hex[i+1]);
	}
	return v;
}

vector<byte> byteArrayToB64(const vector<byte>& v){    
    int n = (int) v.size()/3 * 4;
    if (v.size()%3 == 1)
        n += 2;
    else if (v.size()%3 == 2)
        n += 3;

    int p2 = 0;
    int step = 0;
    vector<byte> B64 (n, 0);
    byte aux;
    for (int i = 0; i < v.size(); ++i){
        if(step == 0){
            B64[p2] = v[i] >> 2;
            ++p2;
            aux = v[i] << 6;
            B64[p2] = aux >> 2;
            ++step;
        }
        else if(step == 1){
            aux = v[i] >> 4;
            B64[p2] = B64[p2] | aux;
            ++p2;
            aux = v[i] << 4;
            B64[p2] = aux >> 2;
            ++step;
        }
        else if(step == 2){
            aux = v[i] >> 6;
            B64[p2] = B64[p2] | aux;
            ++p2;
            aux = v[i] << 2;
            B64[p2] = aux >> 2;
            ++p2;
            step = 0;
        }
    }
    if (v.size()%3 > 0) B64.push_back(64);
    if (v.size()%3 == 1) B64.push_back(64);
    return B64;
}

vector<byte> b64ToByteArray(const vector<byte>& v){
    int pad = 0;
    for(int i = v.size()-1; v[i] == 64; --i) pad++;
    cout << pad << endl;
    int n = (int) (((float)(v.size()-pad)/4) * 3);
    cout << v.size() << endl;
    cout << n << endl;
    
    /*
    if ((v.size()-pad)%4 == 1)
        n ++;
    else if ((v.size()-pad)%4 == 2)
        n += 2;
    else if ((v.size()-pad)%4 == 3)
        n += 3;
    */
    

    int p2 = 0;
    int step = 0;
    vector<byte> out (n, 0);
    byte aux;
    for (int i = 0; i < v.size()-pad; ++i){
        if(step == 0){
            out[p2] = v[i] << 2;
            ++step;
        }
        else if(step == 1){
            aux = v[i] >> 4;
            out[p2] = out[p2] | aux;
            ++p2;
            aux = v[i] << 4;
            out[p2] = aux;
            ++step;
        }
        else if(step == 2){
            aux = v[i] >> 2;
            out[p2] = out[p2] | aux;
            ++p2;
            aux = v[i] << 6;
            out[p2] = aux;
            ++step;
        }
        else if(step == 3){
            out[p2] = out[p2] | v[i];
            ++p2;
            step = 0;
        }
    }
    return out;
}

vector<byte> stringToByteArray(const string& str){
    vector<byte> out (str.size());
    for (int i = 0; i < str.size(); ++i){
        out[i] = str[i];
    }
    return out;
}

vector<byte> intToByteArray(const unsigned long int &num, bool invert){
    byte *p = (byte*)&num;
    vector<byte> out (sizeof(unsigned long int));
    for(int i = 1; i <= sizeof(unsigned long int); ++i){
        if(invert)
            out[out.size() - i] = *p;
        else
            out[i - 1] = *p;
        p++;
    }
    return out;
}

vector<byte> b64StringToByteArray(const string& str){
    vector<byte> out = stringToByteArray(str);
    for(int i = 0; i < out.size(); ++i)
        out[i] = charToB64(out[i]);
    return b64ToByteArray(out);
}









