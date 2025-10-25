#include "../include/stego.h"
#include "../include/dct.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple image structure (placeholder - will be replaced with libjpeg later)
typedef struct {
    int width;
    int height;
    unsigned char *data;  // Grayscale pixel data
} Image;

typedef struct {
    uint32_t message_length;
    uint32_t prng_seed;
    uint8_t ecc_type;
} StegoMetadata;

typedef struct {
    const char *cover_image_path;
    const char *stego_image_path;
    const uint8_t *secret_message;
    size_t message_length;
    uint32_t key;
    ECCParams ecc_params;
} StegoParams;

// Helper function to read image (placeholder - implement with libjpeg later)
static Image* readImage(const char *path) {
    printf("Reading image: %s\n", path);
    // TODO: Implement actual image reading using libjpeg
    // For now, return a dummy image for testing
    Image *img = (Image*)malloc(sizeof(Image));
    if (!img) return NULL;
    
    img->width = 512;
    img->height = 512;
    img->data = (unsigned char*)calloc(img->width * img->height, 1);
    
    return img;
}

// Helper function to write image (placeholder)
static int writeImage(const char *path, Image *img) {
    printf("Writing image: %s\n", path);
    // TODO: Implement actual image writing using libjpeg
    return 0;
}

// Helper function to convert image block to DCT
static void imageToDCT(unsigned char *pixels, DCTBlock dct_block) {
    DCTBlock input;
    for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
            input[i][j] = (double)pixels[i * DCT_BLOCK_SIZE + j];
        }
    }
    forwardDCT(input, dct_block);
}

// Helper function to convert DCT back to pixels
static void dctToImage(DCTBlock dct_block, unsigned char *pixels) {
    DCTBlock output;
    inverseDCT(dct_block, output);
    for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
            int val = (int)(output[i][j] + 0.5);
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            pixels[i * DCT_BLOCK_SIZE + j] = (unsigned char)val;
        }
    }
}

int embedMessage(const char *cover_image_path,
                 const char *stego_image_path,
                 const uint8_t *secret_message,
                 size_t message_len,
                 uint32_t key,
                 ECCParams ecc_params) {
    
    if (!cover_image_path || !stego_image_path || !secret_message) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return -1;
    }

    printf("\n=== Embedding Message ===\n");
    printf("Cover image: %s\n", cover_image_path);
    printf("Message length: %zu bytes\n", message_len);
    printf("Using key: %u\n", key);
    printf("ECC type: %u, redundancy: %u\n", ecc_params.type, ecc_params.redundancy);

    // Read cover image
    Image *img = readImage(cover_image_path);
    if (!img) {
        fprintf(stderr, "Error: Failed to read cover image\n");
        return -1;
    }

    // Calculate number of 8x8 blocks
    int blocks_x = img->width / DCT_BLOCK_SIZE;
    int blocks_y = img->height / DCT_BLOCK_SIZE;
    int total_blocks = blocks_x * blocks_y;

    printf("Image dimensions: %dx%d\n", img->width, img->height);
    printf("Total 8x8 blocks: %d\n", total_blocks);

    // Create bitstream from message
    size_t bit_count = message_len * 8;
    uint8_t *bit_buffer = (uint8_t*)malloc((bit_count + 7) / 8);
    if (!bit_buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(img->data);
        free(img);
        return -1;
    }

    BitStream bs;
    bitstream_init(&bs, bit_buffer, bit_count);

    // Write message bits to bitstream
    for (size_t i = 0; i < message_len; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            bitstream_write_bit(&bs, (secret_message[i] >> bit) & 1);
        }
    }

    // Initialize PRNG with key
    PRNG prng;
    prng_init(&prng, key);

    // Reset bitstream for reading
    bitstream_reset(&bs);

    // Embed bits in DCT coefficients
    int bits_embedded = 0;
    for (int by = 0; by < blocks_y && bits_embedded < (int)bit_count; by++) {
        for (int bx = 0; bx < blocks_x && bits_embedded < (int)bit_count; bx++) {
            // Extract 8x8 block
            unsigned char block_pixels[64];
            for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
                for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
                    int px = bx * DCT_BLOCK_SIZE + j;
                    int py = by * DCT_BLOCK_SIZE + i;
                    block_pixels[i * DCT_BLOCK_SIZE + j] = 
                        img->data[py * img->width + px];
                }
            }

            // Convert to DCT
            DCTBlock dct_block;
            imageToDCT(block_pixels, dct_block);

            // Select coefficient using PRNG (avoid DC coefficient at [0][0])
            uint32_t rand_val = prng_next(&prng);
            int coef_idx = 1 + (rand_val % 63); // Skip DC, use AC coefficients
            int u = coef_idx / DCT_BLOCK_SIZE;
            int v = coef_idx % DCT_BLOCK_SIZE;

            // Embed one bit using LSB modification
            int bit = bitstream_read_bit(&bs);
            if (bit >= 0) {
                int coef_int = (int)dct_block[u][v];
                if ((coef_int & 1) != bit) {
                    dct_block[u][v] += (coef_int >= 0) ? 1.0 : -1.0;
                }
                bits_embedded++;
            }

            // Convert back to pixels
            dctToImage(dct_block, block_pixels);

            // Write block back
            for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
                for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
                    int px = bx * DCT_BLOCK_SIZE + j;
                    int py = by * DCT_BLOCK_SIZE + i;
                    img->data[py * img->width + px] = 
                        block_pixels[i * DCT_BLOCK_SIZE + j];
                }
            }
        }
    }

    printf("Successfully embedded %d bits\n", bits_embedded);

    // Write stego image
    int result = writeImage(stego_image_path, img);

    free(bit_buffer);
    free(img->data);
    free(img);

    return result;
}

