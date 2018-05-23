/* 
Пусть матрицы вещественных чисел A, B заданы на нулевом процессе. 
Написать программу, в которой матрицы равными кусками с нулевого процесса 
пересылаются остальным процессам. Пересылка данных осуществляется функцией MPI_Send. 
Все процессы по формуле вычисляют свои элементы искомых матриц. Каждый процесс отправляет на нулевой 
процесс подсчитанные элементы матриц. В программе реализовать следующие операции вычисления матриц:
*/


#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>

using namespace std;

#define n 3
#define m 3

void print_vector(const int a[][m], int sizea, int sizeb) {
  for (int i = 0; i < sizea; i++) {
    for (int j = 0; j < sizeb; j++) {
      cout<< a[i][j] <<" ";
    }
    cout << "\n";
  }
}

int main( int argc, char **argv) { 
  
  MPI_Init(&argc, &argv);

  int rank, process_size;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_size);

  int count_size = 0;

  if (rank == 0) {
    int x[n][m] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int y[n][m] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    for (int i = 1; i < process_size; i++) {
      MPI_Send(x,n * m, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(y,n * m, MPI_INT, i, 1, MPI_COMM_WORLD);
    }
   
    int a[n][m];

    MPI_Recv(a, n * m, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
    print_vector(a, n, m);

    int mult[n][m];

    MPI_Recv(mult, n * m, MPI_INT, 2, 4, MPI_COMM_WORLD, &status);
    
    print_vector(mult, n, m);
  
  } else {

    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &count_size);
    
    int size = (int)sqrt(count_size);
    int x[size][size], y[size][size];
    
    MPI_Recv(x, count_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(y, count_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    
    if (rank == 1) {
      int z[size][size];
      
      for (int j = 0; j < size; j++) {
        for (int q = 0; q < size; q++) {
          z[j][q] = x[j][q] * y[j][q];
        }
      }
      MPI_Send(z, count_size, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    else if (rank == 2) {
      int z[size][size];
      
      for (int j = 0; j < size; j++) {
        for (int q = 0; q < size; q++) {
          z[j][q] = 0;
      
          for (int s = 0; s < size; s++) {
            z[j][q] += x[j][s] * y[s][q];
          }
        }
      }
      MPI_Send(z, count_size, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }
    printf("rank = %d; size = %d\n", rank, count_size);
  }

  MPI_Finalize();
  return 0;
}
