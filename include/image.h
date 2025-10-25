#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

// Structure representing a simple image in memory
typedef struct {
    uint8_t *data;      // Pixel data stored row-major
    int width;          // Image width in pixels
    int height;         // Image height in pixels
    int channels;       // Number of color channels (1 = grayscale, 3 = RGB)
} Image;

/**
 * @brief Load an uncompressed 24-bit BMP image from file.
 * @param filename Path to the BMP file
 * @param image Pointer to an Image struct to be populated
 * @return 0 on success, negative on failure
 */
int load_bmp(const char *filename, Image *image);

/**
 * @brief Save an Image struct as an uncompressed 24-bit BMP file.
 * @param filename Output BMP file path
 * @param image Pointer to the Image struct
 * @return 0 on success, negative on failure
 */
int save_bmp(const char *filename, const Image *image);

/**
 * @brief Free the pixel data buffer in the Image struct.
 * @param image Pointer to the Image struct
 */
void free_image(Image *image);

#endif // IMAGE_H