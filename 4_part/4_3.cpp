#include <mpi.h>
#include <vector>
#include <ctime>
#include <iostream>

#define matrix_size 8
#define vector_size 4
#define N 10

std::vector<std::vector<float>>A(matrix_size);
std::vector<float>X, aInter, inter, ak, rInter;

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
		result += aInter.at(i) * X.at(i);
		std::cout << aInter.at(i) << " ";
	}
	std::cout << "multiplication result = " << result << " process_id = " << process_id << std::endl;
}

int main(int argc, char **argv) {

	int process_id, size, root = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	X.resize(vector_size);
	aInter.resize(vector_size);
	inter.resize(vector_size);
	rInter.resize(vector_size);

	if (process_id == 0) {
		
		get_fill();
		print_vector();
		
		for (int i = 0; i < matrix_size; i++) {
			for (int j = 0; j < matrix_size; j++) {
				ak.push_back(A.at(i).at(j));
			}
		}
		for (int i = 0; i < ak.size(); i++) {
			std::cout << ak.at(i) << " ";
		}
		std::cout << std::endl << "------------" << std::endl;
	}

	MPI_Scatter(ak.data(), vector_size, MPI_FLOAT, aInter.data(), vector_size, MPI_FLOAT, root, MPI_COMM_WORLD);
	
	MPI_Bcast(&X.front(), vector_size, MPI_FLOAT, root, MPI_COMM_WORLD);
	multiplication(process_id);

	MPI_Gather(&result, 1, MPI_FLOAT, inter.data(), 1, MPI_FLOAT, root, MPI_COMM_WORLD);

	if (process_id == 0) {
		std::cout << "END = " << " ";
		for (auto i : inter)
			std::cout << i << " ";
		std::cout << std::endl;
	}

	MPI_Finalize();
	return 0;
}