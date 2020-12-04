#include <stdlib.h>
#include "Process.h"
#include "GLOBAL.h"
#include "FileHandler.h"
#include "Blur.h"

/*
 * The master process that will open the image and mask files and distribute them to other processes. After processing
 * one part of the image and blurring it, it will gather the other processed part sent by the slave processes and save
 * the blurred image as a file.
 *
 * :param imageFilename: the name of the raw image file to open
 * :param maskFilename: the name of the mask file to be used for the opened raw image
 * :param blurredImageFilename: the name of the raw blurred image file to save
 * :param worldSize: the number of instances in the MPI_COMM_WORLD set (processes number)
 * :param N: the size of the neighbourhood used for the blur
 */
void master(const char *imageFilename, const char *maskFilename, const char *blurredImageFilename, int worldSize, unsigned int N) {

    int size = (H / worldSize) * W;
    unsigned char image[W * H], blurredImage[W * H], blurredImagePart[size];
    unsigned int imagePartSizes[worldSize], displacements[worldSize];
    mask *maskArray;

    for (int i = 0; i < worldSize; ++i) {
        imagePartSizes[i] = (i == worldSize - 1) ? (H / worldSize) * W + (H % worldSize) : (H / worldSize) * W;
        displacements[i] = (i == 0) ? 0 : displacements[i - 1] + imagePartSizes[i - 1];
    }

    getImage(imageFilename, image, W * H);
    unsigned int maskNumber = getMask(maskFilename, &maskArray);

    MPI_Bcast(&maskNumber, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);

    blurring(image, maskArray, maskNumber, blurredImagePart, worldSize, 0, N);

    MPI_Gatherv(blurredImagePart, size, MPI_UNSIGNED_CHAR, blurredImage, imagePartSizes, displacements, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    saveBlurredImage(blurredImageFilename, blurredImage, W * H);
    free(maskArray);
}

/*
 * The slave process that will receive the image to be blurred and it will start processing its part of the image. Once
 * the blurring is done, the processed part of the image will be sent to the master process.
 *
 * :param worldSize: the number of instances in the MPI_COMM_WORLD set (processes number)
 * :param rank: the identifier (ID) of the current process
 * :param N: the size of the neighbourhood used for the blur
 */
void slave(int worldSize, int rank, unsigned int N) {
    unsigned int size = (rank == worldSize - 1) ? (H / worldSize) * W + (H % worldSize) : (H / worldSize) * W;
    unsigned char image[W * H], blurredImagePart[size];
    unsigned int maskNumber;

    MPI_Bcast(&maskNumber, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    mask *maskArray = malloc(maskNumber * sizeof(mask));
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);

    blurring(image, maskArray, maskNumber, blurredImagePart, worldSize, rank, N);

    MPI_Gatherv(blurredImagePart, size, MPI_UNSIGNED_CHAR, NULL, NULL, NULL, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    free(maskArray);
}