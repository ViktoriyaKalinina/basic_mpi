/* 
Написать программу умножения матрицы на вектор z = Ax. 
Распределить матрицу А равномерно вдоль строк, вектор х передать всем процессам. 
После выполнения операции умножения матрицы на вектор на каждом процессе необходимо выполнить 
сбор полученных локальных частей результирующего вектора на один из процессов. 
Использовать функцию MPI_Bcast для рассылки вектора x, для распределения элементов матрицы использовать 
функцию MPI_Scatter, для сбора локальных частей результирующего вектора в глобальный вектор 
использовать функцию MPI_Gather.
*/

#include <mpi.h>
#include <vector>
#include <ctime>
#include <iostream>

#define matrix_size 3
#define vector_size 3
#define N 10

std::vector<std::vector<float>> A(matrix_size);
std::vector<float> X, a_rcvbuf, g_rcvbuf, a_sendbuf; 

float result;

void get_fill() {
	srand(time(0));
	for (int i = 0; i < matrix_size; i++) {
		for (int j = 0; j < matrix_size; j++) {
			A.at(i).push_back(rand() % N);
		}
	}

	for (int i = 0; i < vector_size; i++)
		X.at(i) = (rand() % N);
}

void print_vector() {
	std::cout << "matrix A: " << std::endl;
	for (int i = 0; i < matrix_size; i++) {
		for (int j = 0; j < matrix_size; j++) {
			std::cout << A.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "vector X: " << std::endl;
	for (auto x : X)
		std::cout << x << " ";
		std::cout << std::endl;
}

void multiplication(int process_id) {
	for (int i = 0; i < vector_size; i++) {
		result += a_rcvbuf.at(i) * X.at(i);
		std::cout << a_rcvbuf.at(i) << " ";
	}
	std::cout << "multiplication result = " << result << " process_id = " << process_id << std::endl;
}

int main(int argc, char **argv) {

	int process_id, size, root = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	X.resize(vector_size);
	a_rcvbuf.resize(vector_size);
	g_rcvbuf.resize(vector_size);

	if (process_id == 0) {
		
		get_fill();
		print_vector();
		
		for (int i = 0; i < matrix_size; i++) {
			for (int j = 0; j < matrix_size; j++) {
				a_sendbuf.push_back(A.at(i).at(j));
			}
		}
		for (int i = 0; i < vector_size; i++) {
			std::cout << a_sendbuf.at(i) << "\n ";
		}
		std::cout << std::endl << "\n " << std::endl;
	}

	MPI_Scatter(a_sendbuf.data(), vector_size, MPI_FLOAT, a_rcvbuf.data(), vector_size, MPI_FLOAT, root, MPI_COMM_WORLD);
	MPI_Bcast(&X.front(), vector_size, MPI_FLOAT, root, MPI_COMM_WORLD);
	
	multiplication(process_id);

	MPI_Gather(&result, 1, MPI_FLOAT, g_rcvbuf.data(), 1, MPI_FLOAT, root, MPI_COMM_WORLD);

	if (process_id == 0) {
		std::cout << "result for 0 process = " << " ";
		for (auto i : g_rcvbuf)
			std::cout << i << " ";
		std::cout << std::endl;
	}

	MPI_Finalize();
	return 0;
}