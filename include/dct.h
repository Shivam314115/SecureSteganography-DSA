#ifndef DCT_H
#define DCT_H

#include <stdint.h>

// Define block size for DCT
#define DCT_BLOCK_SIZE 8

// Type for an 8x8 block of pixels / coefficients
typedef double DCTBlock[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

/**
 * Perform 2D Forward Discrete Cosine Transform on an 8x8 block.
 * 
 * @param input  Input 8x8 block of spatial-domain pixel values (e.g., grayscale intensities).
 * @param output Output 8x8 block of DCT coefficients.
 */
void forwardDCT(const DCTBlock input, DCTBlock output);

/**
 * Perform 2D Inverse Discrete Cosine Transform on an 8x8 block.
 * 
 * @param input  Input 8x8 block of DCT coefficients.
 * @param output Output 8x8 block of spatial-domain pixel values.
 */
void inverseDCT(const DCTBlock input, DCTBlock output);

#endif // DCT_H
