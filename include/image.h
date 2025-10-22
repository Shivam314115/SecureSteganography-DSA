#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stddef.h>

// Structure representing a simple image in memory
typedef struct {
    uint8_t *data;      // Pixel data (contiguous row-major bytes)
    int width;          // Image width in pixels
    int height;         // Image height in pixels
    int channels;       // Channels per pixel (e.g., 1 = grayscale, 3 = RGB)
} Image;

// Load a BMP image (uncompressed, 24-bit or 8-bit grayscale)
int load_bmp(const char *filename, Image *image);

// Save a BMP image
int save_bmp(const char *filename, const Image *image);

// Free image memory
void free_image(Image *image);

// --- JPEG interface (if using libjpeg) ---
// Load JPEG image data into an Image struct
int load_jpeg(const char *filename, Image *image);

// Save Image struct as JPEG
int save_jpeg(const char *filename, const Image *image, int quality);

// Optionally: direct access to JPEG's DCT coefficients (advanced/with libjpeg)
// int load_jpeg_dct(const char *filename, ...);
// int save_jpeg_dct(const char *filename, ...);

#endif // IMAGE_H
