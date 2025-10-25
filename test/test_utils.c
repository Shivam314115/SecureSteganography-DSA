#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_bit_functions() {
    uint8_t byte = 0;
    set_bit(&byte, 0, 1);
    assert(get_bit(byte, 0) == 1);
    set_bit(&byte, 0, 0);
    assert(get_bit(byte, 0) == 0);
    set_bit(&byte, 7, 1);
    assert(get_bit(byte, 7) == 1);
    printf("test_bit_functions passed.\n");
}

void test_char_binary_conversion() {
    uint8_t binary[8];
    char_to_binary('A', binary);
    assert(binary_to_char(binary) == 'A');
    char_to_binary('z', binary);
    assert(binary_to_char(binary) == 'z');
    printf("test_char_binary_conversion passed.\n");
}

void test_text_binary_conversion() {
    char text[] = "Hello";
    int len = strlen(text);
    uint8_t binary[len * 8];
    char recovered[len + 1];
    text_to_binary(text, binary, len);
    binary_to_text(binary, recovered, len);
    assert(strcmp(text, recovered) == 0);
    printf("test_text_binary_conversion passed.\n");
}

void test_xor_encryption() {
    uint8_t data = 'A';
    uint8_t key = 0xAA;
    uint8_t encrypted = xor_encrypt(data, key);
    uint8_t decrypted = xor_encrypt(encrypted, key);
    assert(data == decrypted);
    printf("test_xor_encryption passed.\n");
}

int main() {
    test_bit_functions();
    test_char_binary_conversion();
    test_text_binary_conversion();
    test_xor_encryption();
    printf("All tests passed!\n");
    return 0;
}