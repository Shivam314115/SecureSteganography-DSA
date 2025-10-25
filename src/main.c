#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stego.h"
#include "../include/utils.h"

void printMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║  Secure Steganography - DSA Project               ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("1. Embed message into image\n");
    printf("2. Extract message from image\n");
    printf("3. Calculate image capacity\n");
    printf("4. Test BitStream and PRNG\n");
    printf("5. Exit\n");
    printf("\nEnter your choice: ");
}

void testBitStreamAndPRNG() {
    printf("\n=== Testing BitStream ===\n");
    
    // Test bitstream
    uint8_t buffer[4];
    BitStream bs;
    bitstream_init(&bs, buffer, 32);
    
    printf("Writing bits: 1 0 1 1 0 1 0 0\n");
    bitstream_write_bit(&bs, 1);
    bitstream_write_bit(&bs, 0);
    bitstream_write_bit(&bs, 1);
    bitstream_write_bit(&bs, 1);
    bitstream_write_bit(&bs, 0);
    bitstream_write_bit(&bs, 1);
    bitstream_write_bit(&bs, 0);
    bitstream_write_bit(&bs, 0);
    
    bitstream_reset(&bs);
    printf("Reading bits: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", bitstream_read_bit(&bs));
    }
    printf("\n");
    
    // Test PRNG
    printf("\n=== Testing PRNG ===\n");
    PRNG prng;
    uint32_t seed = 12345;
    prng_init(&prng, seed);
    
    printf("Seed: %u\n", seed);
    printf("First 10 random numbers:\n");
    for (int i = 0; i < 10; i++) {
        printf("%u ", prng_next(&prng));
    }
    printf("\n");
    
    // Test deterministic behavior
    prng_init(&prng, seed);
    printf("\nRe-initializing with same seed - should get same sequence:\n");
    for (int i = 0; i < 10; i++) {
        printf("%u ", prng_next(&prng));
    }
    printf("\n");
}

void embedMessageInteractive() {
    char cover_path[256];
    char stego_path[256];
    char message[1024];
    uint32_t key;
    
    printf("\n=== Embed Message ===\n");
    printf("Enter cover image path: ");
    scanf("%255s", cover_path);
    
    printf("Enter output stego image path: ");
    scanf("%255s", stego_path);
    
    printf("Enter secret message: ");
    getchar(); // consume newline
    fgets(message, sizeof(message), stdin);
    // Remove trailing newline
    message[strcspn(message, "\n")] = 0;
    
    printf("Enter encryption key (number): ");
    scanf("%u", &key);
    
    // Set up ECC parameters
    ECCParams ecc;
    ecc.type = 0;           // Simple repetition for now
    ecc.redundancy = 3;     // 3x redundancy
    ecc.n = 0;
    ecc.k = 0;
    
    // Embed the message
    int result = embedMessage(
        cover_path,
        stego_path,
        (uint8_t*)message,
        strlen(message),
        key
    );
    
    if (result == 0) {
        printf("\n✓ Message embedded successfully!\n");
        printf("Stego image saved to: %s\n", stego_path);
    } else {
        printf("\n✗ Failed to embed message (error code: %d)\n", result);
    }
}

void extractMessageInteractive() {
    char stego_path[256];
    char extracted[1024];
    uint32_t key;
    
    printf("\n=== Extract Message ===\n");
    printf("Enter stego image path: ");
    scanf("%255s", stego_path);
    
    printf("Enter decryption key (number): ");
    scanf("%u", &key);
    
    // Extract the message
    int bytes_extracted = extractMessage(
        stego_path,
        (uint8_t*)extracted,
        sizeof(extracted) - 1,
        key
    );
    
    if (bytes_extracted > 0) {
        extracted[bytes_extracted] = '\0';
        printf("\n✓ Message extracted successfully!\n");
        printf("Extracted message: %s\n", extracted);
        printf("Length: %d bytes\n", bytes_extracted);
    } else {
        printf("\n✗ Failed to extract message (error code: %d)\n", bytes_extracted);
    }
}

void calculateCapacityInteractive() {
    char image_path[256];
    
    printf("\n=== Calculate Capacity ===\n");
    printf("Enter image path: ");
    scanf("%255s", image_path);
    
    ECCParams ecc;
    ecc.type = 0;
    ecc.redundancy = 3;
    ecc.n = 0;
    ecc.k = 0;
    
    int capacity = computeCapacity(image_path, ecc);
    
    if (capacity > 0) {
        printf("\nImage capacity: %d bytes\n", capacity);
        printf("That's approximately %d characters of text\n", capacity);
    } else {
        printf("\n✗ Failed to calculate capacity\n");
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║                                                    ║\n");
    printf("║       SECURE STEGANOGRAPHY WITH DSA                ║\n");
    printf("║                                                    ║\n");
    printf("║  Using: DCT Transform + ECC + Priority Queue       ║\n");
    printf("║  Data Structures: BitStream, PRNG, Heap            ║\n");
    printf("║                                                    ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    
    int choice;
    
    while (1) {
        printMenu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                embedMessageInteractive();
                break;
            case 2:
                extractMessageInteractive();
                break;
            case 3:
                calculateCapacityInteractive();
                break;
            case 4:
                testBitStreamAndPRNG();
                break;
            case 5:
                printf("\nThank you for using Secure Steganography!\n");
                return 0;
            default:
                printf("\nInvalid choice. Please select 1-5.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar(); // consume newline
        getchar(); // wait for Enter
    }
    
    return 0;
}
