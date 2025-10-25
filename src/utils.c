#include "../include/utils.h"
#include <string.h>

// BitStream functions
void bitstream_init_writer(BitStream *bs, uint8_t *buffer, size_t bit_count) {
    bs->data = buffer;
    bs->bit_capacity = bit_count;
    bs->bit_position = 0;
    memset(bs->data, 0, (bit_count + 7) / 8);
}

void bitstream_init_reader(BitStream *bs, uint8_t *buffer, size_t bit_count) {
    bs->data = buffer;
    bs->bit_capacity = bit_count;
    bs->bit_position = 0;
}

int bitstream_write_bit(BitStream *bs, int bit) {
    if (bs->bit_position >= bs->bit_capacity) return -1;
    size_t byte_index = bs->bit_position / 8;
    int bit_offset = 7 - (bs->bit_position % 8);
    if (bit) {
        bs->data[byte_index] |= (1 << bit_offset);
    } else {
        bs->data[byte_index] &= ~(1 << bit_offset);
    }
    bs->bit_position++;
    return 0;
}

int bitstream_read_bit(BitStream *bs) {
    if (bs->bit_position >= bs->bit_capacity) return -1;
    size_t byte_index = bs->bit_position / 8;
    int bit_offset = 7 - (bs->bit_position % 8);
    int bit = (bs->data[byte_index] >> bit_offset) & 1;
    bs->bit_position++;
    return bit;
}

void bitstream_reset(BitStream *bs) {
    bs->bit_position = 0;
}

// PRNG functions
void prng_init(PRNG *prng, uint32_t seed) {
    prng->seed = seed;
    prng->state = seed;
}

uint32_t prng_next(PRNG *prng) {
    prng->state = (prng->state * 1103515245 + 12345) & 0x7FFFFFFF;
    return prng->state;
}

// Original utility functions

uint8_t get_bit(uint8_t byte, int position) {
    return (byte >> position) & 1;
}

void set_bit(uint8_t *byte, int position, uint8_t value) {
    if (value) {
        *byte |= (1 << position);
    } else {
        *byte &= ~(1 << position);
    }
}

void char_to_binary(uint8_t c, uint8_t binary[8]) {
    for (int i = 0; i < 8; i++) {
        binary[i] = get_bit(c, 7 - i);
    }
}

uint8_t binary_to_char(uint8_t binary[8]) {
    uint8_t c = 0;
    for (int i = 0; i < 8; i++) {
        set_bit(&c, 7 - i, binary[i]);
    }
    return c;
}

uint8_t xor_encrypt(uint8_t data, uint8_t key) {
    return data ^ key;
}

void text_to_binary(const char *text, uint8_t *binary, int length) {
    for (int i = 0; i < length; i++) {
        char_to_binary(text[i], &binary[i * 8]);
    }
}

void binary_to_text(const uint8_t *binary, char *text, int length) {
    for (int i = 0; i < length; i++) {
        text[i] = binary_to_char((uint8_t *)&binary[i * 8]);
    }
    text[length] = '\0';
}

void print_byte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", get_bit(byte, i));
    }
}

uint32_t compute_checksum(const uint8_t *data, size_t length) {
    uint32_t checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}

void byte_to_binary(uint8_t byte, char *output) {
    for (int i = 7; i >= 0; i--) {
        output[7 - i] = ((byte >> i) & 1) ? '1' : '0';
    }
    output[8] = '\0';
}