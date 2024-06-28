#include <iostream>
#include "./includes/compress.h"
#include "./includes/decompress.h"

using namespace std;

int main() {
    cout << "Welcome to BinBinder!" << endl;
    cout << "What would you like to do?" << endl;
    cout << "1. Compress a file" << endl;
    cout << "2. Decompress a file" << endl;
    cout << "3. Quit the application" << endl;
    cout << endl;

    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1: {
            char inputFile[256];
            cout << "Enter the name of the file to compress: ";
            cin >> inputFile;

            compressFile(inputFile);
            // if () {
            //     cout << "Compression successful!" << endl;
            // } else {
            //     cerr << "Compression failed." << endl;
            // }
            break;
        }
        case 2: {
            char compressedFile[256];
            cout << "Enter the name of the compressed file: ";
            cin >> compressedFile;

            decompressFile(compressedFile);
            // if (decompressFile(compressedFile)) {
            //     cout << "Decompression successful!" << endl;
            // } else {
            //     cerr << "Decompression failed." << endl;
            // }
            break;
        }
        case 3: {
            cout << "Exiting the application. Thank you!" << endl;
            break;
        }
        default:
            cerr << "Invalid choice. Exiting..." << endl;
            break;
    }

    return 0;
}