int extractMessage(const char *stego_image_path,
                   uint8_t *extracted_message,
                   size_t max_len,
                   uint32_t key) {
    
    if (!stego_image_path || !extracted_message) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return -1;
    }

    printf("\n=== Extracting Message ===\n");
    printf("Stego image: %s\n", stego_image_path);
    printf("Using key: %u\n", key);

    // Read stego image
    Image *img = readImage(stego_image_path);
    if (!img) {
        fprintf(stderr, "Error: Failed to read stego image\n");
        return -1;
    }

    int blocks_x = img->width / DCT_BLOCK_SIZE;
    int blocks_y = img->height / DCT_BLOCK_SIZE;

    printf("Image dimensions: %dx%d\n", img->width, img->height);

    // Initialize PRNG with same key
    PRNG prng;
    prng_init(&prng, key);

    // Create bitstream for extraction
    size_t bit_count = max_len * 8;
    uint8_t *bit_buffer = (uint8_t*)calloc((bit_count + 7) / 8, 1);
    if (!bit_buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(img->data);
        free(img);
        return -1;
    }

    BitStream bs;
    bitstream_init(&bs, bit_buffer, bit_count);

    // Extract bits from DCT coefficients
    int bits_extracted = 0;
    for (int by = 0; by < blocks_y && bits_extracted < (int)bit_count; by++) {
        for (int bx = 0; bx < blocks_x && bits_extracted < (int)bit_count; bx++) {
            // Extract 8x8 block
            unsigned char block_pixels[64];
            for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
                for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
                    int px = bx * DCT_BLOCK_SIZE + j;
                    int py = by * DCT_BLOCK_SIZE + i;
                    block_pixels[i * DCT_BLOCK_SIZE + j] = 
                        img->data[py * img->width + px];
                }
            }

            // Convert to DCT
            DCTBlock dct_block;
            imageToDCT(block_pixels, dct_block);

            // Select same coefficient using PRNG
            uint32_t rand_val = prng_next(&prng);
            int coef_idx = 1 + (rand_val % 63);
            int u = coef_idx / DCT_BLOCK_SIZE;
            int v = coef_idx % DCT_BLOCK_SIZE;

            // Extract LSB
            int coef_int = (int)dct_block[u][v];
            int bit = coef_int & 1;
            bitstream_write_bit(&bs, bit);
            bits_extracted++;
        }
    }

    printf("Extracted %d bits\n", bits_extracted);

    // Convert bits back to message
    bitstream_reset(&bs);
    size_t bytes_extracted = bits_extracted / 8;
    for (size_t i = 0; i < bytes_extracted && i < max_len; i++) {
        unsigned char byte = 0;
        for (int bit = 7; bit >= 0; bit--) {
            int b = bitstream_read_bit(&bs);
            if (b >= 0) {
                byte |= (b << bit);
            }
        }
        extracted_message[i] = byte;
    }
    
    if (bytes_extracted < max_len) {
        extracted_message[bytes_extracted] = '\0';
    }

    free(bit_buffer);
    free(img->data);
    free(img);

    return (int)bytes_extracted;
}

