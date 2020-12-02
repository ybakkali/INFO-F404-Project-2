//
// Created by yahyabakkali on 1/12/20.
//

#ifndef INFO_F404_PROJECT_2_FILEHANDLER_H
#define INFO_F404_PROJECT_2_FILEHANDLER_H

typedef struct Mask {
    unsigned int start_i;
    unsigned int start_j;
    unsigned int stop_i;
    unsigned int stop_j;
} mask;

void getImage(const char *filename, unsigned char *image, unsigned char *blurredImage, unsigned int size);
int getMask(const char *filename, mask **maskArray);
void saveBlurredImage(const char *filename, unsigned char *blurredImage, unsigned int size);

#endif //INFO_F404_PROJECT_2_FILEHANDLER_H
