#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "GLOBAL.h"

int parseCommandLine(int argc, char **argv, const char **imageFilename, const char **maskFilename, const char **blurredImageFilename, unsigned int *N);
void getImage(const char *filename, unsigned char *image, unsigned int size);
unsigned int getMask(const char *filename, mask **maskArray);
void saveBlurredImage(const char *filename, unsigned char *blurredImage, unsigned int size);

#endif
