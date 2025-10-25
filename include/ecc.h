#ifndef ECC_H
#define ECC_H

#include <stdint.h>
#include <stddef.h>

// Encode data with a simple repetition code
void ecc_encode(const uint8_t *data, size_t data_len, uint8_t *encoded_data, int repetition_count);

// Decode data with a simple repetition code
void ecc_decode(const uint8_t *encoded_data, size_t encoded_len, uint8_t *decoded_data, int repetition_count);

#endif // ECC_H
