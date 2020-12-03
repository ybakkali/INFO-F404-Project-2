
all: Main.c FileHandler.o Blur.o
	mpicc Main.c FileHandler.o Blur.o -o Main.out

FileHandler.o: FileHandler.h FileHandler.c
	mpicc -c FileHandler.c

Blur.o: Blur.h Blur.c
	mpicc -c Blur.c

run:
	mpirun -n 6 Main.out -f data/police1.raw -m data/mask1 -o data/blurred_image.raw -n 8

clean:
	rm *.o*