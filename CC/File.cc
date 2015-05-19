#include "File.hh"


void File::searchOnFile(const string filename, int thres){
    vector<byte> v;
    ifstream cypherfile;
    cypherfile.open(&filename[0]);
    string line;
    if (cypherfile.is_open()){
        int count = 1;
        while(getline (cypherfile,line)){
            v = Conversion::hexToByteArray(line);
            if(Attack::testBytes(v, thres))
                cout << "line " << count << endl;
            count++;
        }
    }
    else
        cout << "unable to open file" << endl;

    cypherfile.close();
}

vector<byte> File::fetchFromFile(const string filename){
    ifstream file;
    file.open(&filename[0]);
    string line;
    string block;
    if (file.is_open()){
        while(getline (file,line)){
            block = block + line;
        }
    }
    else
        cout << "unable to open file" << endl;

    file.close();

    //put string into byte array (B64)
    vector<byte> blockArray (block.size());
    for(int i = 0; i < block.size(); ++i){
        blockArray[i] = Conversion::charToB64(block[i]);
    }

    //B64 -> Array
    return Conversion::b64ToByteArray(blockArray);
}

vector<string> File::getStrings(const string filename){
    ifstream file;
    file.open(&filename[0]);
    string line;
    vector<string> ret(0);
    if(file.is_open()){
        while(getline(file, line)){
            ret.push_back(line);
        }
    }
    else
        cout << "unable to open file" << endl;

    file.close();

    return ret;
}



