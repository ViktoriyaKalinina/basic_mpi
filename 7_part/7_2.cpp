#include <iostream>
#include <cmath>
#include "mpi.h"
#include <stdio.h>
#include <cstdlib>

using namespace std;

#define size 10
#define n 4
#define m 3

void printArr(int a[], int s) {
    for (int i=0; i<s; i++) {
        cout<< a[i] <<" ";
    } 
    cout<<"\n";
}

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int process_id, process_size;
    MPI_Status status;

    int newGroupProcessId[4][3] = {
        {0,1,2},
        {3,4,5},
        {6,7,8},
        {8,10,11}
    };

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);
    
    MPI_Group wgroup, g1, g2,g3,g4;
    MPI_Comm_group(MPI_COMM_WORLD, &wgroup);
    MPI_Group_incl(wgroup, m, newGroupProcessId[0], &g1);
    MPI_Group_incl(wgroup, m, newGroupProcessId[1], &g2);
    MPI_Group_incl(wgroup, m, newGroupProcessId[2], &g3);
    MPI_Group_incl(wgroup, m, newGroupProcessId[3], &g4);

    MPI_Comm cm1, cm2, cm3, cm4, intercomm1, intercomm2, local1, local2;
    MPI_Comm_create(MPI_COMM_WORLD, g1, &cm1);
    MPI_Comm_create(MPI_COMM_WORLD, g2, &cm2);
    MPI_Comm_create(MPI_COMM_WORLD, g3, &cm3);
    MPI_Comm_create(MPI_COMM_WORLD, g4, &cm4);

    int rank1 = -1;
    int rank2 = -1;
    int rank3 = -1;
    int rank4 = -1;

    int a[size];

    if (cm1 != MPI_COMM_NULL)  {
        MPI_Comm_rank(cm1, &rank1);
        for (int i = 0; i<m; i++) {
            a[i] = 1;
        }
    }
    if (cm2 != MPI_COMM_NULL)  {
        MPI_Comm_rank(cm2, &rank2);
            for (int i = 0; i<m; i++) {
                a[i] = 2;
            }
    }
    if (cm3 != MPI_COMM_NULL)  {
        MPI_Comm_rank(cm3, &rank3);
            for (int i = 0; i<m; i++) {
                a[i] = 3;
            }
    }
    if (cm4 != MPI_COMM_NULL)  {
        MPI_Comm_rank(cm4, &rank4);
            for (int i = 0; i<m; i++) {
                a[i] = 4;
            }
    }

    int gathered_arr[m][m];

    if (cm1 != MPI_COMM_NULL)
        MPI_Gather(&a, m, MPI_INT, gathered_arr, m, MPI_INT, 0, cm1);
    if (cm2 != MPI_COMM_NULL)
        MPI_Gather(&a, m, MPI_INT, gathered_arr, m, MPI_INT, 0, cm2);
    if (cm3 != MPI_COMM_NULL)
        MPI_Gather(&a, m, MPI_INT, gathered_arr, m, MPI_INT, 0, cm3);
    if (cm4 != MPI_COMM_NULL)
        MPI_Gather(&a, m, MPI_INT, gathered_arr, m, MPI_INT, 0, cm4);

    if (rank1 == 0 || rank2 == 0 || rank3 == 0 || rank4 == 0) {
        for (int i = 0; i< m; i++) {
            for (int j=0; j<m;j++) {
                printf("%d ", gathered_arr[i][j]);
            }
            printf("\n");
        }
    }
    int rlead;
    int tag = 777;
    if (cm1!=MPI_COMM_NULL && rank1 == 0) { local1=cm1; rlead=1;}
    if (cm2!=MPI_COMM_NULL && rank2 == 0) { local1=cm1; rlead=0;}
        
    //intercomm1 create
    if (rank1 == 0 || rank2 == 0) {
        MPI_Intercomm_create(local1, 0, MPI_COMM_WORLD, rlead, tag, &intercomm1);
    }

    if (rank1 == 0 && intercomm1 != MPI_COMM_NULL) {
        MPI_Send(&gathered_arr, m*m, MPI_INT, 1, 20, intercomm1);
    }
    if (cm2 != MPI_COMM_NULL && rank2 == 0) {
        int gath_new[m][m];
        MPI_Recv(&gath_new, m*m, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, intercomm1, MPI_STATUSES_IGNORE);
    }


    if (cm1 != MPI_COMM_NULL) {
        MPI_Comm_free(&cm1);
    }
    if (cm2 != MPI_COMM_NULL) {
        MPI_Comm_free(&cm2);
    }
    if (cm3 != MPI_COMM_NULL) {
        MPI_Comm_free(&cm3);
    }
    if (cm4 != MPI_COMM_NULL) {
        MPI_Comm_free(&cm4);
    }
    MPI_Group_free(&g1);
    MPI_Group_free(&g2);
    MPI_Group_free(&g3);
    MPI_Group_free(&g4);

    MPI_Finalize();
    return 0;
}
