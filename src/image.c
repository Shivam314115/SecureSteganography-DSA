#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

int load_bmp(const char *filename, Image *image) {
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    fread(&bfh, sizeof(bfh), 1, f);
    fread(&bih, sizeof(bih), 1, f);

    if (bfh.bfType != 0x4D42) { fclose(f); return -2; } // Not a BMP file
    if (bih.biBitCount != 24 || bih.biCompression != 0) { fclose(f); return -3; } // Not a 24-bit uncompressed BMP

    image->width = bih.biWidth;
    image->height = abs(bih.biHeight);
    image->channels = 3;
    int image_size = image->width * image->height * image->channels;
    image->data = (uint8_t *)malloc(image_size);

    fseek(f, bfh.bfOffBits, SEEK_SET);

    int row_padded = (image->width * 3 + 3) & (~3);
    uint8_t *row = (uint8_t *)malloc(row_padded);

    for (int i = 0; i < image->height; i++) {
        fread(row, sizeof(uint8_t), row_padded, f);
        for (int j = 0; j < image->width; j++) {
            // BMP stores pixels in BGR order, so we convert to RGB
            image->data[(i * image->width + j) * 3 + 2] = row[j * 3 + 0];
            image->data[(i * image->width + j) * 3 + 1] = row[j * 3 + 1];
            image->data[(i * image->width + j) * 3 + 0] = row[j * 3 + 2];
        }
    }

    free(row);
    fclose(f);
    return 0;
}

int save_bmp(const char *filename, const Image *image) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    int row_padded = (image->width * 3 + 3) & (~3);
    int data_size = row_padded * image->height;

    BITMAPFILEHEADER bfh = {
        0x4D42, // "BM"
        sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + data_size,
        0, 0,
        sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
    };

    BITMAPINFOHEADER bih = {
        sizeof(BITMAPINFOHEADER),
        image->width,
        image->height,
        1,
        24,
        0,
        data_size,
        0, 0, 0, 0
    };

    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    uint8_t *row = (uint8_t *)malloc(row_padded);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            // Convert back to BGR for BMP file
            row[j * 3 + 0] = image->data[(i * image->width + j) * 3 + 2];
            row[j * 3 + 1] = image->data[(i * image->width + j) * 3 + 1];
            row[j * 3 + 2] = image->data[(i * image->width + j) * 3 + 0];
        }
        fwrite(row, sizeof(uint8_t), row_padded, f);
    }

    free(row);
    fclose(f);
    return 0;
}

void free_image(Image *image) {
    if (image && image->data) {
        free(image->data);
    }
}