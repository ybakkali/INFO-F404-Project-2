#ifndef INFO_F404_PROJECT_2_BLUR_H
#define INFO_F404_PROJECT_2_BLUR_H

#include "FileHandler.h"
#define W 1280
#define H 720

int neighbourhoodAverage(unsigned int i, unsigned int j, unsigned int N, const unsigned char *image);
void blurring(const unsigned char *image, mask *maskArray, int maskNumber,
              unsigned char *blurredImage, int worldSize, int rank, int N);



#endif //INFO_F404_PROJECT_2_BLUR_H
