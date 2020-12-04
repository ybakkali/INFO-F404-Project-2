#ifndef GLOBAL_H
#define GLOBAL_H

#include <mpi.h>
#define W 1280
#define H 720

MPI_Datatype MPI_MASK;

typedef struct Mask {
    unsigned int start_i;
    unsigned int start_j;
    unsigned int stop_i;
    unsigned int stop_j;
} mask;

#endif
