
all: blur.c fileHandler.o blurring.o process.o
	mpicc blur.c fileHandler.o blurring.o process.o -o blur -Wall

fileHandler.o: fileHandler.h fileHandler.c
	mpicc -c fileHandler.c -Wall

process.o: process.h process.c
	mpicc -c process.c -Wall

blurring.o: blurring.h blurring.c
	mpicc -c blurring.c -Wall

run:
	mpirun -n 6 blur -f data/police1.raw -m data/mask1 -o data/blurred_image.raw -n 5

clean:
	rm *.o*