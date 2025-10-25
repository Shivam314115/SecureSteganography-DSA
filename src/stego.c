#include "../include/stego.h"
#include "../include/image.h"
#include "../include/utils.h"
#include "../include/huffman.h"
#include <stdlib.h>
#include <string.h>

int embedMessage(const StegoParams *params) {
    Image cover_image;
    if (load_bmp(params->cover_image_path, &cover_image) != 0) {
        return -1;
    }

    // Calculate frequency of characters
    unsigned freq[256] = {0};
    for (int i = 0; params->secret_message[i] != '\0'; i++) {
        freq[(unsigned char)params->secret_message[i]]++;
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

    // Build Huffman tree
    HuffmanNode* root = build_huffman_tree(data, freq_data, size);

    // Generate Huffman codes
    int codes[256][MAX_TREE_HT] = { {0} };
    generate_codes(root, codes);

    // Calculate the size of the encoded message
    int encoded_size = 0;
    for (int i = 0; params->secret_message[i] != '\0'; i++) {
        encoded_size += codes[(unsigned char)params->secret_message[i]][0];
    }

    // Encode the message
    char *encoded_message = (char *)malloc(encoded_size + 1);
    huffman_encode(params->secret_message, codes, encoded_message);
    encoded_message[encoded_size] = '\0';

    // Embed the header
    int data_index = 0;
    cover_image.data[data_index++] = (uint8_t)size;
    for (int i = 0; i < size; i++) {
        cover_image.data[data_index++] = data[i];
        cover_image.data[data_index++] = freq_data[i];
    }
    *(uint32_t *)&cover_image.data[data_index] = encoded_size;
    data_index += 4;

    // Embed the encoded message
    for (int i = 0; i < encoded_size; i++) {
        set_bit(&cover_image.data[data_index], 0, encoded_message[i] - '0');
        data_index++;
    }

    if (save_bmp(params->stego_image_path, &cover_image) != 0) {
        free_image(&cover_image);
        return -1;
    }

    free(encoded_message);
    free_image(&cover_image);

    return 0;
}

int extractMessage(const char *stego_image_path, char *output_message, size_t max_length) {
    Image stego_image;
    if (load_bmp(stego_image_path, &stego_image) != 0) {
        return -1;
    }

    // Extract the header
    int size = stego_image.data[0];
    int data_index = 1;
    char data[size];
    int freq_data[size];
    for (int i = 0; i < size; i++) {
        data[i] = stego_image.data[data_index++];
        freq_data[i] = stego_image.data[data_index++];
    }
    uint32_t encoded_size = *(uint32_t *)&stego_image.data[data_index];
    data_index += 4;

    // Rebuild Huffman tree
    HuffmanNode* root = build_huffman_tree(data, freq_data, size);

    // Extract the encoded message
    char *encoded_message = (char *)malloc(encoded_size + 1);
    for (uint32_t i = 0; i < encoded_size; i++) {
        encoded_message[i] = get_bit(stego_image.data[data_index++], 0) + '0';
    }
    encoded_message[encoded_size] = '\0';

    // Decode the message
    huffman_decode(encoded_message, output_message, root);

    free(encoded_message);
    free_image(&stego_image);

    return strlen(output_message);
}