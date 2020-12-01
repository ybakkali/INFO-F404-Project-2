#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define W 1280
#define H 720

typedef struct Mask {
    unsigned int start_i;
    unsigned int start_j;
    unsigned int stop_i;
    unsigned int stop_j;
} mask;


int neighbourhood_average(int i, int j, int N, const unsigned char image[]) {
    int sum = 0, elem_number = 0;
    for (int x = i - N; x < i + N; x++) {
        for (int y = j - N; y < j + N; y++) {
            if (x >= 0 && x < H && y >= 0 && y < W) {
                sum += image[W * x + y];
                elem_number++;
            }
        }
    }
    return sum / elem_number;
}


int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    int N = atoi(argv[1]);
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("%d\n", rank);


    unsigned char image[W * H];
    unsigned char blurred_image[W * H];

    FILE* file = fopen("data/police1.raw", "rb");
    fread(image, 1, W * H, file);
    rewind(file);
    fread(blurred_image, 1, W * H, file);

    mask *array = malloc(1 * sizeof(mask));

    FILE* f = fopen("data/mask1", "rb");
    mask mask_;

    int mask_number = 0;
    while (fscanf(f, "%u %u %u %u", &mask_.start_i, &mask_.start_j, &mask_.stop_i, &mask_.stop_j) != EOF) {
        array[mask_number] = mask_;
        array = realloc(array, 2 * sizeof(mask));
        mask_number++;
    }

    for (int index = 0 ; index < mask_number; index++) {
        mask m = array[index];

        for (int i = m.start_i; i < m.stop_i; i++) {
            for (int j = m.start_j; j < m.stop_j; j++) {

                blurred_image[W * i + j] = neighbourhood_average(i, j, N, image);
            }
        }
    }

    FILE *output = fopen( "data/blurred_image.raw" , "wb" );
    fwrite(blurred_image , 1 , W * H , output);

    fclose(output);
    fclose(f);
    fclose(file);
    free(array);
    MPI_Finalize();
    return 0;
}