#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "FileHandler.h"
#include "Process.h"
#include "GLOBAL.h"

MPI_Datatype MPI_MASK;

/*
 * Add a new MPI type to send the master's mask array to the slaves.
 */
void MPI_MASKType() {
    int count = 4;
    int arrayOfBlockLengths[] = {1, 1, 1, 1};
    MPI_Aint arrayOfDisplacements[] = {offsetof(mask, start_i), offsetof(mask, start_j),
                                       offsetof(mask, stop_i),offsetof(mask, stop_j)};
    MPI_Datatype arrayOfTypes[] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
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
    unsigned int N;

    if (rank == 0) {
        const char *imageFilename, *maskFilename, *blurredImageFilename;
        if (parseCommandLine(argc, argv, &imageFilename, &maskFilename, &blurredImageFilename, &N) == 1) {
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        MPI_Bcast(&N, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        master(imageFilename, maskFilename, blurredImageFilename, worldSize, N);

    } else {
        MPI_Bcast(&N, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        slave(worldSize, rank, N);
    }
    MPI_Finalize();
    return 0;
}