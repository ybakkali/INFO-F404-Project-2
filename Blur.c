#include "Blur.h"

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

void blurring(const unsigned char *image, mask *maskArray, int maskNumber, unsigned char *blurredImage, int worldSize, int rank, int N) {

    int start = rank * (H / worldSize);
    int stop = (rank < worldSize - 1) ? start + (H / worldSize) : H;
    int counter = 0;

    for (int i = start; i < stop; i++) {
        for (int j = 0; j < W; j++) {
            blurredImage[counter] = image[W * i + j];
            counter++;
        }
    }

    for (int index = 0 ; index < maskNumber; index++) {
        mask m = maskArray[index];
        counter = 0;
        for (int i = start; i < stop; i++) {
            for (int j = 0; j < W; j++) {
                if (i >= m.start_i && i < m.stop_i && j >= m.start_j && j < m.stop_j) {
                    blurredImage[counter] = neighbourhoodAverage(i, j, N, image);
                }
                counter++;
            }
        }
    }
}