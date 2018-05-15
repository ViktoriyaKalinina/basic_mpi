#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>


using namespace std;

#define size1 3
#define size2 3

void printArr(const int a[][size2], int sizea, int sizeb) {
    for (int i=0; i<sizea; i++) {
        for (int j=0; j<sizeb; j++) {
            cout<<a[i][j]<<" ";
        }
        cout << "\n";
    }
}

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int process_id, process_size;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);

    int count_message_size = 0;

    if (process_id == 0) {
        int x[size1][size2] = {{1,2, 3}, {4,5 ,6}, {7, 8,9}};
        int y[size1][size2] = {{1,2, 3}, {4,5 ,6},{7, 8,9}};

        for (int i=1; i<process_size; i++) {
            MPI_Send(x,size1 * size2, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(y,size1 * size2, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        int a[size1][size2];

        MPI_Recv(a, size1 * size2, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
        cout<<"\n\na = ";
        printArr(a, size1, size2);

        int mult[size1][size2];
        MPI_Recv(mult, size1 * size2, MPI_INT, 2, 4, MPI_COMM_WORLD, &status);
        cout<<"\n\nb = ";
        printArr(mult, size1, size2);
    } else  {
        for (int i=0; i<process_size; i++) {
            MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count_message_size);
            int size = (int)sqrt(count_message_size);
            int x[size][size], y[size][size];
            MPI_Recv(x, count_message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(y, count_message_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            switch (process_id) {
                case  1: {
                    int z[size][size];
                    for (int j = 0; j< size; j++) {
                        for (int q = 0; q< size; q++) {
                            z[j][q] = x[j][q] * y[j][q];
                        }
                    }
                    MPI_Send(z, count_message_size, MPI_INT, 0, 3, MPI_COMM_WORLD);
                    break;
                }
                case 2: {
                    int z[size][size];
                    for (int j = 0; j< size; j++) {
                        for (int q = 0; q< size; q++) {
                            z[j][q] = 0;
                            for (int s = 0; s < size; s++) {
                                z[j][q] += x[j][s] * y[s][q];
                            }
                        }
                    }
                    MPI_Send(z, count_message_size, MPI_INT, 0, 4, MPI_COMM_WORLD);
                    break;
                }
            }
            printf("process id = %d; message_size = %d\n", process_id, count_message_size);
        }
    }

    MPI_Finalize();
    return 0;
}
