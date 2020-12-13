
all: blur.c utils.o blurring.o process.o
	mpicc blur.c utils.o blurring.o process.o -o blur -Wall

utils.o: utils.h utils.c
	mpicc -c utils.c -Wall

process.o: process.h process.c
	mpicc -c process.c -Wall

blurring.o: blurring.h blurring.c
	mpicc -c blurring.c -Wall

run:
	mpirun -n 6 blur -f data/police1.raw -m data/mask1 -o data/blurred_image.raw -n 5

clean:
	rm *.o*