// Найти среднее арифметическое положительных чисел массива

#include <mpi.h>
#include <iostream>

#define N 10

int main( int argc, char **argv) {

	MPI_Init(&argc, &argv);
    
    int x[10];
    int rank;
    int size;
    float result[2];

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
	int elements_per_proc = N / size;

    float localAverage;

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            x[i] = rand() % 100;
            printf(" %d ", x[i]);
        }
    }
    int displs[size];
    int scounts[size];

    for (int i = 0; i < size; ++i) {
        displs[i] = i * elements_per_proc;
        scounts[i] = elements_per_proc;
    }

    MPI_Scatterv(x, scounts, displs, MPI_INT,
                 x, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    localAverage = 0;
    int count = 0;
    for (int i = 0; i < elements_per_proc; i++) {
        if (x[i] > 0) {
            localAverage += x[i];
            count++;
        }
    }

    float arr[2];
    arr[0] = localAverage;
    arr[1] = (float) count;
    MPI_Reduce(&arr, &result,
               2, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0)
        printf("\n average = %f \n", result[0]/result[1]);
		
	MPI_Finalize();
	return 0;
}