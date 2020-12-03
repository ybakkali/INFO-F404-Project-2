#ifndef INFO_F404_PROJECT_2_FILEHANDLER_H
#define INFO_F404_PROJECT_2_FILEHANDLER_H

#include "GLOBAL.h"

void getImage(const char *filename, unsigned char *image, unsigned int size);
int getMask(const char *filename, mask **maskArray);
void saveBlurredImage(const char *filename, unsigned char *blurredImage, unsigned int size);

#endif //INFO_F404_PROJECT_2_FILEHANDLER_H
