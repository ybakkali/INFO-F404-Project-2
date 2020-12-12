#include "blurring.h"
#include <stdio.h>

/*
 * Compute the average value of the pixel at the (x,y) coordinate of the image with a neighborhood of size N
 *
 * :param i: the y-coordinate of a pixel of the image
 * :param j: the x-coordinate of a pixel of the image
 * :param N: the size of the neighbourhood used for the blur
 * :param image: the buffer where the image is stored
 *
 * :return : the average value
 */
unsigned char neighbourhoodAverage(int i, int j, int N, const unsigned char *image) {
    int sum = 0, elem_number = 0;
    for (int x = i - N; x < i + N; x++) {
        for (int y = j - N; y < j + N; y++) {
            // Take only the neighborhood pixel that is within the dimension of the image
            // Best case 4N**2, Worst case N**2
            if (x >= 0 && x < H && y >= 0 && y < W) {
                sum += image[W * x + y];
                elem_number++;
            }
        }
    }
    return sum / elem_number;
}

/*
 * Blurring of an image by replacing each pixel in the mask interval by the average pixel of the neighborhood
 *
 * :param image: the buffer where the image is stored
 * :param maskArray: the list of masks to use to blur the image
 * :param maskNumber: the number of masks in the maskArray list
 * :param blurredImagePart: the buffer in which the blurred image part will be stored
 * :param worldSize: the number of instances in the MPI_COMM_WORLD set (processes number)
 * :param rank: the identifier (ID) of the current process
 * :param N: the size of the neighbourhood used for the blur
 */
void blurring(const unsigned char *image, mask *maskArray, int maskNumber, unsigned char *blurredImagePart, int worldSize, int rank, int N) {

    // Separate the blurring task into several processes
    // Each process processes a unique part of the image designated by the [start,stop] interval
    int start = rank * (H / worldSize);
    int stop = (rank < worldSize - 1) ? start + (H / worldSize) : H;
    int counter = 0;

    // Before starting blurring
    // Copy the part of the image into the blurredImagePart buffer
    for (int i = start; i < stop; i++) {
        for (int j = 0; j < W; j++) {
            blurredImagePart[counter] = image[W * i + j];
            counter++;
        }
    }

    // Starting blurring
    // Blurring each pixel in the interval of the mask belonging to the maskArray
    for (int index = 0 ; index < maskNumber; index++) {
        mask m = maskArray[index];
        counter = 0;
        for (int i = start; i < stop; i++) {
            for (int j = 0; j < W; j++) {
                if (i >= m.start_i && i <= m.stop_i && j >= m.start_j && j <= m.stop_j) {
                    blurredImagePart[counter] = neighbourhoodAverage(i, j, N, image);
                }
                counter++;
            }
        }
    }
}