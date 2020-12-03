#ifndef PROCESS_H
#define PROCESS_H

void master(const char *imageFilename, const char *maskFilename, const char *blurredImageFilename, int worldSize, unsigned int N);
void slave(int worldSize, int rank, unsigned int N);

#endif
