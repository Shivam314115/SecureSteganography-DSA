#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/image.h"

// BMP file header structs with 1-byte packing
#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;        // File type, must be 'BM' (0x4D42)
    uint32_t bfSize;        // Size of file in bytes
    uint16_t bfReserved1;   // Reserved, must be 0
    uint16_t bfReserved2;   // Reserved, must be 0
    uint32_t bfOffBits;     // Offset to start of image data
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

// Load BMP: uncompressed 24-bit image, BMP stores pixels bottom-up, BGR format
int load_bmp(const char *filename, Image *image) {
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    if (fread(&bfh, sizeof(bfh), 1, f) != 1 ||
        fread(&bih, sizeof(bih), 1, f) != 1) {
        fclose(f); return -2;
    }
    if (bfh.bfType != 0x4D42) { fclose(f); return -3; }
    if (bih.biBitCount != 24 || bih.biCompression != 0) { fclose(f); return -4; }

    fseek(f, bfh.bfOffBits, SEEK_SET);

    int width = bih.biWidth;
    int height = (bih.biHeight > 0) ? bih.biHeight : -bih.biHeight;

    int row_padded = (width * 3 + 3) & (~3);
    image->data = malloc(3 * width * height);
    if (!image->data) { fclose(f); return -5; }

    image->width = width;
    image->height = height;
    image->channels = 3;

    // BMP pixel rows start from bottom of image
    for (int y = height - 1; y >= 0; y--) {
        uint8_t *row = malloc(row_padded);
        if (!row) { free(image->data); fclose(f); return -5; }
        fread(row, 1, row_padded, f);
        for (int x = 0; x < width; x++) {
            int src_idx = x * 3;
            int dst_idx = (y * width + x) * 3;
            // Convert BGR to RGB
            image->data[dst_idx + 0] = row[src_idx + 2];
            image->data[dst_idx + 1] = row[src_idx + 1];
            image->data[dst_idx + 2] = row[src_idx + 0];
        }
        free(row);
    }

    fclose(f);
    return 0;
}

// Save BMP: write 24-bit uncompressed BMP file storing pixel rows bottom-up in BGR order
int save_bmp(const char *filename, const Image *image) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    int width = image->width;
    int height = image->height;
    int row_padded = (width * 3 + 3) & (~3);
    int data_size = row_padded * height;

    BITMAPFILEHEADER bfh = {0x4D42, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (uint32_t)data_size, 0, 0, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)};
    BITMAPINFOHEADER bih = {sizeof(BITMAPINFOHEADER), width, height, 1, 24, 0, (uint32_t)data_size, 0, 0, 0, 0};

    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    // Write rows bottom-up, convert RGB to BGR
    for (int y = height - 1; y >= 0; y--) {
        uint8_t *row = calloc(1, row_padded);
        if (!row) { fclose(f); return -2; }
        for (int x = 0; x < width; x++) {
            int src_idx = (y * width + x) * 3;
            int dst_idx = x * 3;
            row[dst_idx + 0] = image->data[src_idx + 2]; // B
            row[dst_idx + 1] = image->data[src_idx + 1]; // G
            row[dst_idx + 2] = image->data[src_idx + 0]; // R
        }
        fwrite(row, 1, row_padded, f);
        free(row);
    }

    fclose(f);
    return 0;
}

void free_image(Image *image) {
    if (image->data) free(image->data);
    image->data = NULL;
}

// Test main function loading and saving BMP file from your absolute paths
int main() {
    Image img;
    const char *input_path = "/Users/yash/Documents/GitHub/SecureSteganography-DSA/data/input.bmp";
    const char *output_path = "/Users/yash/Documents/GitHub/SecureSteganography-DSA/data/copy.bmp";

    if (load_bmp(input_path, &img) != 0) {
        printf("Failed to load %s\n", input_path);
        return 1;
    }
    printf("Loaded: %dx%d\n", img.width, img.height);

    // Optionally process img.data here...

    if (save_bmp(output_path, &img) != 0) {
        printf("Failed to save %s\n", output_path);
        free_image(&img);
        return 2;
    }
    printf("Saved %s\n", output_path);

    free_image(&img);
    return 0;
}
    