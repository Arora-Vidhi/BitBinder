#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <unordered_map>
#include "compress.h"
using namespace std;

unordered_map<char, string> hashmap;

struct huffnode
{
    char character;
    unsigned int freq;
    struct huffnode *left;
    struct huffnode *right;
};
typedef struct huffnode *NODE;

struct Minheap {
    unsigned short size, capacity;
    NODE *array;
};
typedef struct Minheap *HEAP;

NODE newleafnode(char c, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = c;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void swap(NODE *a, int i, int j) {
    NODE tmp = (NODE)malloc(sizeof(huffnode));
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap) {
    int i, j, k;
    unsigned short n = heap->size;
    NODE *a = heap->array;
    for (i = n / 2; i > 0; i--) {
        if (2 * i + 1 <= n) {
            j = a[2 * i]->freq < a[2 * i + 1]->freq ? 2 * i : 2 * i + 1;
        }
        else {
            j = 2 * i;
        }

        if (a[j]->freq < a[i]->freq)
        {
            swap(a, i, j);
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n ? a[j]->freq > a[2 * j + 1]->freq : 0))) {
                if (2 * j + 1 <= n) {
                    k = a[2 * j]->freq < a[2 * j + 1]->freq ? 2 * j : 2 * j + 1;
                }
                else {
                    k = 2 * j;
                }
                swap(a, k, j);
                j = k;
            }
        }
    }
}

NODE mintop(HEAP heap) {
    NODE top = (NODE)malloc(sizeof(huffnode));
    top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, int f) {
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap) {
    while (heap->size > 1) {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, (left_child->freq + right_child->freq));
    }
}

void assign_codes(NODE root, string str) {
    string s1 = "", s2 = "";
    if (!root->left && !root->right) {
        hashmap[root->character] = str;
    } else {
        s1 = str + "0";
        s2 = str + "1";
        assign_codes(root->left, s1);
        assign_codes(root->right, s2);
    }
}

bool compressFile(const char* inFile) {

    // Validating if input is given
    // if (argc != 2) {
    //     cout << "Invalid cmd line arg. Usage: .\a.exe <input file> \n";
    //     return(1);
    // }

    // Opening input file
    FILE *input = fopen(inFile, "r");
    // Checking opened file
    if (input == NULL) {
        cout << "Error while opening the input file.\n";
        return(2);
    }

    // Naming the files
    string in = inFile;
    string filetype = in.substr(in.find_last_of(".")+1);
    cout << "File is of ." << filetype << " type. \n";
    string out = in.substr(0, in.find_last_of(".")) + "-compressed.bin";

    // Opening output file
    FILE *output = fopen(out.c_str(), "wb");
    if (output == NULL) {
        cout << "Error creating output file. \n";
        return(3);
    }

    // Variables to process info
    string file_contents = "";
    char buff[1];

    // Write file type
    int ftsize = filetype.length();
    char num = ftsize + '0';
    fwrite(&num, 1, 1, output);

    for(int i = 0; i < ftsize; i++) {
        buff[0] = filetype[i];
        fwrite(buff, 1, 1, output);
    }

    // Holds frequency of each unique character
    int i, f_size = 0, freq[256] = {0};
    unsigned short num_of_unique_chars = 0;
    while (fread(buff, 1, 1, input)) {
        if (!freq[buff[0] - 0]) {
            num_of_unique_chars++;
        }
        freq[buff[0] - 0]++;
        f_size++;
    }
    fclose(input);
    FILE *input1 = fopen(inFile, "r");

    // Declaring data structure
    HEAP heap = (HEAP)malloc(sizeof(struct Minheap));
    heap->capacity = num_of_unique_chars + 1;
    heap->size = 0;
    heap->array = (NODE *)malloc(heap->capacity * sizeof(struct huffnode));

    // Storing input to the heap
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            heap->size++;
            heap->array[heap->size] = newleafnode(i, freq[i]);
        }
    }

    // Heapify
    heapify(heap);

    // Construct the huffman tree and assign the codes to he characters
    huffman_tree(heap);
    assign_codes(heap->array[heap->size], "");
    // unordered_map<char, string>::iterator x;

    // Convert file content to binary code by assigning codes to each character
    while (fread(buff, 1, 1, input)) {
        file_contents += hashmap[buff[0]];
    }

    // Find the padding to make binary code length a multiple of 8
    int padding = 0;
    if (file_contents.length() % 8 != 0) {
        padding = 8 - (file_contents.length() % 8);
    }

    // Add the padding
    for (int i = 0; i < padding; i++) {
        file_contents = "0" + file_contents;
    }

    // Write the codes assigned to characters at the start of the file
    string codes;
    unordered_map<char, string>::iterator iter;
    for (iter = hashmap.begin(); iter != hashmap.end(); iter++) {
        codes = (iter->first + iter->second);
        fwrite(codes.c_str(), codes.length() + 1, 1, output);
    }

    // Write the padding given to the binary code
    char null = '\0';
    char pad = padding + '0';
    fwrite(&null, 1, 1, output);
    fwrite(&pad, 1, 1, output);
    // To identify padding with 2 null characters
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);

    // Take 8 bit binary code -> decimal -> character and write char to output file.
    char tmp_bin[9];
    int dec, w;
    for (int j = 0; file_contents[j]; j = j + 8) {
        strncpy(tmp_bin, &file_contents[j], 8);
        // Convert to integer equivalent and then character
        dec = 0;
        i = 7;
        while (i > -1) {
            w = tmp_bin[i] == '1' ? 1 : 0;
            dec += ((int)pow(2, 7 - i)) * w;
            i--;
        }
        buff[0] = dec;
        fwrite(buff, 1, 1, output);
    }
    free(heap);
    fclose(input1);
    fclose(output);
    cout<<"Compressed File successfully\n";
    cout<<"File name (with path): "<<out.c_str()<<"\n";

    return 1;
}