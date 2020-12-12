#ifndef PROCESS_H
#define PROCESS_H

void master(const char *imageFilename, const char *maskFilename, const char *blurredImageFilename, int worldSize, int N);
void slave(int worldSize, int rank, int N);

#endif
