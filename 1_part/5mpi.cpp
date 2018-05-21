/* 5) Написать программу и запустить ее на p (= 2, 3, 4, 6, 8) процессах. 
На нулевом процессе объявить и задать массив из 12 элементов. С помощью функции 
MPI_Send разослать блоки массива на остальные процессы. 
Размер блока массива 12/p+1. 
В результате на нулевом процессе должны быть элементы массива с 0 до 12/p+1, 
на первом процессе с 12/p+1 до 2×(12/p+1), 
на 3м процессе с 2×(12/p+1) до 3×(12/p+2) и т.д. 
Вывести элементы массива на экран на каждом процессе. */

#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>

using namespace std;

int *cut_array(int size, int process, int source[]) {
  int *result = new int[size];
  int count = 0;
  int head = (process - 1) * size;
  int tail = (process) * size;
  for (int i = head; i < tail; i++) {
    result[count] = source[i];
    count++;
  }
  return result;
}

int main( int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, total_processes;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

  int message_size;

  if (rank == 0) {
    int array[12] = { 1, 2, 3, 4, 5 ,6 ,7 ,8, 9, 10, 11, 12 };
    for (int i = 1; i < total_processes; i++) {
      int size = 12 / (total_processes + 1);
      int *message = cut_array(size, i, array);
      MPI_Send(message, size, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else  {
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &message_size);
    int array[message_size];
    MPI_Recv(array, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    printf("process id = %d; message_size = %d\n", rank, message_size);
    for (int i = 0; i < message_size; i++) {
      cout << array[i] << " ";
    }
    cout<<"\n";
  }

  MPI_Finalize();
  return 0;
}
