#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "../include/dct.h"

#define PI 3.14159265358979323846

void dct_forward(const double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE],
                 double output[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE]) {
    for (int u = 0; u < DCT_BLOCK_SIZE; u++) {
        for (int v = 0; v < DCT_BLOCK_SIZE; v++) {
            double sum = 0.0;
            for (int x = 0; x < DCT_BLOCK_SIZE; x++) {
                for (int y = 0; y < DCT_BLOCK_SIZE; y++) {
                    sum += input[x][y] *
                           cos(((2 * x + 1) * u * PI) / (2.0 * DCT_BLOCK_SIZE)) *
                           cos(((2 * y + 1) * v * PI) / (2.0 * DCT_BLOCK_SIZE));
                }
            }
            double cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
            double cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
            output[u][v] = 0.25 * cu * cv * sum;
        }
    }
}

void dct_inverse(const double input[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE],
                 double output[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE]) {
    for (int x = 0; x < DCT_BLOCK_SIZE; x++) {
        for (int y = 0; y < DCT_BLOCK_SIZE; y++) {
            double sum = 0.0;
            for (int u = 0; u < DCT_BLOCK_SIZE; u++) {
                for (int v = 0; v < DCT_BLOCK_SIZE; v++) {
                    double cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    double cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    sum += cu * cv * input[u][v] *
                           cos(((2 * x + 1) * u * PI) / (2.0 * DCT_BLOCK_SIZE)) *
                           cos(((2 * y + 1) * v * PI) / (2.0 * DCT_BLOCK_SIZE));
                }
            }
            output[x][y] = 0.25 * sum;
        }
    }
}