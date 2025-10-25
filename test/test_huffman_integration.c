#include "../include/stego.h"
#include "../include/huffman.h"
#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
    char secret_message[] = "This is a longer test message to see if the Huffman coding works correctly.";
    char extracted_message[100];

    StegoParams params = {
        .cover_image_path = "data/input.bmp",
        .stego_image_path = "data/stego.bmp",
        .secret_message = secret_message,
        .message_length = strlen(secret_message),
        .key = 12345
    };

    // Embed the message using Huffman coding
    int result = embedMessage(&params);
    assert(result == 0);
    printf("Message embedded successfully.\n");

    // Extract the message
    result = extractMessage(params.stego_image_path, extracted_message, sizeof(extracted_message));
    assert(result > 0);
    extracted_message[result] = '\0';
    printf("Message extracted: %s\n", extracted_message);

    assert(strcmp(secret_message, extracted_message) == 0);
    printf("Huffman integration test passed!\n");

    return 0;
}
