/*
// 11. Определить индексы i, j (i ≠ j), для которых выражение
// А[i] – A[i+1] + A[i+2] – A[i+3] +- … A[j] имеет максимальное значение.
// Входные данные: массив чисел А, произвольной длины большей 10.
// Количество потоков не является параметром задачи.
*/


#include <iostream>
#include <vector>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

unsigned int arrSize;
std::vector<std::vector<double>> C;
std::vector<double> A;


void* summary(void* param) {
	int index_i = (int)param;
	for (unsigned int j = index_i; j < arrSize; j++) {
		if (j == index_i) {
			C[index_i][j] = A[j];
		}

		else {
			C[index_i][j] = C[index_i][j - 1] + A[j] * pow(-1, j);
		}
	}
	return NULL;
}

int main() {
	std::cout << "Enter vector size: ";
	std::cin >> arrSize;

	if (arrSize <= 10 || arrSize > 10000) {
		std::cout << "Wrong size of vector = " << arrSize << "\n";
		return 1;
	}

	A.resize(arrSize, 0);
	C.resize(arrSize, std::vector<double>(arrSize, DBL_MIN));


	for (unsigned int i = 0; i < arrSize; ++i) {
		// A[i] = rand();
		A[i] = i;
	}

	for (unsigned int i = 0; i < arrSize; i++) {
		std::cout << "A[" << i << "] = " << A[i] << "\n";
	}

	int threadNumber = arrSize - 1;
	std::vector<pthread_t> thread(threadNumber);

	for (int i = 0; i < threadNumber; i++) {
		pthread_create(&thread[i], NULL, summary, (void*)(i));
	}

	for (int i = threadNumber - 1; i > 0; i--) {
		pthread_join(thread[i], NULL);
	}

	double temp = DBL_MIN;
	int result_i = -1, result_j = -1;
	for (unsigned int i = 0; i < arrSize; i++) {
		for (unsigned int j = 0; j < arrSize; j++) {
			if (temp < C[i][j]) {
				temp = C[i][j];
				result_i = i;
				result_j = j;
			}
		}
	}

	std::cout << "Result indexes = " << result_i << " " << result_j << "\n";

	return 0;
}