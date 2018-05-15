#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>
#include <cstdlib>

using namespace std;

#define a 10
#define b 5

void printArr(const int g[][b], int sizea, int sizeb) {
    for (int i=0; i<sizea; i++) {
        for (int j=0; j<sizeb; j++) {
            cout<<g[i][j]<<" ";
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

    int globalData[a][b], globalData1[a][b];
    int localData[a], localData1[a];
    
    if (process_id == 0) {
        for (int i =0; i<a; i++) {
            for (int j =0; j< b; j++) {
                globalData[i][j] = (int)(-100 + rand() % 200);
                globalData1[i][j] = (int)(-100 + rand() % 200);
            }
        }
        printf("Process - %d defined global data\n", process_id);
    }


    MPI_Scatter(globalData, a, MPI_INT, &localData, a, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(globalData1, a, MPI_INT, &localData1, a, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Local 
    int localNew[a];
    for (int i=0; i < a; i++) {
        localNew[i] = localData[i] * localData1[i];
    }
    int glob[a][b];
    MPI_Gather(&localNew, a, MPI_INT, glob, a, MPI_INT, 0, MPI_COMM_WORLD);

    if (process_id == 0) {
        printArr(glob, a, b);
    }

    MPI_Finalize();
    return 0;
}
