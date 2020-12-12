#ifndef BLUR_H
#define BLUR_H

#include "GLOBAL.h"

unsigned char neighbourhoodAverage(int i, int j, int N, const unsigned char *image);
void blurring(const unsigned char *image, mask *maskArray, int maskNumber,
              unsigned char *blurredImage, int worldSize, int rank, int N);

#endif
