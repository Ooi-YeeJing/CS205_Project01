
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

	std::cout << "Ooi Yee Jing - Hello World!\n";
	std::cout << "1. Manually Enter Matrix\n";
	std::cout << "2. Auto-Generate Matrix\n";

	int option;
	cin >> option;
	// Gather input 
	if ((option == 1) || (option == 2))
	{
		if (option == 1) {
			std::cout << "A Row:"; cin >> aRow;
			std::cout << "A Column:"; cin >> aCol;
			std::cout << "B Row:"; cin >> bRow;
			std::cout << "B Column:"; cin >> bCol;

			if (aCol != bRow)
			{
				std::cout << "Wrong input";
				return 1;
			}

			char temp;
			long sum_AMatrix_Length = aRow * aCol;

			std::cout << "Matrix B["<< sum_AMatrix_Length <<"]:";
			aFlatMatrix = new float[sum_AMatrix_Length];
			for (long i = 0; i < sum_AMatrix_Length; i++) {
				cin >> temp;
				if (isdigit(temp))
					aFlatMatrix[i] = temp;

				else {
					std::cout << "Wrong input";
					return 1;
				}
			}

			long sum_BMatrix_Length = bRow * bCol;

			std::cout << "Matrix A[" << sum_BMatrix_Length << "]:";
			bFlatMatrix = new float[sum_BMatrix_Length];
			for (long i = 0; i < sum_BMatrix_Length; i++) {
				cin >> temp;
				if (isdigit(temp))
					bFlatMatrix[i] = temp;
				else {
					std::cout << "Wrong input";
					return 1;
				}
			}
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

	

	// Initialize MatrixMul
	MatrixMul* matrixMul = new MatrixMul(aRow, aCol, bRow, bCol, aFlatMatrix, bFlatMatrix);

	auto start = std::chrono::steady_clock::now();
	cout << "MultiplyWithOutAMP" << endl;
	matrixMul->MultiplyWithoutAMP();
	auto end = std::chrono::steady_clock::now();
	std::cout
		<< "MultiplyWithOutAMP took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl;

	matrixMul->printProductMatrix();

	start = std::chrono::steady_clock::now();
	cout << "MultiplyWithoutAMPParallel" << endl;
	matrixMul->MultiplyWithoutAMPParallel(matrixMul->getAMatrix(), matrixMul->getBMatrix(), matrixMul->getProductMatrix());
	end = std::chrono::steady_clock::now();
	std::cout
		<< "MultiplyWithoutAMPParallel took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl;

	matrixMul->printProductMatrix();


	start = std::chrono::steady_clock::now();
	cout << "MultipleWithAMP" << endl;
	matrixMul->MultipleWithAMP();
	end = std::chrono::steady_clock::now();
	std::cout
		<< "MultipleWithAMP took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl;

	matrixMul->printProductMatrix();


	start = std::chrono::steady_clock::now();
	cout << "MultipleWithOpenBlasFloat" << endl;
	matrixMul->MultipleWithOpenBlasFloat();
	end = std::chrono::steady_clock::now();
	std::cout
		<< "MultipleWithOpenBlasFloat took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl;

	matrixMul->printProductFlat();

}