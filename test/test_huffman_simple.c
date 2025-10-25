#include "../include/huffman.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
    char message[] = "this is a test";
    unsigned freq[256] = {0};
    for (int i = 0; message[i] != '\0'; i++) {
        freq[(unsigned char)message[i]]++;
    }

    char data[256];
    int freq_data[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            freq_data[size] = freq[i];
            size++;
        }
    }

    HuffmanNode* root = build_huffman_tree(data, freq_data, size);

    int codes[256][MAX_TREE_HT] = { {0} };
    generate_codes(root, codes);

    char encoded_message[1000];
    huffman_encode(message, codes, encoded_message);

    char decoded_message[1000];
    huffman_decode(encoded_message, decoded_message, root);

    printf("Original message: %s\n", message);
    printf("Encoded message: %s\n", encoded_message);
    printf("Decoded message: %s\n", decoded_message);

    assert(strcmp(message, decoded_message) == 0);
    printf("Huffman simple test passed!\n");

    return 0;
}
