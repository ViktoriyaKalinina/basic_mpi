// Реализовать при помощи операций MPI_Send и MPI_Recv коллективный сбор данных - 
// операцию MPI_Gather. 

#include <mpi.h>
#include <iostream>

#define N 10

int main( int argc, char **argv) {
    
	MPI_Init(&argc, &argv);
    
	int x[10];
  int rank, ProcNum;

  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int elements_per_proc = N / ProcNum;
  int *subarr1 = new int[elements_per_proc];

    if (rank == 0) {
			printf("Send: ");
        for (int i = 0; i < N; i++) {
            x[i] = rand() % 100;
						printf(" %d ", x[i]);
        }
        printf("\n");
    }

    if (rank == 0) {
        MPI_Sendrecv(x, elements_per_proc, MPI_INT, 0, 0, subarr1, elements_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = elements_per_proc; i < N; i += elements_per_proc) {
          MPI_Send(x + i, elements_per_proc, MPI_INT, i / elements_per_proc, 0, MPI_COMM_WORLD);
        }

    } else {
      MPI_Recv(subarr1, elements_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < elements_per_proc; i++) {
      printf(" %d ", subarr1[i]);
    }

    printf(" : from process %d\n", rank);
    
	MPI_Send(subarr1, elements_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);

    int newArr[elements_per_proc];

    if (rank == 0) {
			printf("Recv: ");
        for (int i = 0; i < ProcNum; ++i) {
            MPI_Recv(newArr, elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < elements_per_proc; ++j) {
                printf(" %d ", newArr[j]);
            }
        }
    }

	MPI_Finalize();
	return 0;
}


