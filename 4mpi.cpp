/* 4) Используя блокирующую операцию передачи сообщений (MPI_Send и MPI_Recv) 
выполнить пересылку данных одномерного массива, из процесса с номером 1 на 
остальные процессы группы. На процессах получателях до выставления функции приема 
(MPI_Recv) определить количество принятых данных (MPI_Probe). Выделить память 
под размер приемного буфера, после чего вызвать функцию MPI_Recv. 
Полученное сообщение выдать на экран. */
#include "mpi.h"
#include "iostream"
#define N 5

int main(int argc, char **argv)
{
  int size, rank;
  int receive, send;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int A[N];
  int B[N];

  for(int i = 0; i < N; i++)
  {
    A[i] = rand() % N;
  }

  receive = 0;
  send = 0;
  if (rank == 0) {
    
    for (int i = 0; i < N; i++)
    {
      send = A[i];
      MPI_Send(&send, N, MPI_INT, 1, 5, MPI_COMM_WORLD);
      printf("process = %d a = %d b = %d \n", rank, receive, send);
    }
  }
  else if (rank == 1) 
  {
    MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    for (int i = 0; i < N; i++)
    {
      receive = B[i];
      MPI_Recv(&receive, N, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
      printf("process=%d a=%d b=%d \n", rank, receive, send);  
    }
  }
  MPI_Finalize();
  return 0;
}
