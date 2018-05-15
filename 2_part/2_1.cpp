#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>

#define A_ 2
#define B_ 3

using namespace std;

void printArr(int a[], int size) {
    for (int i=0; i<size; i++) {
        cout<< a[i] <<" ";
    } 
    cout<<"\n";
}

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int process_id, process_size;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);

    int count_message_size = 0;
    int size = 10;

    if (process_id == 0) {
        int x[size] = {1,2, 3, 4,5 ,6,7, 8,9, 11};
        int y[size] = {1,2, 3, 4,5 ,6,7, 8,9, 10};

        for (int i=1; i<process_size; i++) {
            MPI_Send(x,size, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(y,size, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        int a[size];

        MPI_Recv(a, size, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
        cout<<"a = ";
        printArr(a, size);

        int xy[size], yx[size];
        MPI_Recv(xy, size, MPI_INT, 2, 4, MPI_COMM_WORLD, &status);
        MPI_Recv(yx, size, MPI_INT, 2, 5, MPI_COMM_WORLD, &status);
        cout<<"\n\nd = ";
        printArr(xy, size);
        printArr(yx, size);

    } else  {
        for (int i=0; i<process_size; i++) {
            MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count_message_size);
            int x[count_message_size], y[count_message_size];
            MPI_Recv(x, count_message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(y, count_message_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            switch (process_id) {
                case  1: {
                    int z[count_message_size];
                    for (int j = 0; j< count_message_size; j++) {
                        z[j] = (A_ * x[j]) + (B_ * y[j]);
                    }
                    MPI_Send(z, count_message_size, MPI_INT, 0, 3, MPI_COMM_WORLD);
                    break;
                }
                case 2: {
                    int xy[count_message_size], yx[count_message_size];
                    for (int j = 0; j< count_message_size; j++) {
                        xy[j] = y[j];
                        yx[j] = x[j];
                    }
                    MPI_Send(xy, count_message_size, MPI_INT, 0, 4, MPI_COMM_WORLD);
                    MPI_Send(yx, count_message_size, MPI_INT, 0, 5, MPI_COMM_WORLD);
                    break;
                }
            }
            printf("process id = %d; message_size = %d\n", process_id, count_message_size);
        }
    }

    MPI_Finalize();
    return 0;
}
