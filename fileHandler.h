#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "GLOBAL.h"

int parseCommandLine(int argc, char **argv, const char **imageFilename, const char **maskFilename, const char **blurredImageFilename, int *N);
void getImage(const char *filename, unsigned char *image, int size);
int getMask(const char *filename, mask **maskArray);
void saveBlurredImage(const char *filename, unsigned char *blurredImage, int size);

#endif