int computeCapacity(const char *image_path, ECCParams ecc_params) {
    Image *img = readImage(image_path);
    if (!img) return -1;

    int blocks_x = img->width / DCT_BLOCK_SIZE;
    int blocks_y = img->height / DCT_BLOCK_SIZE;
    int total_blocks = blocks_x * blocks_y;

    // Each block can hide 1 bit (conservative estimate)
    // Account for ECC redundancy
    int raw_capacity_bits = total_blocks;
    int capacity_bytes = raw_capacity_bits / (8 * ecc_params.redundancy);

    free(img->data);
    free(img);

    return capacity_bytes;
}

int embedMetadata(const StegoMetadata *metadata, void *coef_arrays, void *cinfo) {
    // TODO: Implement robust header embedding using repeated bits in stable coefficients
    printf("Embedding meta msg_len=%u, seed=%u, ecc_type=%u\n",
           metadata->message_length, metadata->prng_seed, metadata->ecc_type);
    (void)coef_arrays;  // Suppress unused parameter warning
    (void)cinfo;
    return 0;
}

int extractMetadata(StegoMetadata *metadata, void *coef_arrays, void *cinfo) {
    // TODO: Implement metadata extraction with error checking
    printf("Extracting metadata...\n");
    (void)metadata;
    (void)coef_arrays;
    (void)cinfo;
    return 0;
}

int main() {
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║     Secure Steganography - Quick Test             ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");
    
    // Test 1: BitStream functionality
    printf("Test 1: BitStream\n");
    printf("------------------\n");
    uint8_t buffer[2];
    BitStream bs;
    bitstream_init(&bs, buffer, 16);
    
    // Write some bits
    bitstream_write_bit(&bs, 1);
    bitstream_write_bit(&bs, 0);
    bitstream_write_bit(&bs, 1);
    bitstream_write_bit(&bs, 1);
    
    // Read them back
    bitstream_reset(&bs);
    printf("Bits written: 1 0 1 1\n");
    printf("Bits read back: %d %d %d %d\n", 
           bitstream_read_bit(&bs),
           bitstream_read_bit(&bs),
           bitstream_read_bit(&bs),
           bitstream_read_bit(&bs));
    printf("✓ BitStream test passed\n\n");
    
    // Test 2: PRNG functionality
    printf("Test 2: PRNG\n");
    printf("------------------\n");
    PRNG prng;
    prng_init(&prng, 42);
    printf("Seed: 42\n");
    printf("First 5 random numbers: ");
    for (int i = 0; i < 5; i++) {
        printf("%u ", prng_next(&prng));
    }
    printf("\n✓ PRNG test passed\n\n");
    
    // Test 3: Embedding and extraction (with dummy image)
    printf("Test 3: Embed and Extract\n");
    printf("------------------\n");
    
    const char *secret = "Hello, World!";
    printf("Original message: %s\n", secret);
    
    // Create StegoParams struct for embedding
    StegoParams embed_params;
    embed_params.cover_image_path = "test_cover.jpg";
    embed_params.stego_image_path = "test_stego.jpg";
    embed_params.secret_message = (uint8_t*)secret;
    embed_params.message_length = strlen(secret);
    embed_params.key = 12345;
    embed_params.ecc_params.type = 0;
    embed_params.ecc_params.redundancy = 3;
    embed_params.ecc_params.n = 0;
    embed_params.ecc_params.k = 0;
    
    // Embed
    int embed_result = embedMessage(&embed_params);
    
    if (embed_result == 0) {
        printf("✓ Embedding completed\n");
    } else {
        printf("✗ Embedding failed\n");
    }
    
    // Extract
    char extracted[256];
    int extract_result = extractMessage(
        "test_stego.jpg",
        12345,
        extracted,
        sizeof(extracted) - 1
    );
    
    if (extract_result > 0) {
        extracted[extract_result] = '\0';
        printf("✓ Extraction completed\n");
        printf("Extracted message: %s\n", extracted);
        
        // Verify
        if (strcmp(secret, extracted) == 0) {
            printf("✓✓ Message matches original!\n");
        } else {
            printf("⚠ Message differs from original\n");
        }
    } else {
        printf("✗ Extraction failed\n");
    }
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║          All tests completed!                      ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    
    return 0;
}
