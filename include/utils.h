#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>   // for uint8_t, uint32_t
#include <stdbool.h>  // for bool type
#include <stdio.h>    // for debugging prints
#include <stddef.h>   // for size_t

// ============================================
// BitStream structure for bit-level operations
// ============================================
typedef struct {
    uint8_t *data;          // Buffer holding bits
    size_t bit_capacity;    // Total capacity in bits
    size_t bit_position;    // Current read/write position
} BitStream;

/**
 * Initialize a bitstream with a buffer.
 * 
 * @param bs Pointer to BitStream structure.
 * @param buffer Pointer to byte buffer.
 * @param bit_count Total capacity in bits.
 */
void bitstream_init(BitStream *bs, uint8_t *buffer, size_t bit_count);

/**
 * Write a single bit to the bitstream.
 * 
 * @param bs Pointer to BitStream.
 * @param bit Bit value (0 or 1).
 * @return 0 on success, -1 if buffer full.
 */
int bitstream_write_bit(BitStream *bs, int bit);

/**
 * Read a single bit from the bitstream.
 * 
 * @param bs Pointer to BitStream.
 * @return Bit value (0 or 1), or -1 if end reached.
 */
int bitstream_read_bit(BitStream *bs);

/**
 * Reset bitstream position to beginning.
 * 
 * @param bs Pointer to BitStream.
 */
void bitstream_reset(BitStream *bs);

// ============================================
// Pseudo-Random Number Generator (PRNG)
// ============================================
typedef struct {
    uint32_t state;     // Current state
    uint32_t seed;      // Initial seed
} PRNG;

/**
 * Initialize PRNG with a seed.
 * 
 * @param prng Pointer to PRNG structure.
 * @param seed Initial seed value.
 */
void prng_init(PRNG *prng, uint32_t seed);

/**
 * Generate next pseudo-random number.
 * 
 * @param prng Pointer to PRNG structure.
 * @return Next pseudo-random 32-bit integer.
 */
uint32_t prng_next(PRNG *prng);

// ============================================
// Original utility functions
// ============================================

/**
 * @brief Get a bit (0 or 1) from a byte.
 * @param byte The input byte.
 * @param position The bit position (0 = LSB, 7 = MSB).
 * @return The bit value (0 or 1).
 */
uint8_t get_bit(uint8_t byte, int position);

/**
 * @brief Set a specific bit in a byte to 0 or 1.
 * @param byte Pointer to the byte to modify.
 * @param position Bit position to set (0 = LSB, 7 = MSB).
 * @param value The bit value (0 or 1).
 */
void set_bit(uint8_t *byte, int position, uint8_t value);

/**
 * @brief Convert a character to its binary representation (8 bits).
 * @param c The input character.
 * @param binary Array of size 8 to store the binary result.
 */
void char_to_binary(uint8_t c, uint8_t binary[8]);

/**
 * @brief Convert an 8-bit binary array back to a character.
 * @param binary Array of 8 bits.
 * @return The resulting character.
 */
uint8_t binary_to_char(uint8_t binary[8]);

/**
 * @brief Apply a simple XOR-based encryption/decryption on a byte.
 * @param data The input byte.
 * @param key The XOR key.
 * @return The encrypted or decrypted byte.
 */
uint8_t xor_encrypt(uint8_t data, uint8_t key);

/**
 * @brief Convert text to binary array (sequence of bits).
 * @param text The input string.
 * @param binary Output array to store bits.
 * @param length Length of the text.
 */
void text_to_binary(const char *text, uint8_t *binary, int length);

/**
 * @brief Convert binary array back to text.
 * @param binary Input bit array.
 * @param text Output string.
 * @param length Length of the text.
 */
void binary_to_text(const uint8_t *binary, char *text, int length);

/**
 * @brief Print binary representation of a byte (for debugging).
 * @param byte The input byte.
 */
void print_byte(uint8_t byte);

/**
 * @brief Compute simple checksum for data integrity.
 * @param data Pointer to data buffer.
 * @param length Length of data in bytes.
 * @return 32-bit checksum value.
 */
uint32_t compute_checksum(const uint8_t *data, size_t length);

/**
 * @brief Convert bytes to binary string representation (for debugging).
 * @param byte Input byte.
 * @param output Output string buffer (must be at least 9 bytes).
 */
void byte_to_binary(uint8_t byte, char *output);

#endif // UTILS_H
