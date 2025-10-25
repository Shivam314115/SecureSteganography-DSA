#ifndef DCT_H
#define DCT_H

#include <stdint.h>
#include <stddef.h>

// Block size for JPEG (8x8)
#define DCT_BLOCK_SIZE 8
typedef double DCTBlock[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

/**
 * @brief Forward 2D DCT on a single 8x8 block of image data (in-place or output buffer).
 * @param block Input: pointer to 8x8 block (row-major), Output: transformed DCT coefficients
 */
void dct_forward(const double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE],
                 double output[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE]);

/**
 * @brief Inverse 2D DCT on a single 8x8 block of DCT coefficients.
 * @param block Input: pointer to 8x8 block of coefficients, Output: reconstructed 8x8 pixel values
 */
void dct_inverse(const double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE],
                 double output[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE]);

/**
 * @brief Perform forward DCT for all non-overlapping 8x8 blocks in an image.
 *        The image pixels should be in row-major order (grayscale or single channel).
 * @param image_pixels Pointer to width*height pixel buffer (uint8_t or double)
 * @param width Image width
 * @param height Image height
 * @param dct_coeffs Output: buffer to store DCT coefficients (same size as image, or array of blocks)
 */
void dct_image(const uint8_t *image_pixels,
               int width, int height,
               double *dct_coeffs);

/**
 * @brief Perform inverse DCT for all non-overlapping 8x8 DCT blocks in an image.
 * @param dct_coeffs Input buffer of DCT coefficients
 * @param width Image width
 * @param height Image height
 * @param image_pixels Output: reconstructed image (uint8_t)
 */
void idct_image(const double *dct_coeffs,
                int width, int height,
                uint8_t *image_pixels);

#endif // DCT_H