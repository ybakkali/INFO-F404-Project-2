#ifndef PROCESS_H
#define PROCESS_H

void master(int argc, char **argv, int worldSize);
void slave(int worldSize, int rank);

#endif
