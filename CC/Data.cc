#include "Data.hh"

void Data::toB64(){
    if(this->type == 1) return;
    this->data = byteArrayToB64(this->data);
}

void Data::toByteArray(){
    if(this->type != 1) return;
    this->data = b64ToByteArray(this->data);
}

Data::Data(){
    this->data = vector<byte>();
    this->type = 0;
}

Data::Data(int i){
    this->data = vector<byte>(i, 0);
    this->type = 0;
}

Data::Data(int i, byte b){
    this->data = vector<byte>(i, b);
    this->type = 0;
}

Data::Data(string input){
    data = hexToByteArray(input);
    this->type = 0;
}

Data::Data(string input, int type){
    switch (type){
        case 0:
            data = hexToByteArray(input);
            this->type = 0;
            break;
        case 1:
            data = b64StringToArray(input);
            this->type = 1;
            break;
        case 2:
            data = stringToByteArray(input);
            this->type = 2;
            break;
        default:
            cerr << "bad type, created empty Data" << endl;
            data = vector<byte>();
            this->type = 0;
            break;
    }
}

Data::Data(vector<byte> data){
    this->data = data;
    this->type = 0;
}

Data::~Data(){}

vector<byte> Data::getData() const{
    return data;
}

byte Data::pop(){
    byte b = data[data.size()-1];
    data.pop_back();
    return b;
}

void Data::push(byte b){
    data.push_back(b);
}

int Data::size() const{
    return data.size();
}

int Data::getType() const{
    return type;
}

void Data::setType(int t){
    this->type = t;
}

void Data::toType(int t){
    if(this->type == t) return;
    if(this->type != 1 and t == 1){
        this->toB64();
        this->type = 1;
        return;
    }
    if(this->type == 1){
        this->toByteArray();
        this->type = t;
        return;
    }
    this->type = t;
}

void Data::pkcs7_pad(int newSize){
    byte n = newSize - data.size();
    for(int i = 0; data.size() < newSize; ++i)
        data.push_back(n);
}

void Data::pkcs7_strip_padding(){
    bool valid = this->pkcs7_validate_padding();
    byte num = data[data.size() - 1];
    if(valid){
        vector<byte> out(data.size()-num);
        for(int i = 0; i < out.size(); ++i)
            out[i] = data[i];
        data = out;
    }
    else{
        cout << "invalid padding" << endl;
    }
}

bool Data::pkcs7_validate_padding(){
    byte num = data[data.size() - 1];
    if ((int)num > data.size()) return false;
    else if(num == 0) return false;
    bool cond = true;
    for(int i = data.size() - (int)num; i < data.size() and cond; ++i)
        if(data[i] != num) cond = false;
    return cond;
}

Data operator+(const Data& a, const Data& b){
    Data out (a.size() + b.size());
    for(int i = 0; i < a.size(); ++i){
        out[i] = a[i];
    }
    for(int i = 0; i < b.size(); ++i){
        out[i + a.size()] = b[i];
    }
    out.setType(a.getType());
    return out;
}

Data operator+(const Data& a, byte b){
    Data out (a.size() + 1);
    for(int i = 0; i < a.size(); ++i){
        out[i] = a[i];
    }
    out[a.size()] = b;
    out.setType(a.getType());
    return out;
}

void Data::operator+=(Data& d){
    for(int i = 0; i < d.size(); ++i){
        data.push_back(d[i]);
    }
}

void Data::operator+=(byte b){
    data.push_back(b);
}

byte& Data::operator[](int i){
    return data[i];
}

byte Data::operator[](int i) const{
    return data[i];
}

Data operator^(const Data& a, const Data& b){
    vector<byte> v = a.getData();
    vector<byte> v2 = b.getData();
    Data out(repeating_key_xor(v, v2));
    if(a.getType() == 2) out.setType(0);
    else out.setType(a.getType());
    return out;
}

Data operator^(const Data& a, const vector<byte>& b){
    vector<byte> v = a.getData();
    Data out(repeating_key_xor(v, b));
    if(a.getType() == 2) out.setType(0);
    else out.setType(a.getType());
    return out;
}

Data operator^(const Data& a, string b){
    vector<byte> v = a.getData();
    Data out(repeating_key_xor(v, b));
    if(a.getType() == 2) out.setType(0);
    else out.setType(a.getType());
    return out;
}

Data operator^(const Data& a, byte b){
    Data out(single_key_xor(a.getData(), b));
    if(a.getType() == 2) out.setType(0);
    else out.setType(a.getType());
    return out;
}

ostream &operator<<(ostream& os, const Data& d){
    vector<byte> data = d.getData();
    switch (d.getType()){
        case 0:
        os << "0x";
            for(int i = 0; i < data.size(); ++i){
                os << table_hex[data[i] >> 4];
                byte aux = data[i] << 4;
                os << table_hex[aux >> 4];
            }
            break;
        case 1:
            for (int i = 0; i < data.size(); ++i)
                os << table_64[(int)data[i]];
            break;
        case 2:
            for (int i = 0; i < data.size(); ++i){
                if (not ((data[i] >= 127 or data[i] < 32) and data[i] != 8 and data[i] != 9 and data[i] != 10))
                    os << (char)data[i];
                else
                    os << "\\" << (int)data[i];
            }
            break;
    }
    return os;
}

void Data::operator= (const vector<byte>& v){
    this->data = v;
    this->type = 0;
}
