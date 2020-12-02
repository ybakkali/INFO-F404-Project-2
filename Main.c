#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "FileHandler.h"

#define W 1280
#define H 720

int neighbourhoodAverage(unsigned int i, unsigned int j, unsigned int N, const unsigned char *image) {
    int sum = 0, elem_number = 0;
    for (int x = i - N; x < i + N; x++) {
        for (int y = j - N; y < j + N; y++) {
            if (x >= 0 && x < H && y >= 0 && y < W) {
                sum += image[W * x + y];
                elem_number++;
            }
        }
    }
    return sum / elem_number;
}

void blurring(const unsigned char *image, mask *maskArray, int maskNumber, unsigned char *blurredImage, int rank) {
    for (int index = 0 ; index < maskNumber; index++) {
        mask m = maskArray[index];

        for (int i = m.start_i; i < m.stop_i; i++) {
            for (int j = m.start_j; j < m.stop_j; j++) {
                if ((W * i + j) % 5 == rank) {
                    printf("Rank %i index (%i,%i)\n", rank, i, j);
                    blurredImage[W * i + j] = neighbourhoodAverage(i, j, 5, image);
                }
            }
        }
    }

    saveBlurredImage("data/blurred_image.raw", blurredImage, W * H);
}

void master(MPI_Datatype MPI_MASK) {
    unsigned char image[W * H];
    unsigned char blurredImage[W * H];
    mask *maskArray;
    getImage("data/police1.raw", image, blurredImage, W * H);
    int maskNumber = getMask("data/mask1", &maskArray);
    MPI_Bcast(&maskNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(blurredImage, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);
    blurring(image, maskArray, maskNumber, blurredImage, 0);
    free(maskArray);
}

void slave(MPI_Datatype MPI_MASK, int rank) {
    unsigned char image[W * H];
    unsigned char blurredImage[W * H];
    int maskNumber;
    MPI_Bcast(&maskNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(image, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(blurredImage, W * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    mask *maskArray = malloc(maskNumber * sizeof(mask));
    MPI_Bcast(maskArray, maskNumber, MPI_MASK, 0, MPI_COMM_WORLD);
    blurring(image, maskArray, maskNumber, blurredImage, rank);
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

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    unsigned int N = atoi(argv[1]);

    int maskNumber;

    if (rank == 0) {
        master(MPI_MASK);
    } else {
        slave(MPI_MASK, rank);
    }
    MPI_Finalize();
    return 0;
}