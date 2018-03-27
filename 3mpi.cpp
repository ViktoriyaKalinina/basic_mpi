/* 3) Написать программу, запустить ее на 2х процессах. 
На нулевом процессе задать массив а из 10 элементов, 
значения сгенерировать случайным образом. 
Переслать этот массив целиком первому процессу с 
помощью функции MPI_Send. Распечатать на первом процессе принятый массив. */

#include "mpi.h"
#include "iostream"
#define N 30

int main(int argc, char** argv)
{
  int size, proc;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (proc == 0)
  {
    printf("Process -  %d \n", proc);
    int a[N];
    for (int i = 0; i < 10; i++)
      a[i] = rand() % N;
    printf("Send array, process %d \n", proc);
    for (int i = 1; i < size; i++)
      MPI_Send(&a, 10, MPI_INT, i, 99, MPI_COMM_WORLD);
  }
  else
  {
    printf("Process -  %d \n", proc);
    int b[N];
    printf("%d process print array \n", proc);
    MPI_Recv(&b, 10, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
    for (int i = 0; i < 10; i++)
      printf("b[%d]=%d ", i, b[i]);
  }
  MPI_Finalize();
}

// mpic++ 3mpi.cpp     
// mpiexec -n 2 ./a.out
