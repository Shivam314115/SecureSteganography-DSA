#include "../include/ecc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
    uint8_t data[] = "hello";
    size_t data_len = strlen((char *)data);
    int repetition_count = 3;
    size_t encoded_len = data_len * repetition_count;
    uint8_t encoded_data[encoded_len];
    uint8_t decoded_data[data_len + 1];

    ecc_encode(data, data_len, encoded_data, repetition_count);

    // Introduce some errors
    encoded_data[1] = 'a';
    encoded_data[7] = 'b';

    ecc_decode(encoded_data, encoded_len, decoded_data, repetition_count);
    decoded_data[data_len] = '\0';

    printf("Original data: %s\n", data);
    printf("Decoded data: %s\n", decoded_data);

    assert(strcmp((char *)data, (char *)decoded_data) == 0);
    printf("ECC test passed!\n");

    return 0;
}
