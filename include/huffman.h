#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TREE_HT 100

// Huffman tree node
typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// A Min Heap: Collection of min-heap (or Huffman) nodes
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffmanNode **array;
} MinHeap;

// Function to build a Huffman tree
HuffmanNode* build_huffman_tree(char data[], int freq[], int size);

void generate_codes(HuffmanNode* root, int codes[][100]);

// Function to encode a message
void huffman_encode(char *message, int codes[][100], char *encoded_message);

// Function to decode a message
void huffman_decode(char *encoded_message, char *decoded_message, HuffmanNode* root);

#endif // HUFFMAN_H
