#include "../include/dct.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void dct_forward(const double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE], double output[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE]) {
    double ci, cj, dct1, sum;

    for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
            if (i == 0) ci = 1 / sqrt(2.0);
            else ci = 1;
            if (j == 0) cj = 1 / sqrt(2.0);
            else cj = 1;

            sum = 0;
            for (int k = 0; k < DCT_BLOCK_SIZE; k++) {
                for (int l = 0; l < DCT_BLOCK_SIZE; l++) {
                    dct1 = input[k][l] * 
                           cos((2 * k + 1) * i * M_PI / (2.0 * DCT_BLOCK_SIZE)) * 
                           cos((2 * l + 1) * j * M_PI / (2.0 * DCT_BLOCK_SIZE));
                    sum += dct1;
                }
            }
            output[i][j] = (2.0 / DCT_BLOCK_SIZE) * ci * cj * sum;
        }
    }
}

void dct_inverse(const double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE], double output[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE]) {
    double ci, cj, dct1, sum;

    for (int k = 0; k < DCT_BLOCK_SIZE; k++) {
        for (int l = 0; l < DCT_BLOCK_SIZE; l++) {
            sum = 0;
            for (int i = 0; i < DCT_BLOCK_SIZE; i++) {
                for (int j = 0; j < DCT_BLOCK_SIZE; j++) {
                    if (i == 0) ci = 1 / sqrt(2.0);
                    else ci = 1;
                    if (j == 0) cj = 1 / sqrt(2.0);
                    else cj = 1;

                    dct1 = ci * cj * input[i][j] * 
                           cos((2 * k + 1) * i * M_PI / (2.0 * DCT_BLOCK_SIZE)) * 
                           cos((2 * l + 1) * j * M_PI / (2.0 * DCT_BLOCK_SIZE));
                    sum += dct1;
                }
            }
            output[k][l] = (2.0 / DCT_BLOCK_SIZE) * sum;
        }
    }
}

void dct_image(const uint8_t *image_pixels, int width, int height, double *dct_coeffs) {
    int num_blocks_x = width / DCT_BLOCK_SIZE;
    int num_blocks_y = height / DCT_BLOCK_SIZE;

    for (int by = 0; by < num_blocks_y; by++) {
        for (int bx = 0; bx < num_blocks_x; bx++) {
            double input_block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];
            double output_block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

            for (int y = 0; y < DCT_BLOCK_SIZE; y++) {
                for (int x = 0; x < DCT_BLOCK_SIZE; x++) {
                    int pixel_x = bx * DCT_BLOCK_SIZE + x;
                    int pixel_y = by * DCT_BLOCK_SIZE + y;
                    input_block[y][x] = image_pixels[pixel_y * width + pixel_x];
                }
            }

            dct_forward(input_block, output_block);

            for (int y = 0; y < DCT_BLOCK_SIZE; y++) {
                for (int x = 0; x < DCT_BLOCK_SIZE; x++) {
                    int coeff_x = bx * DCT_BLOCK_SIZE + x;
                    int coeff_y = by * DCT_BLOCK_SIZE + y;
                    dct_coeffs[coeff_y * width + coeff_x] = output_block[y][x];
                }
            }
        }
    }
}

void idct_image(const double *dct_coeffs, int width, int height, uint8_t *image_pixels) {
    int num_blocks_x = width / DCT_BLOCK_SIZE;
    int num_blocks_y = height / DCT_BLOCK_SIZE;

    for (int by = 0; by < num_blocks_y; by++) {
        for (int bx = 0; bx < num_blocks_x; bx++) {
            double input_block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];
            double output_block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

            for (int y = 0; y < DCT_BLOCK_SIZE; y++) {
                for (int x = 0; x < DCT_BLOCK_SIZE; x++) {
                    int coeff_x = bx * DCT_BLOCK_SIZE + x;
                    int coeff_y = by * DCT_BLOCK_SIZE + y;
                    input_block[y][x] = dct_coeffs[coeff_y * width + coeff_x];
                }
            }

            dct_inverse(input_block, output_block);

            for (int y = 0; y < DCT_BLOCK_SIZE; y++) {
                for (int x = 0; x < DCT_BLOCK_SIZE; x++) {
                    int pixel_x = bx * DCT_BLOCK_SIZE + x;
                    int pixel_y = by * DCT_BLOCK_SIZE + y;
                    double val = output_block[y][x];
                    if (val < 0) val = 0;
                    if (val > 255) val = 255;
                    image_pixels[pixel_y * width + pixel_x] = (uint8_t)round(val);
                }
            }
        }
    }
}
