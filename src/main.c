#include "../include/stego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
    printf("Usage: ./main embed <cover_image> <secret_message_file> <stego_image>\n");
    printf("       ./main extract <stego_image> <output_file>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "embed") == 0) {
        if (argc != 5) {
            print_usage();
            return 1;
        }

        char *cover_image = argv[2];
        char *secret_message_file = argv[3];
        char *stego_image = argv[4];

        FILE *f = fopen(secret_message_file, "r");
        if (!f) {
            printf("Error: Could not open secret message file.\n");
            return 1;
        }
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *secret_message = (char *)malloc(fsize + 1);
        fread(secret_message, 1, fsize, f);
        fclose(f);
        secret_message[fsize] = '\0';

        StegoParams params = {
            .cover_image_path = cover_image,
            .stego_image_path = stego_image,
            .secret_message = secret_message,
            .message_length = strlen(secret_message),
            .key = 12345
        };

        if (embedMessage(&params) != 0) {
            printf("Error: Could not embed message.\n");
            return 1;
        }

        printf("Message embedded successfully.\n");
        free(secret_message);

    } else if (strcmp(argv[1], "extract") == 0) {
        if (argc != 4) {
            print_usage();
            return 1;
        }

        char *stego_image = argv[2];
        char *output_file = argv[3];

        char extracted_message[1000];
        if (extractMessage(stego_image, extracted_message, sizeof(extracted_message)) <= 0) {
            printf("Error: Could not extract message.\n");
            return 1;
        }

        FILE *f = fopen(output_file, "w");
        if (!f) {
            printf("Error: Could not open output file.\n");
            return 1;
        }
        fprintf(f, "%s", extracted_message);
        fclose(f);

        printf("Message extracted successfully.\n");

    } else {
        print_usage();
        return 1;
    }

    return 0;
}
