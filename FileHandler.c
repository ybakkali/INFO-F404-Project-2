#include "FileHandler.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Read the raw image file and store it in a buffer.
 *
 * :param filename: the name of the raw image to open
 * :param image: the buffer where the image will be stored
 * :param size: the size of the image which is the height * width
 */
void getImage(const char *filename, unsigned char *image, unsigned int size) {
    FILE* file = fopen(filename, "rb");
    fread(image, 1, size, file);
    fclose(file);
}

/*
 * Read the mask file and store it in a buffer.
 *
 * :param filename: the name of the mask file to be used for the opened raw image
 * :param maskArray: the list of masks to use to blur the image
 */
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

/*
 * Create a raw blurred image file.
 *
 * :param filename : the name of the raw blurred image to save
 * :param blurredImage: the buffer in which the blurred image is stored
 * :param size : the size of the image which is the height * width
 */
void saveBlurredImage(const char *filename, unsigned char *blurredImage, unsigned int size) {
    FILE *file = fopen(filename, "wb" );
    fwrite(blurredImage, 1, size, file);
    fclose(file);
}