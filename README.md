# BitBinder

## A file compression and decompression application built in C++ using Huffman coding.

### Running the code
Clone the repository and open the terminal in the same folder and run the following commands to compile the code.
* g++ -c main.cpp -o main.o
* g++ -c ./includes/compress.cpp -o compress.o
* g++ -c ./includes/decompress.cpp -o decompress.o
* g++ main.o compress.o decompress.o -o BitBinder

Now to run the code, in the terminal, type
* ./BitBinder.exe

**NOTE:** Make sure the path to the file to be compressed/decompressed is ready with you when you run the code.

### Introduction
A Huffman code is an optimal prefix code commonly used for lossless data compression, the algorithm of which was developed by David A. Huffman in the 1950’s. It is a variable-length encoding which works by assigning smaller codes to more frequently occurring characters(symbols) and longer codes to those characters(symbols) whose occurrence is relatively rare to have overall optimised data compression results. The time complexity of this algorithm is *O(n)* where *n* is the number of characters occurring in the data to be encoded, if the frequencies(weights) are sorted. The codes obtained in Huffman coding are *Prefix codes*. Prefix codes are those where the code of one symbol will not form a prefix of a code assigned to any other symbol. This helps us to represent a character in possibly *less that 8 bits*, hence reducing the size of the compressed file.

A *Huffman tree* is used to generate the most optimal codes. The file type(extension) and the Huffman codes are written in the compressed file before the actual content which are later used to decompress the file to original state. Every time we move to the *left subtree*, we add a *0* to the code whereas we add a *1* when moving onto the *right subtree*. We keep adding 0’s and 1’s as we move down from the root till we reach a *leaf node* where the characters are stored. [More on Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)

This application can be used to compress any text based files(like ```.txt```, ```.docx```, ```.c```, ```.cpp```, ```.html```, ```.js``` etc)

### Working
Following are the steps to build and traverse the Huffman tree:
1.	Create a *minheap (priority queue)* based on the frequencies of each character, the nodes corresponding to the unique characters present int the input file.
2.	Until the number of nodes in the minheap becomes 1, keep popping 2 nodes and insert their newly created parent node into the heap (first popped node is the left child) and run *heapify*. 
3.	Traverse the Huffman Tree and keep adding a 0 each time we move to the left subtree and a 1 on moving to the right subtree till we reach the leaf nodes, the string obtained becomes the prefix code of the character stored in the leaf. 
4.	Create a *hashmap* mapping each character with its prefix code and write this hashmap in the compressed file.

**NOTE:**
1.	It is important to ensure the number of bits in the string produced by replacing each character with its code is a multiple of 8 so that we can now read 8 bits at a time, get its decimal equivalent and the corresponding ASCII equivalent character is finally written to the compressed file. This is where the actual compression takes place and the compressed file generated is smaller than the input file.
2.	The size of the input file must be big enough to notice the size difference. A very small input file if encoded/compressed would otherwise result in a bigger compressed file owing to the additional contents of the hashmap written at the beginning (which would surpass the reduction in size obtained by replacing characters with prefix codes). This should be avoided as there is no point in having a compression algorithm if the input file is so small.

### Result
To test the compression algorithm, we have used a randomly generated ```tester.txt``` file which is around 14KB in size. Upon compressing the file, we can see that there is around **42% reduction in size** of the input file (14KB to 8KB). This file is now decompressed and the tester-decompressed.txt file is obtained which is 14KB in size and retains all the original contents of tester.txt without any loss in data. Hence the name ***Lossless Data Compression Algorithm***.

There are two more files used to test the code(```tester2.docx``` and ```tester3.html```)

### Conclusion
We can arrive at the conclusion that this lossless data compression algorithm based on Huffman coding is one of the most efficient ways of encoding and compressing data. Using this, we are able to represent characters that occur more frequently with possibly lesser than 8 bits. Therefore, the number of bits saved by these characters greatly surpasses the extra bits which might possibly be used to represent characters that occur very rarely. This causes the compressed file to occupy (in best cases) less than 50 percent of the original file size that too without losing any kind of data!
