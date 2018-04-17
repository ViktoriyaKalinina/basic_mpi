// В массиве вещественных чисел вычислить минимальное значение и 
// глобальный индекс минимального элемента. Использовать функции 
// MPI_Scatter и MPI_Reduce с операцией MPI_MINLOC.

#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>
#include <cstdlib>

using namespace std;

#define size 100

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int process_id, process_size;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);

    int count = size/ process_size;
    int globalData[size];
    int localData[count];

    struct { 
        float value; 
        int   index; 
    } local_in, local_out; 

    
    if (process_id == 0) {
        for (int i =0; i<size; i++) {
            globalData[i] = (int)(-100 + rand() % 200);
            cout << globalData[i] << " ";
        }
        printf("Process - %d defined global data\n", process_id);
        printf("Processes count = %d, local_data count of massive = %d\n", process_size, size/process_size);
    }


    MPI_Scatter(globalData, size/process_size, MPI_INT, &localData, size/process_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Local in define from localData
    local_in.value = localData[0]; 
    local_in.index = 0; 
    for (int i=1; i < count; i++) {
        if (local_in.value > localData[i]) { 
            local_in.value = localData[i]; 
            local_in.index = i; 
        } 
    }
    local_in.index = process_id * count + local_in.index;
    MPI_Reduce(&local_in, &local_out, 1, MPI_FLOAT_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);


    if (process_id ==0) {
        float minval = local_out.value; 
        int minindex = local_out.index % size; 
        cout << "\n\nMIN = "<< minval << " INDEX = " << minindex;
    }


    MPI_Finalize();
    return 0;
}
