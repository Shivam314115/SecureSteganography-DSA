#include "../include/image.h"
#include <stdio.h>
#include <assert.h>

int main() {
    Image img, out_img;
    int result = load_bmp("data/input.bmp", &img);
    assert(result == 0);
    printf("Loaded input.bmp: %dx%d\n", img.width, img.height);

    result = save_bmp("data/copy.bmp", &img);
    assert(result == 0);
    printf("Saved copy.bmp\n");

    result = load_bmp("data/copy.bmp", &out_img);
    assert(result == 0);
    printf("Loaded copy.bmp: %dx%d\n", out_img.width, out_img.height);

    assert(img.width == out_img.width);
    assert(img.height == out_img.height);
    assert(img.channels == out_img.channels);

    for (int i = 0; i < img.width * img.height * img.channels; i++) {
        assert(img.data[i] == out_img.data[i]);
    }

    free_image(&img);
    free_image(&out_img);

    printf("Image load/save test passed!\n");

    return 0;
}