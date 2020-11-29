#include <iostream>
#include <omp.h>
#include <vector>
#include <thread>

unsigned int arrSize;
std::vector<std::vector<double>> C;
std::vector<double> A;
double result = DBL_MIN;
int result_i = -1, result_j = -1;


void summary(int index_i) {
	// int index_i = (int)param;
	for (unsigned int j = index_i; j < arrSize; j++) {
		if (j == index_i) {
			C[index_i][j] = A[j];
		}

		else {
			C[index_i][j] = C[index_i][j - 1] - A[j] * pow(-1, j + index_i);
		}

		if (result < C[index_i][j]) {
			result = C[index_i][j];
			result_i = index_i;
			result_j = j;
		}
	}
}

int main() {
	// Init rand. 
	srand(time(NULL));

	// Enter size of array.
	std::cout << "Enter vector size: ";
	std::cin >> arrSize;

	// Check entering.
	if (arrSize <= 10 || arrSize > INT32_MAX) {
		std::cout << "Wrong size of vector = " << arrSize << "\n";
		return 1;
	}

	A.resize(arrSize, 0);
	C.resize(arrSize, std::vector<double>(arrSize, DBL_MIN));

	// Random array values.
	for (unsigned int i = 0; i < arrSize; ++i) {
		A[i] = 1 + rand() % 100000;
	}

	for (unsigned int i = 0; i < arrSize; i++) {
		std::cout << "A[" << i << "] = " << A[i] << "\n";
	}

	// Make parallel computations.
#pragma omp parallel for shared(result, result_i, result_j)
	for (int i = 0; i < arrSize - 1; i++) {
		summary(i);
	}

	// Print result.
	std::cout << "Result indexes = " << result_i << " " << result_j << "\n";

	return 0;
}