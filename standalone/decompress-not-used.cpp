#include <iostream>
#include <unordered_map>

using namespace std;

string convert2binary(int decimal){
    string s = "";

    // this fails as neg numbers can also come and this conversion is only for pos numbers
    // bool neg = 0;
    // if(decimal<0){
    //     neg = 1;
    //     decimal*=-1;
    // }
    // while(decimal){
    //     s = to_string(decimal%2) + s;
    //     decimal = decimal/2; 
    // }
    // while(s.length()<=8){
    //     s = '0' + s;
    // }
    // if(neg){
    //     s[0] = 1;
    // }

    // since we require the desired ascii value to be 8 bits
    for(int i = 7; i>=0; i--){
        if(decimal>>i & 1)s += '1';
        else s += '0';
    }

    return s;
}

int main(int argc, char* argv[]){
    
    // checking if a file has been given
    if(argc!=2){
        cout<<"Requires a file to be decompressed."<<'\n';
        cout<<"Usage: ./a.exe {input_file_name}";

        return 0;
    }

    // opening the file to be processed
    FILE *input = fopen(argv[1],"rb");

    // checking if file has opened
    if(input==NULL){
        cout<<"Error opening the file";
        
        return 0;
    }

    // reading 1 byte at a time
    char buffer[1];

    // reading the extension size
    fread(buffer,1,1,input);
    int extensionSize = buffer[0]-'0'; 

    // building output file name
    string in = argv[1];
    string out = in.substr(0,in.find("-compressed")) + "-decompressed.";

    // reading the extension 
    while(extensionSize--){
        fread(buffer,1,1,input);
        out = out + buffer[0];
    }

    // creating the output file
    // here .c_str() used as fopen requires the first parameter to be of type const char* type: an array of char
    FILE *output = fopen(out.c_str(),"w"); 
    if(output==NULL){
        cout<<"Error creating output file";
        
        return 0;
    }

    // creating a map of the code words
    unordered_map<string,char> code_words;
    string code = "";
    int flag = 0;

    while(fread(buffer,1,1,input)){
        // checking if it's the end of the particular codeword
        if(buffer[0]!='\0'){
            flag = 0;
            code = code + buffer[0];
        }

        // if yes it's the end
        else{
            flag++;
            if(flag==2) break;

            //storing the binary value of each character
            code_words[code.substr(1,code.length()-1)] = code[0];
            code = "";
        }
    }

    // checking if the map is correctly formed
    // for(auto it:code_words){
    //     cout<<it.first<<" "<<it.second<<'\n';
    // }

    // reading the padding size and storing
    int padding = 0;
    fread(buffer,1,1,input);
    padding = buffer[0]-'0';

    // and also the extra null chars read
    fread(buffer,1,1,input);
    fread(buffer,1,1,input);

    // getting the actual binary code
    code = "";
    int dec = 0;

    while(fread(buffer,1,1,input)){
        dec = buffer[0];

        // here convert2binary functions converts the binary form to be 8 bits
        code = code + convert2binary(dec);
    }

    //checking if code correctly inserted
    // cout<<code<<" ";
    // return 0;

    // processing of binary coded words
    int start = 0;
    // checking if there is padding or not
    if(padding==0){
        code = '0' + code;
        start = 1;
    }else{
        start = padding;
    }

    // getting back the actual ascii
    for(int i = 1; code[start]!='\0'; i++){
        if(code_words.find(code.substr(start,i)) != code_words.end()){
            fwrite(&(code_words[code.substr(start,i)]),1,1,output);
            // cout<<code.substr(start,i)<<" "<<code_words[code.substr(start,i)]<<" ";
            start = start + i;
            i = 0;
        }
    }

    cout<<"File Decompressed!"<<'\n';
    cout<<"File name (with path): "<<out.c_str()<<"\n";

    fclose(input);
    fclose(output);

    return 0;
}


/*
File: *-compressed.bin format
- First character represents the extension size
- Then follows the ext name
- Then follows the code words i.e binary form of all the chars
- Next tells us the padding 
- After which the coded text can be found
*/