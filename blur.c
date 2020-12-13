#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "process.h"
#include "GLOBAL.h"

/*
 * Add a new MPI type to send the master's mask array to the slaves.
 */
void MPI_MASKType() {
    int count = 4;
    int arrayOfBlockLengths[] = {1, 1, 1, 1};
    MPI_Aint arrayOfDisplacements[] = {offsetof(mask, start_i), offsetof(mask, start_j),
                                       offsetof(mask, stop_i),offsetof(mask, stop_j)};
    MPI_Datatype arrayOfTypes[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Type_create_struct(count, arrayOfBlockLengths, arrayOfDisplacements, arrayOfTypes, &MPI_MASK);
    MPI_Type_commit(&MPI_MASK);
}

/*
 * The main code.
 */
int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    int rank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_MASKType();

    if (rank == 0) {
        master(argc, argv, worldSize);
    } else {
        slave(worldSize, rank);
    }
    MPI_Finalize();
    return 0;
}