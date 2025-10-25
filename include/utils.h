#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

// BitStream structure for bit-level operations
typedef struct {
    uint8_t *data;
    size_t bit_capacity;
    size_t bit_position;
} BitStream;

void bitstream_init_writer(BitStream *bs, uint8_t *buffer, size_t bit_count);
void bitstream_init_reader(BitStream *bs, uint8_t *buffer, size_t bit_count);
int bitstream_write_bit(BitStream *bs, int bit);
int bitstream_read_bit(BitStream *bs);
void bitstream_reset(BitStream *bs);

// Pseudo-Random Number Generator (PRNG)
typedef struct {
    uint32_t state;
    uint32_t seed;
} PRNG;

void prng_init(PRNG *prng, uint32_t seed);
uint32_t prng_next(PRNG *prng);

// Original utility functions
uint8_t get_bit(uint8_t byte, int position);
void set_bit(uint8_t *byte, int position, uint8_t value);
void char_to_binary(uint8_t c, uint8_t binary[8]);
uint8_t binary_to_char(uint8_t binary[8]);
uint8_t xor_encrypt(uint8_t data, uint8_t key);
void text_to_binary(const char *text, uint8_t *binary, int length);
void binary_to_text(const uint8_t *binary, char *text, int length);
void print_byte(uint8_t byte);
uint32_t compute_checksum(const uint8_t *data, size_t length);
void byte_to_binary(uint8_t byte, char *output);

#endif // UTILS_H