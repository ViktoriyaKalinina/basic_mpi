/* Написать mpi программу, печатающую "Hello, World!". Запустить программу на 4 процессах. */

#include <mpi.h>
#include <stdio.h>
 
int main (int argc, char* argv[])
{
  int rank, size;
 
  MPI_Init (&argc, &argv);
  printf( "Hello world!\n");
  MPI_Finalize();
  return 0;
}

// mpic++ 1mpi.cpp   
// mpiexec -n 4 ./a.out