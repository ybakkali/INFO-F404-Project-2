
all: Main.c
	mpicc Main.c -o Main.out

run:
	mpirun -n 1 Main.out 8

clean:
	rm Main.out
