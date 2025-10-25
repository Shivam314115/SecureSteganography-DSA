#include "../include/dct.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>

#define TOLERANCE 1e-6

int main() {
    double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];
    double dct_coeffs[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];
    double reconstructed[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    // Create a sample input block
    for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
            input[i][j] = (double)(i + j);
        }
    }

    dct_forward(input, dct_coeffs);
    dct_inverse(dct_coeffs, reconstructed);

    // Check if the reconstructed block is close to the original
    for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
            assert(fabs(input[i][j] - reconstructed[i][j]) < TOLERANCE);
        }
    }

    printf("DCT test passed!\n");

    return 0;
}