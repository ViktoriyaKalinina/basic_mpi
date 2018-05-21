#include <iostream>
#include <mpi.h>
#include <vector>

#define N 10

void print_vector(int *x, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", x[i]);
	}
	
	printf("\n");
}

int* absVector(int *vec, int size) {
	int* res = new int[10];
	for (int i = 0; i < size; i++) {
		res[i] = abs(vec[i]);
	}
	return res;
}

int main(int argc, char** argv) {

	int vec[10];
	int rank, size, resultVar = 0;
	int *vbuf, *result;

	for (int i = 0; i < N; i++)
	{
		vec[i] = rand() % N;
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	vbuf = new int[10 / size];
	result = new int[10 / size];

	if (10 % size != 0) {
	
		printf("%s\n", "Number of process  % 10 == 0");
	
		MPI_Finalize();
		return 0;
	}
	
	MPI_Scatter(vec, 10 / size, MPI_INT, vbuf, 10 / size, MPI_INT, 0, MPI_COMM_WORLD);
	
	vbuf = absVector(vbuf, 10 / size);
	MPI_Reduce(vbuf, result, 10 / size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (rank == 0) {
		for (int i = 0; i < 10 / size; i++)
	
		{
			resultVar += result[i];
		}
		
		printf("%d - vector norm ", resultVar);
		print_vector(vec, 10);
	
	}
	
	MPI_Finalize();
	return 0;

}