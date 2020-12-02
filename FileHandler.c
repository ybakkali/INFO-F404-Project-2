#include "FileHandler.h"
#include <stdio.h>
#include <stdlib.h>

void getImage(const char *filename, unsigned char *image, unsigned int size) {
    FILE* file = fopen(filename, "rb");
    fread(image, 1, size, file);
    fclose(file);
}

int getMask(const char *filename, mask **maskArray) {
    *maskArray = malloc(1 * sizeof(mask));
    FILE* file = fopen(filename, "rb");

    mask mask_ = {0, 0, 0, 0};

    int maskNumber = 0;
    while (fscanf(file, "%u %u %u %u", &mask_.start_i, &mask_.start_j, &mask_.stop_i, &mask_.stop_j) != EOF) {
        (*maskArray)[maskNumber] = mask_;
        maskNumber++;
        *maskArray = realloc(*maskArray, (maskNumber + 1) * sizeof(mask));
    }
    fclose(file);
    return maskNumber;
}

void saveBlurredImage(const char *filename, unsigned char *blurredImage, unsigned int size) {
    FILE *file = fopen(filename, "wb" );
    fwrite(blurredImage, 1, size, file);
    fclose(file);
}