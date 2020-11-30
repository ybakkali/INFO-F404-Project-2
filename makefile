# Directory where openmpi library is located
MPI_DIR=/home/yann/openmpi
# Open MPI include directory
INC_DIR=$(MPI_DIR)/include
# Libs where openmpi shared objects are
LIB_DIR=$(MPI_DIR)/lib
# Bin directory
BIN_DIR=$(MPI_DIR)/bin


FLAGS = -I$(INC_DIR) -L$(LIB_DIR)

all: blur.c blur.h
	gcc -o blur blur.c $(FLAGS) -lmpi

run:
	export PATH=$(PATH):$(BIN_DIR) && \
	mpirun -n 4 blur


