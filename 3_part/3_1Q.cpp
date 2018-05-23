/* 
вычислить норму вектора x 
Для распределения элементов вектора x по процессам использовать функцию MPI_Scatter. 
Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum.
*/

#include <iostream>
#include <mpi.h>
#include <vector>

#define N 5

void print_vector(int *x, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", x[i]);
	}
	printf("\n");
}

int* vectors_abs(int *vec, int size) {
	int* res = new int[N];
	for (int i = 0; i < size; i++) {
		res[i] = abs(vec[i]);
	}
	return res;
}

int main(int argc, char** argv) {

	int vec[N];
	int process_id, size, resultVar = 0;
	int *vbuf, *result;

	for (int i = 0; i < N; i++){
		vec[i] = rand() % N;
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	vbuf = new int[N/size];
	result = new int[N/size];

	if (N % size != 0) {
		printf("%s\n", "Number of process  % N == 0");
	
		MPI_Finalize();
		return 0;
	}

	MPI_Scatter(vec, N/size, MPI_INT, vbuf, N/size, MPI_INT, 0, MPI_COMM_WORLD);
	
	vbuf = vectors_abs(vbuf, N/size);
	
	MPI_Reduce(vbuf, result, N/size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (process_id == 0) {
		for (int i = 0; i < N/size; i++){
			resultVar += result[i];
		}
		
		printf("%d - norm ", resultVar);
		print_vector(vec, N);
	}
	
	MPI_Finalize();
	return 0;
}