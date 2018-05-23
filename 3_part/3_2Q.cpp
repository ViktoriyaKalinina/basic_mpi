/* 
вычислить скалярное произведение двух векторов. 
Для распределения элементов вектора x по процессам использовать функцию MPI_Scatter. 
Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum.
*/

#include <iostream>
#include <vector>
#include <mpi.h>

	std::vector<float> X, Y, x_rcbuf, y_rcbuf, p_vector;
	int numbElemEachProc = 1; 
	float sum = 0;

	void print_vector() {
	std::cout << "X: ";
	for (auto x : X)
		std::cout << x << " ";
	
	std::cout << std::endl << "Y: ";
	for (auto y : Y)
		std::cout << y << " ";
	std::cout << std::endl;
}

	void get_fill_vector(int SIZE) {
	srand(time(0));
	for (int i = 0; i < SIZE; i++) {
		X.push_back(rand() % 10);
		Y.push_back(rand() % 10);
	}
}

	void disribution_data(int numberOfProcesses, int root) {
	MPI_Scatter(X.data(), numbElemEachProc, MPI_FLOAT, x_rcbuf.data(), numbElemEachProc, MPI_FLOAT, root, MPI_COMM_WORLD);
	MPI_Scatter(Y.data(), numbElemEachProc, MPI_FLOAT, y_rcbuf.data(), numbElemEachProc, MPI_FLOAT, root, MPI_COMM_WORLD);
}

	int main(int argc, char** argv) {
	int rank, size, root = 0, SIZE;
	float psum = 0, nmult = 0;
	
	MPI_Init(&argc, &argv);
	
	x_rcbuf.resize(numbElemEachProc);
	y_rcbuf.resize(numbElemEachProc);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	SIZE = size * numbElemEachProc;

	if (rank == 0) {
		get_fill_vector(SIZE);
		print_vector();
	}
	
	disribution_data(size, root);
	for (int i = 0; i < numbElemEachProc; i++) {
		psum += x_rcbuf.at(i) * y_rcbuf.at(i);
	}
	
	for (int i = 0; i < p_vector.size(); i++) {
		std::cout << p_vector.at(i) << " ";
	}
	
	MPI_Reduce(&psum, &nmult, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		std::cout <<"SUM = "<< nmult << std::endl;
	}

	MPI_Finalize();
	return 0;
}