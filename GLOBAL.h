#ifndef GLOBAL_H
#define GLOBAL_H

#include <mpi.h>
#define W 1280
#define H 720

MPI_Datatype MPI_MASK;

typedef struct Mask {
    int start_i;
    int start_j;
    int stop_i;
    int stop_j;
} mask;

#endif
