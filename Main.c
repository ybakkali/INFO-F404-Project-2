#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>
#include "FileHandler.h"
#include "Blur.h"
#include "GLOBAL.h"

MPI_Datatype MPI_MASK;

int parseCommandLine(int argc, char **argv, char *imageFilename, char *maskFilename, char *blurredImageFilename, int *N) {

    if (argc != 9) {
        printf("Some options are missing\n");
        return 1;
    }

    int option, mandatoryOptions = 0;
    while((option = getopt(argc, argv, "f:m:o:n:")) != -1){
        switch(option){
            case 'f':
                strcpy(imageFilename, optarg);
                mandatoryOptions++;
                break;
            case 'm':
                strcpy(maskFilename, optarg);
                mandatoryOptions++;
                break;
            case 'o':
                strcpy(blurredImageFilename, optarg);
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

void MPI_MASKType() {
    int count = 4;
    int arrayOfBlockLengths[] = {1, 1, 1, 1};
    MPI_Aint arrayOfDisplacements[] = {offsetof(mask, start_i), offsetof(mask, start_j),
                                       offsetof(mask, stop_i),offsetof(mask, stop_j)};
    MPI_Datatype arrayOfTypes[] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
    MPI_Type_create_struct(count, arrayOfBlockLengths, arrayOfDisplacements, arrayOfTypes, &MPI_MASK);
    MPI_Type_commit(&MPI_MASK);
}

void master(const char *imageFilename, const char *maskFilename, const char *blurredImageFilename, int worldSize, int N) {
    int size = (H / worldSize) * W + (H % worldSize);

    unsigned char image[W * H];
    unsigned char blurredImage[W * H];
    unsigned char blurredImagePart[size];
    mask *maskArray;

    getImage(imageFilename, image, W * H);
    int maskNumber = getMask(maskFilename, &maskArray);

    MPI_Bcast(&maskNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);

    blurring(image, maskArray, maskNumber, blurredImagePart, worldSize, 0, N);

    MPI_Gather(blurredImagePart, size, MPI_UNSIGNED_CHAR, blurredImage, size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    saveBlurredImage(blurredImageFilename, blurredImage, W * H);
    free(maskArray);
}

void slave(int worldSize, int rank, int N) {
    int size = (H / worldSize) * W + (H % worldSize);
    unsigned char image[W * H];
    unsigned char blurredImagePart[size];
    int maskNumber;

    MPI_Bcast(&maskNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    mask *maskArray = malloc(maskNumber * sizeof(mask));
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);

    blurring(image, maskArray, maskNumber, blurredImagePart, worldSize, rank, N);

    MPI_Gather(blurredImagePart, size, MPI_UNSIGNED_CHAR, NULL, size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    free(maskArray);
}

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    int rank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_MASKType();

    if (rank == 0) {
        char imageFilename[100], maskFilename[100], blurredImageFilename[100];
        int N;

        if (parseCommandLine(argc, argv, imageFilename, maskFilename, blurredImageFilename, &N) == 1) {
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
        master(imageFilename, maskFilename, blurredImageFilename, worldSize, N);

    } else {
        int N;
        MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
        slave(worldSize, rank, N);
    }
    MPI_Finalize();
    return 0;
}