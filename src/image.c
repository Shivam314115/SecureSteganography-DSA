#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/image.h"

// -- Minimal BMP loader for uncompressed 24-bit BMP files only --

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

    if (bfh.bfType != 0x4D42) { fclose(f); return -2; }
    if (bih.biBitCount != 24 || bih.biCompression != 0) { fclose(f); return -3; }

    fseek(f, bfh.bfOffBits, SEEK_SET);

    int row_padded = (bih.biWidth * 3 + 3) & (~3);
    image->data = (uint8_t *)malloc(3 * bih.biWidth * abs(bih.biHeight));
    image->width = bih.biWidth;
    image->height = abs(bih.biHeight);
    image->channels = 3;

    for (int y = 0; y < image->height; y++) {
        uint8_t *row = (uint8_t*)malloc(row_padded);
        fread(row, 1, row_padded, f);
        for (int x = 0; x < image->width; x++) {
            int dst_idx = (y * image->width + x) * 3;
            int src_idx = x * 3;
            image->data[dst_idx + 0] = row[src_idx + 2]; // R
            image->data[dst_idx + 1] = row[src_idx + 1]; // G
            image->data[dst_idx + 2] = row[src_idx + 0]; // B
        }
        free(row);
    }
    fclose(f);
    return 0;
}

int save_bmp(const char *filename, const Image *image) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    int width = image->width, height = image->height;
    int row_padded = (width * 3 + 3) & (~3);
    int data_size = row_padded * height;
    BITMAPFILEHEADER bfh = {0x4D42, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + data_size, 0, 0, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)};
    BITMAPINFOHEADER bih = {sizeof(BITMAPINFOHEADER), width, height, 1, 24, 0, data_size, 0, 0, 0, 0};

    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    for (int y = 0; y < height; y++) {
        uint8_t row[row_padded];
        memset(row, 0, row_padded);
        for (int x = 0; x < width; x++) {
            int src_idx = (y * width + x) * 3;
            int dst_idx = x * 3;
            row[dst_idx + 2] = image->data[src_idx + 0]; // B
            row[dst_idx + 1] = image->data[src_idx + 1]; // G
            row[dst_idx + 0] = image->data[src_idx + 2]; // R
        }
        fwrite(row, 1, row_padded, f);
    }
    fclose(f);
    return 0;
}

void free_image(Image *image) {
    if (image->data) free(image->data);
    image->data = NULL;
}

// ---- Minimal test main ----
#ifdef TEST_IMAGE_MAIN
int main() {
    Image img;
    if (load_bmp("input.bmp", &img) != 0) {
        printf("Failed to load input.bmp\n");
        return 1;
    }
    printf("Loaded: %dx%d\n", img.width, img.height);

    // [OPTIONAL: Do something with img.data here]

    if (save_bmp("copy.bmp", &img) != 0) {
        printf("Failed to save copy.bmp\n");
        free_image(&img);
        return 2;
    }
    printf("Saved copy.bmp\n");
    free_image(&img);
    return 0;
}
#endif
