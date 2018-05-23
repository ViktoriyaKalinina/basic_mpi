/* На основе исходной группы, создать 4 новые группы процессов со своим коммуникатором. 
На каждом процессе каждой группы объявить одномерный массив целых чисел и заполнить его 
числовыми значениями, равными номеру группы. Используя коллективные операции, собрать 
локальные массивы на нулевых   процессах каждой группы. Полученные массивы выдать на экран. 
Используя интеркоммуникатор, выполнить обмен собранными массивами между  
первой – второй и третьей - четвертой группами.  
Полученные массивы выдать на экран. Выполнить программу на 12 процессах.	 
*/
#include "mpi.h"
#include <iostream>
#include <cstdlib>

using namespace std;


int main (int argc, char **argv)
{
  MPI_Init (&argc, &argv);

  int total_processes, main_rank;
  MPI_Comm_size( MPI_COMM_WORLD, &total_processes );
  MPI_Comm_rank( MPI_COMM_WORLD, &main_rank );

  MPI_Comm my_comm;

  int color = main_rank / 3;
  MPI_Comm_split(MPI_COMM_WORLD, color, main_rank % 3, &my_comm);

  int rank = -1;
  if (my_comm != MPI_COMM_NULL) {
    MPI_Comm_rank(my_comm, &rank);
  }
  int size = -1;
  if (my_comm != MPI_COMM_NULL) MPI_Comm_size(my_comm, &size);

  int mas[4] = {color, color, color, color};

  int com[12];
  MPI_Gather(&mas, 4, MPI_INT, &com, 4, MPI_INT, 0, my_comm);


  if (rank == 0) {
    cout << "from " << color << " group, com = ";
    for (int i = 0; i < 12; ++i) {
      cout << com[i] << " ";
    }
    cout << endl;
  }

  int tag = -1;
  int rlead = -1;

  if (color == 0) {
    tag = 333;
    rlead = 3;
  }
  if (color == 1) {
    tag = 333;
    rlead = 0;
  }
  if (color == 2) {
    tag = 999;
    rlead = 9;
  }
  if (color == 3) {
    tag = 999;
    rlead = 6;
  }

  MPI_Comm intercomm;
  MPI_Intercomm_create(my_comm, 0, MPI_COMM_WORLD, rlead, tag, &intercomm);

  if ((color == 0 || color == 2) && rank == 0) {
    MPI_Send(&com, 12, MPI_INT, 0, color, intercomm);
  }

  if ((color == 1 || color == 3) && rank == 0) {
    int buf[12];
    MPI_Recv(&buf, 12, MPI_INT, MPI_ANY_SOURCE, color - 1, intercomm, MPI_STATUS_IGNORE);

    cout << endl;
    cout << "From " << color << " group, buf = ";
    for (int i = 0; i < 12; ++i) {
      cout << buf[i] << " ";
    }
    cout << endl;
  }


  if (my_comm != MPI_COMM_NULL) MPI_Comm_free(&my_comm);

  MPI_Finalize();
  return 0;
}

