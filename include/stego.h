#ifndef STEGO_H
#define STEGO_H

#include <stdint.h>
#include <stddef.h>

// Metadata structure for steganography header
typedef struct {
    uint32_t message_length;     // Length of secret message in bytes
    uint32_t prng_seed;          // Seed for pseudo-random coefficient selection
    uint8_t ecc_type;            // Type of ECC used (0=none, 1=repetition, 2=Reed-Solomon)
    uint8_t ecc_n;               // ECC total symbols (for Reed-Solomon)
    uint8_t ecc_k;               // ECC message symbols (for Reed-Solomon)
    uint8_t reserved;            // Reserved for future use
} StegoHeader;

// Structure to hold embedding/extraction parameters
typedef struct {
    char *cover_image_path;      // Path to cover image (for embedding)
    char *stego_image_path;      // Path to output/input stego image
    char *secret_message;        // Secret message to embed
    size_t message_length;       // Length of secret message
    uint32_t key;                // Secret key for PRNG seed
    uint8_t ecc_type;            // ECC type to use
    uint8_t redundancy_level;    // ECC redundancy level (e.g., repetition count)
} StegoParams;

/**
 * Embed a secret message into a cover image.
 * 
 * @param params Pointer to StegoParams containing all embedding parameters.
 * @return 0 on success, -1 on failure.
 */
int embedMessage(const StegoParams *params);

/**
 * Extract a hidden message from a stego image.
 * 
 * @param stego_image_path Path to the stego image.
 * @param key Secret key used during embedding (for PRNG seed).
 * @param output_message Buffer to store extracted message (caller must allocate).
 * @param max_length Maximum length of output buffer.
 * @return Number of bytes extracted on success, -1 on failure.
 */
int extractMessage(const char *stego_image_path, uint32_t key, 
                   char *output_message, size_t max_length);

/**
 * Estimate embedding capacity of an image in bytes.
 * 
 * @param image_path Path to the cover image.
 * @return Estimated capacity in bytes, -1 on error.
 */
int estimateCapacity(const char *image_path);

/**
 * Create and embed a resilient header containing metadata.
 * 
 * @param header Pointer to StegoHeader structure.
 * @param coefficients Pointer to DCT coefficient array.
 * @param num_coeffs Total number of coefficients available.
 * @return 0 on success, -1 on failure.
 */
int embedHeader(const StegoHeader *header, double *coefficients, size_t num_coeffs);

/**
 * Extract and verify header from stego image.
 * 
 * @param coefficients Pointer to DCT coefficient array.
 * @param num_coeffs Total number of coefficients.
 * @param header Pointer to StegoHeader structure to fill.
 * @return 0 on success, -1 on failure or header verification failed.
 */
int extractHeader(const double *coefficients, size_t num_coeffs, StegoHeader *header);

#endif // STEGO_H