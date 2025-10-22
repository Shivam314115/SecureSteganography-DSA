#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>   // for uint8_t, uint32_t
#include <stdbool.h>  // for bool type
#include <stdio.h>    // for debugging prints

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

#endif