#pragma once

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <amp.h>
#include <cblas.h>
#include <omp.h>
using namespace concurrency;
using namespace std;

class MatrixMul {
private:
	long aRow;
	long aCol;
	long bRow;
	long bCol;
	float* aMatrixFlat;
	float* bMatrixFlat;
	float* productFlat;
	float** aMatrix;
	float** bMatrix;
	float** productMatrix;

public:
	MatrixMul(long aRow, long aCol, long bRow, long bCol, float* aFlatMatrix, float* bFlatMatrix) {
		this->aRow = aRow;
		this->aCol = aCol;
		this->bRow = bRow;
		this->bCol = bCol;
		this->aMatrixFlat = aFlatMatrix;
		this->bMatrixFlat = bFlatMatrix;
		long r = aRow * bCol;
		this->productFlat = new float[r]{ 0 };
		this->productMatrix = new float* [aRow];

		// Convert 2 Dimensional vectors
		long i = 0;
		aMatrix = new float* [aRow];
		for (long r = 0; r < aRow; r++) {
			aMatrix[r] = new float[aCol] {0};
			for (long c = 0; c < aCol; c++) {
				aMatrix[r][c] = aFlatMatrix[i++];
			}
		}

		i = 0;
		bMatrix = new float* [bRow];
		for (long r = 0; r < bRow; r++) {
			bMatrix[r] = new float[bCol] {0};
			for (long c = 0; c < bCol; c++) {
				bMatrix[r][c] = bFlatMatrix[i++];
			}
		}
	}

	float** getAMatrix() {
		return aMatrix;
	}
	float** getBMatrix() {
		return bMatrix;
	}
	float** getProductMatrix() {
		return productMatrix;
	}

	void MultiplyWithoutAMP();
	void MultiplyParallel(float** aMatrix, float** bMatrix, float** product);
	void MultipleWithAMP();
	void MultipleWithOpenBlasFloat();

	// Print Output
	void printProductMatrix();
	void printProductFlat();
};


void MatrixMul::MultiplyWithoutAMP() {
	this->productMatrix = new float* [aRow] {0};
	for (long row = 0; row < aRow; row++) {
		this->productMatrix[row] = new float[bCol] {0};
		for (long col = 0; col < bCol; col++) {
			for (long inner = 0; inner < aCol; inner++) {
				this->productMatrix[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
			}
			//std::cout << this->productMatrix[row][col] << "  ";
		}
		//std::cout << "\n";
	}
}

void MatrixMul::MultiplyParallel(float** aMatrix, float** bMatrix, float** productMatrix) {
	int row, col, inner;
	productMatrix = new float* [aRow] {0};
	omp_set_num_threads(8);
	// declare shared and private variables for OpenMP threads
#pragma omp parallel shared (aMatrix, bMatrix, productMatrix) private(row, col, inner)
	{
		// static allocation of data to threads
#pragma omp for schedule(static)
		for (row = 0; row < aRow; row++) {
			productMatrix[row] = new float[bCol] {0};
			for (col = 0; col < bCol; col++) {
				for (inner = 0; inner < aCol; inner++) {
					productMatrix[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
				}
			}
			//std::cout << omp_get_thread_num() << "|" << omp_get_num_threads() << "\n";
		}
	}

	//for (long row = 0; row < aRow; row++) {
	//	for (long col = 0; col < bCol; col++) {
	//		std::cout << productMatrix[row][col] << "  ";
	//	}
	//	std::cout << "\n";
	//}
}

void MatrixMul::MultipleWithAMP() {

	array_view<float, 2> a(aRow, aCol, aMatrixFlat);
	array_view<float, 2> b(bRow, bCol, bMatrixFlat);
	array_view<float, 2> pd(aRow, bCol, productFlat);
	long mycol = aCol;
	parallel_for_each(pd.extent, [=](index<2> idx) restrict(amp) {
		long row = idx[0];
		long col = idx[1];
		for (long inner = 0; inner < mycol; inner++) {
			pd[idx] += a(row, inner) * b(inner, col);
		}
		});

	pd.synchronize();

	/*for (long row = 0; row < aRow; row++) {
		for (long col = 0; col < bCol; col++) {
			std::cout << pd(row, col) << "  ";
		}
		std::cout << "\n";
	}*/
}


void MatrixMul::MultipleWithOpenBlasFloat() {

	double alpha = 1.0;
	double beta = 0.0;
 
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, aRow, aRow, bRow, alpha, aMatrixFlat, aCol, bMatrixFlat, bCol, beta, productFlat, bCol);

	//int i = 0;
	//for (long row = 0; row < aRow; row++) {
	//	for (long col = 0; col < bCol; col++) {
	//		std::cout << productFlat[i++] << "  ";
	//	}
	//	std::cout << "\n";
	//}
}

void MatrixMul::printProductMatrix() {
	for (long row = 0; row < aRow; row++) {
		for (long col = 0; col < bCol; col++) {
			std::cout << this->productMatrix[row][col] << "  ";
		}
		std::cout << "\n";
	}
}

void MatrixMul::printProductFlat() {
	int i = 0;
	for (long row = 0; row < aRow; row++) {
		for (long col = 0; col < bCol; col++) {
			std::cout << this->productFlat[i++] << "  ";
		}
		std::cout << "\n";
	}
}