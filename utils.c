#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Parse the command line and extract the arguments from the options and store them.
 *
 * :param argc: the argument count
 * :param argv: the argument values
 * :param imageFilename: the buffer in which the image filename string will be stored
 * :param maskFilename: the buffer in which the mask filename string will be stored
 * :param blurredImageFilename: the buffer in which the blurred image filename string will be stored
 * :param N: the neighbourhood size value
 *
 * : return: 1 if a mandatory option is missing in the command line, otherwise 0
 */
int parseCommandLine(int argc, char **argv, const char **imageFilename, const char **maskFilename, const char **blurredImageFilename, int *N) {

    if (argc != 9) {
        printf("Some options are missing\n");
        return 1;
    }

    int option, mandatoryOptions = 0;
    while((option = getopt(argc, argv, "f:m:o:n:")) != -1){
        switch(option){
            case 'f':
                *imageFilename = optarg;
                mandatoryOptions++;
                break;
            case 'm':
                *maskFilename = optarg;
                mandatoryOptions++;
                break;
            case 'o':
                *blurredImageFilename = optarg;
                mandatoryOptions++;
                break;
            case 'n':
                *N = atoi(optarg);
                mandatoryOptions++;
                break;
            case '?':
                //printf("unknown option: %c\n", optopt);
                return 1;
        }
    }

    if (mandatoryOptions != 4) {
        printf("Some mandatory options are missing\n");
        return 1;
    }

    return 0;
}

/*
 * Read the raw image file and store it in a buffer.
 *
 * :param filename: the name of the raw image file to open
 * :param image: the buffer where the image will be stored
 * :param size: the size of the image which is the height * width
 */
void getImage(const char *filename, unsigned char *image, int size) {
    FILE* file = fopen(filename, "rb");
    fread(image, 1, size, file);
    fclose(file);
}

/*
 * Read the mask file and store it in a buffer.
 *
 * :param filename: the name of the mask file to be used for the opened raw image
 * :param maskArray: the list of masks to use to blur the image
 *
 * :return: the number of masks
 */
int getMask(const char *filename, mask **maskArray) {
    *maskArray = malloc(1 * sizeof(mask));
    FILE* file = fopen(filename, "r");
    int maskNumber = 0;
    mask mask_ = {0, 0, 0, 0};

    while (fscanf(file, "%i %i %i %i", &mask_.start_i, &mask_.start_j, &mask_.stop_i, &mask_.stop_j) != EOF) {
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
 * :param filename : the name of the raw blurred image file to save
 * :param blurredImage: the buffer in which the blurred image is stored
 * :param size : the size of the image which is the height * width
 */
void saveBlurredImage(const char *filename, unsigned char *blurredImage, int size) {
    FILE *file = fopen(filename, "wb" );
    fwrite(blurredImage, 1, size, file);
    fclose(file);
}