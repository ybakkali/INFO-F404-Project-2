#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "FileHandler.h"
#include "Blur.h"

#define W 1280
#define H 720

void master(const char *imageFilename, const char *blurredImageFilename, MPI_Datatype MPI_MASK, int worldSize, int N) {
    int size = (H / worldSize) * W + (H % worldSize);

    unsigned char image[W * H];
    unsigned char blurredImage[W * H];
    unsigned char blurredImagePortion[size];
    mask *maskArray;

    getImage(imageFilename, image, W * H);
    int maskNumber = getMask(blurredImageFilename, &maskArray);

    MPI_Bcast(&maskNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);

    blurring(image, maskArray, maskNumber, blurredImagePortion, worldSize, 0, N);

    MPI_Gather(blurredImagePortion, size, MPI_UNSIGNED_CHAR, blurredImage, size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    saveBlurredImage("data/blurred_image.raw", blurredImage, W * H);
    free(maskArray);
}

void slave(MPI_Datatype MPI_MASK, int worldSize, int rank, int N) {
    int size = (H / worldSize) * W + (H % worldSize);
    unsigned char image[W * H];
    unsigned char blurredImagePortion[size];
    int maskNumber;

    MPI_Bcast(&maskNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    mask *maskArray = malloc(maskNumber * sizeof(mask));
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);

    blurring(image, maskArray, maskNumber, blurredImagePortion, worldSize, rank, N);

    MPI_Gather(blurredImagePortion, size, MPI_UNSIGNED_CHAR, NULL, size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    free(maskArray);
}

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int count = 4;
    int arrayOfBlockLengths[] = {1, 1, 1, 1};
    MPI_Aint arrayOfDisplacements[] = {offsetof(mask, start_i), offsetof(mask, start_j),
                                       offsetof(mask, stop_i),offsetof(mask, stop_j)};
    MPI_Datatype arrayOfTypes[] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
    MPI_Datatype MPI_MASK;

    MPI_Type_create_struct(count, arrayOfBlockLengths, arrayOfDisplacements, arrayOfTypes, &MPI_MASK);
    MPI_Type_commit(&MPI_MASK);

    int rank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    unsigned int N = atoi(argv[3]);

    if (rank == 0) {
        master(argv[1], argv[2], MPI_MASK, worldSize, N);
    } else {
        slave(MPI_MASK, worldSize, rank, N);
    }
    MPI_Finalize();
    return 0;
}