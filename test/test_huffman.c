#include "../include/huffman.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_SIZE 100

int main() {
    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
    int size = sizeof(arr) / sizeof(arr[0]);

    HuffmanNode* root = build_huffman_tree(arr, freq, size);

    int codes[256][MAX_TREE_HT] = { {0} };
    generate_codes(root, codes);

    char message[] = "abcdef";
    char encoded_message[MAX_SIZE];
    char decoded_message[MAX_SIZE];

    huffman_encode(message, codes, encoded_message);
    printf("Encoded message: %s\n", encoded_message);

    huffman_decode(encoded_message, decoded_message, root);
    printf("Decoded message: %s\n", decoded_message);

    assert(strcmp(message, decoded_message) == 0);
    printf("Huffman encode/decode test passed!\n");

    return 0;
}
