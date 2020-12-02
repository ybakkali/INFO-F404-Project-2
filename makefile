
all: Main.c FileHandler.o
	mpicc Main.c FileHandler.o -o Main.out

FileHandler.o: FileHandler.h FileHandler.c
	mpicc -c FileHandler.c

run:
	mpirun -n 6 Main.out 8

clean:
	rm Main.out