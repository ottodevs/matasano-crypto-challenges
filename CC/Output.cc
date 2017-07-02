#include "Output.hh"

void printB64(const vector <byte>&v){
    for (int i = 0; i < v.size(); ++i) {
        cout << table_64 [(int)v [i]];
    }
    // check if '='s needed
    cout << endl;
}

void printHex(const vector <byte>&v){
    for (int i = 0; i < v.size(); ++i) {
        cout << table_hex [v [i] >> 4];
        byte aux = v [i] << 4;
        cout << table_hex [aux >> 4];
    }
    cout << endl;
}

void printInt(const vector <byte>&v){
    cout << (int)v [0];
    for (int i = 0; i < v.size(); ++i) {
        cout << " " << (int)v [i];
    }
    cout << endl;
}

void printChar(const vector <byte>&v){
    for (int i = 0; i < v.size(); ++i) {
        if (not ((v [i] >= 127 or v [i] < 32) and v [i] != 8 and v [i] != 9 and
                 v [i] != 10))
            cout << (char)v [i];
        else
            cout << "\\" << (int)v [i];
    }
    // cout << endl;
}

void printAscii(int i){
    if (not ((i >= 127 or i < 32) and i != 8 and i != 9 and i != 10))
        cout << (char)i;
    else
        cout << "\\" << (int)i;
}

void printAscii(byte i){
    if (not ((i >= 127 or i < 32) and i != 8 and i != 9 and i != 10))
        cout << (char)i;
    else
        cout << "\\" << (int)i;
}
