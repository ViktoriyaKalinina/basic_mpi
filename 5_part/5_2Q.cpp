// двумерная декомпозиция А на 4 процесса
// 0 2
// 1 3

#include <stdio.h> 
#include <stdlib.h> 
#include "mpi.h"

#define N 10

int main( int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int A[8][8], alocal[4][4]; 
		int i, j, r, rank, size;
    
    MPI_Datatype stype, t[2], vtype; 
    MPI_Aint displs[2];

    int blklen[2], sendcount[4], sdispls[4];

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size ); 
		
		if (size != 4){
    	fprintf(stderr, "Запустите для 4 процессов \n"); 
			MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0){

    for (j = 0; j < 8; j++){
    	for (i = 0; i < 8; i++){
    		A[i][j] = (1.0 + i / 10.0 + j / 100.0);
			}
		}
    
    MPI_Type_vector(4, 4, 8, MPI_DOUBLE, &vtype);

    t[0] = vtype;
    t[1] = MPI_UB;

    displs[0] = 0; 
		displs[1] = 4 * sizeof(double);
    blklen[0] = 1;
    blklen[1] = 1;

    MPI_Type_struct( 2, blklen, displs, t, &stype );
    MPI_Type_commit( &stype );

    sendcount[0] = 1; 
    sendcount[1] = 1;
    sendcount[2] = 1;
    sendcount[3] = 1;

    sdispls[0] = 0;
    sdispls[1] = 1;
    sdispls[2] = 8;
    sdispls[3] = 9;

    MPI_Scatterv(&A[0][0], sendcount, sdispls, stype, &alocal[0][0], 4*4, MPI_DOUBLE, 0, MPI_COMM_WORLD);

} else {
    
		MPI_Scatterv(0, 0, 0, MPI_DATATYPE_NULL, &alocal[0][0], 4*4, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (r = 0; r < size; r++) { 
			if (rank == r){
        printf( "Вывод для процесса  %d \n", r ); 

				for (j = 0; j < 4; j++){
            for (i = 0; i < 4; i++) {
							printf( "%d ", alocal[i][j] ); 
						  printf(" ");
				}
		  }
		fflush(stdout);
		}
	}
}
MPI_Finalize(); 
return 0;
}