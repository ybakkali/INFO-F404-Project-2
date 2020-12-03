#ifndef BLUR_H
#define BLUR_H

#include "GLOBAL.h"

unsigned int neighbourhoodAverage(unsigned int i, unsigned int j, unsigned int N, const unsigned char *image);
void blurring(const unsigned char *image, mask *maskArray, int maskNumber,
              unsigned char *blurredImage, int worldSize, int rank, int N);

#endif
