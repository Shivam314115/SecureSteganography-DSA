#include "../include/utils.h"
#include <string.h>

/**
 * @brief Get a bit (0 or 1) from a byte.
 * 
 * @param byte The input byte.
 * @param position Bit position (0 = LSB, 7 = MSB).
 * @return The bit value (0 or 1).
 */
uint8_t get_bit(uint8_t byte, int position) {
    return (byte >> position) & 1;
}

/**
 * @brief Set a specific bit in a byte to 0 or 1.
 * 
 * @param byte Pointer to the byte.
 * @param position Bit position (0 = LSB, 7 = MSB).
 * @param value Bit value (0 or 1).
 */
void set_bit(uint8_t *byte, int position, uint8_t value) {
    if (value)
        *byte |= (1 << position);
    else
        *byte &= ~(1 << position);
}

/**
 * @brief Convert a character to its binary (8-bit) representation.
 * 
 * @param c The input character.
 * @param binary Output array of 8 bits.
 */
void char_to_binary(uint8_t c, uint8_t binary[8]) {
    for (int i = 0; i < 8; i++)
        binary[7 - i] = (c >> i) & 1;  // store MSB first
}

/**
 * @brief Convert an 8-bit binary array back to a character.
 * 
 * @param binary Array of 8 bits.
 * @return The reconstructed character.
 */
uint8_t binary_to_char(uint8_t binary[8]) {
    uint8_t c = 0;
    for (int i = 0; i < 8; i++)
        c = (c << 1) | binary[i];
    return c;
}

/**
 * @brief Simple XOR-based encryption/decryption.
 * 
 * @param data Input byte.
 * @param key XOR key.
 * @return Encrypted or decrypted byte.
 */
uint8_t xor_encrypt(uint8_t data, uint8_t key) {
    return data ^ key;
}

/**
 * @brief Convert text to a sequence of binary bits.
 * 
 * @param text Input string.
 * @param binary Output bit array.
 * @param length Length of input text.
 */
void text_to_binary(const char *text, uint8_t *binary, int length) {
    for (int i = 0; i < length; i++) {
        uint8_t bits[8];
        char_to_binary((uint8_t)text[i], bits);
        for (int j = 0; j < 8; j++) {
            binary[i * 8 + j] = bits[j];
        }
    }
}

/**
 * @brief Convert a sequence of binary bits back into readable text.
 * 
 * @param binary Input bit array.
 * @param text Output string.
 * @param length Number of characters to reconstruct.
 */
void binary_to_text(const uint8_t *binary, char *text, int length) {
    for (int i = 0; i < length; i++) {
        uint8_t bits[8];
        for (int j = 0; j < 8; j++) {
            bits[j] = binary[i * 8 + j];
        }
        text[i] = (char)binary_to_char(bits);
    }
    text[length] = '\0';  // Null-terminate string
}

/**
 * @brief Print binary representation of a byte (for debugging).
 * 
 * @param byte Input byte.
 */
void print_byte(uint8_t byte) {
    for (int i = 7; i >= 0; i--)
        printf("%d", get_bit(byte, i));
    printf(" ");
}
int main() {
    uint8_t byte = 0xAB;
    print_byte(byte);
    return 0;
}
