#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>
#include <cstdlib>

using namespace std;

#define a 100
#define b 5

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int process_id, process_size;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);

    int globalData[a][b];
    int localData[a];
    
    if (process_id == 0) {
        for (int i = 0; i < a; i++) {
            for (int j = 0; j < b; j++) {
                globalData[i][j] = (int)(-100 + rand() % 200);
            }
        }
        printf("Process - %d defined global data\n", process_id);
    }

    MPI_Scatter(globalData, a, MPI_INT, &localData, a, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Local 
    int sum = 0, sum_max;
    for (int i = 0; i < a; i++) {
        sum += abs(localData[i]);
    }
    printf("Process  - %d val = %d\n",process_id ,sum);


    MPI_Reduce(&sum, &sum_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (process_id == 0) {
        cout <<"Maximum = " <<sum_max;
    }
    MPI_Finalize();
    return 0;
}
