#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>

#define alfa 2
#define beta 3

using namespace std;

void print_vector(int *vector) {
  for (int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++) {
    cout << vector[i] <<" ";
  }
  cout<<"\n";
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int process_id, total_processes;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
  MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

  int count_message_size = 0;
  int size = 2;

  if (process_id == 0) {
    int x[] = { 1, 2 };
    int y[] = { 3, 4 };

    for (int i = 1; i < total_processes; i++) {
      MPI_Send(x, size, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(y, size, MPI_INT, i, 1, MPI_COMM_WORLD);
    }

    int a[size];

    MPI_Recv(a, size, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
    print_vector(a); // xi * alfa + yi * beta

    int xy[size], yx[size];

    MPI_Recv(xy, size, MPI_INT, 2, 4, MPI_COMM_WORLD, &status);
    MPI_Recv(yx, size, MPI_INT, 2, 5, MPI_COMM_WORLD, &status);
   
    print_vector(xy); // xi * yi ? {1*3}, {2*4} = {3, 8} 
    
    print_vector(yx); // xi <--> yi  1,2??

  } else {
    
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &count_message_size);
    int x[count_message_size], y[count_message_size];

    MPI_Recv(x, count_message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(y, count_message_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
   
    if (process_id == 1) {
      int z[count_message_size];
      
      for (int j = 0; j < count_message_size; j++) {
        z[j] = (alfa * x[j]) + (beta * y[j]);
      }
      MPI_Send(z, count_message_size, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    
    else if (process_id == 2) {
    
      int xy[count_message_size], yx[count_message_size];
     
      for (int i = 0; i < count_message_size; i++) {
        xy[i] = x[i] * y[i];
				y[i] = 0;
      }

      MPI_Send(xy, count_message_size, MPI_INT, 0, 4, MPI_COMM_WORLD);
			MPI_Send(yx, count_message_size, MPI_INT, 0, 5, MPI_COMM_WORLD);
    }
    printf("process_id = %d; size = %d\n", process_id, count_message_size);
  }

  MPI_Finalize();
  return 0;
}
