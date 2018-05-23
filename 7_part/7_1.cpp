/* 
Создать новую группу из процессов с номерами 8, 3, 9, 1, 6. 
Используя конструктор MPI_Comm_create(…), создать коммуникатор группы. 
На нулевом процессе созданной группы объявить и заполнить числовыми значениями 
одномерный массив вещественных чисел и вывести его на экран. 
Используя функцию MPI_Bcast передать массив всем процессам созданной группы. 
Полученные массивы вывести на экран. Передать полученный массив из последнего 
процесса новой группы на нулевой процесс исходной группы. Выполнить программу на 10 процессах.
*/
#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>
#include <cstdlib>

using namespace std;

#define size 10

void printArr(int a[], int s) {
    for (int i = 0; i < s; i++) {
        cout<< a[i] <<" ";
    } 
    cout<<"\n";
}

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int process_id, process_size;
    MPI_Status status;

    int newGroupProcessId[5] = {8, 3, 9, 1, 6};

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);
    
    MPI_Group wgroup, group1;
    MPI_Comm_group(MPI_COMM_WORLD, &wgroup);
    MPI_Group_incl(wgroup, 5, newGroupProcessId, &group1);

    MPI_Comm comm1, local;
    MPI_Comm_create(MPI_COMM_WORLD, group1, &comm1);

    int rank1=-1;

    int a[size];

    if (comm1 != MPI_COMM_NULL)  {
        MPI_Comm_rank(comm1, &rank1);
    }

    if (rank1==0 && comm1 != MPI_COMM_NULL) {
        for (int i = 0; i < size; i++) {
            a[i] = i + 1;
        }
        printArr(a, size);
        MPI_Bcast(&a, size, MPI_INT, 0, comm1);
    }

    if (rank1 != -1 && comm1 != MPI_COMM_NULL) {
      MPI_Bcast(&a, size, MPI_INT, 0, comm1);
      printf("%d - rank for print array\n", rank1);
      printArr(a, size);
      if (rank1 == 4) {
          MPI_Send(a, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }

    if (process_id == 0 ) {
      MPI_Recv(a, size, MPI_INT, 6, 0, MPI_COMM_WORLD, &status);
      printf("\n\n\n RECV DATA to 0");
      printArr(a, size);
    }


    if (comm1 != MPI_COMM_NULL) {
      MPI_Comm_free(&comm1);
    }

    MPI_Group_free(&group1);

    MPI_Finalize();
    return 0;
}
