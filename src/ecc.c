#include "../include/ecc.h"

void ecc_encode(const uint8_t *data, size_t data_len, uint8_t *encoded_data, int repetition_count) {
    for (size_t i = 0; i < data_len; i++) {
        for (int j = 0; j < repetition_count; j++) {
            encoded_data[i * repetition_count + j] = data[i];
        }
    }
}

void ecc_decode(const uint8_t *encoded_data, size_t encoded_len, uint8_t *decoded_data, int repetition_count) {
    for (size_t i = 0; i < encoded_len / repetition_count; i++) {
        int counts[256] = {0};
        for (int j = 0; j < repetition_count; j++) {
            counts[encoded_data[i * repetition_count + j]]++;
        }

        int max_count = 0;
        uint8_t majority_char = 0;
        for (int k = 0; k < 256; k++) {
            if (counts[k] > max_count) {
                max_count = counts[k];
                majority_char = (uint8_t)k;
            }
        }
        decoded_data[i] = majority_char;
    }
}
