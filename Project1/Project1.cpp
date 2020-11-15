
#include <iostream>
#include "MatrixMul.h"
#include <amp.h>
#include <ctime>
#include <time.h>
#include <chrono>
#include <vector>
#include <omp.h>
using namespace concurrency;
using namespace std;

int main()
{
	long aRow, aCol, bRow, bCol;
	float* aFlatMatrix;
	float* bFlatMatrix;

	std::cout << "Ooi Yee Jing - CS205 Project1\n";
	std::cout << "1. Manually Enter Matrix\n";
	std::cout << "2. Auto-Generate Matrix\n";

	int option;
	cin >> option;
	// Gather input 
	if ((option == 1) || (option == 2))
	{
		if (option == 1) {
		}
		else {
			aRow = 10;
			aCol = 10;
			bRow = 10;
			bCol = 10;

			srand(time(NULL));
			int MAX_RAND = 50;
			long sum_AMatrix_Length = aRow * aCol;
			aFlatMatrix = new float[sum_AMatrix_Length];
			float x = 1;
			for (long i = 0; i < sum_AMatrix_Length; i++)
				aFlatMatrix[i] = static_cast <float> (rand()) / (static_cast <float> (MAX_RAND));;

			x = 1;
			long sum_BMatrix_Length = bRow * bCol;
			bFlatMatrix = new float[sum_BMatrix_Length];
			for (long i = 0; i < sum_BMatrix_Length; i++)
				bFlatMatrix[i] = static_cast <float> (rand()) / (static_cast <float> (MAX_RAND));;
		}
	}
	else {
		std::cout << "Wrong  input";
		return 1;
	}

	cout << "Number of elements in each matrixA: " << (aRow * aCol) << endl;
	cout << "Number of elements in each matrixB: " << (bRow * bCol) << endl << endl;

	// Initialize MatrixMul
	MatrixMul* matrixMul = new MatrixMul(aRow, aCol, bRow, bCol, aFlatMatrix, bFlatMatrix);

	cout << "Result: " << endl;
	auto start = std::chrono::steady_clock::now();
	std::cout << "Multiply Calculation: " << endl;
	matrixMul->MultiplyWithoutAMP();
	matrixMul->printProductMatrix();
	auto end = std::chrono::steady_clock::now();
	std::cout << endl;
	//std::cout
	//	<< "Multiply Calculation took "
	//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;

	start = std::chrono::steady_clock::now();
	std::cout << "Multiple Parallel: " << endl;
	matrixMul->MultiplyParallel(matrixMul->getAMatrix(), matrixMul->getBMatrix(), matrixMul->getProductMatrix());
	matrixMul->printProductMatrix();
	end = std::chrono::steady_clock::now();
	std::cout << endl;
	//std::cout
	//	<< "Multiple Parallel took "
	//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;

	start = std::chrono::steady_clock::now();
	std::cout << "Multiply With AMP: " << endl;
	matrixMul->MultipleWithAMP();
	matrixMul->printProductMatrix();
	end = std::chrono::steady_clock::now();
	std::cout << endl;
	//std::cout
	//	<< "Multiply With AMP took "
	//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;

	start = std::chrono::steady_clock::now();
	std::cout << "Multiple With OpenBlasFloat: " << endl;
	matrixMul->MultipleWithOpenBlasFloat();
	matrixMul->printProductFlat();
	end = std::chrono::steady_clock::now();
	std::cout << endl;
	//std::cout
	//	<< "Multiple With OpenBlasFloat took "
	//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;



}