#include<iostream>
#include<sstream>
#include<chrono>
using namespace std;

struct Matrix
{
	int a_rows;
	int a_columns;
	int b_rows;
	int b_columns;
	char ch[100];
	float floatInput;
	float * arr;
	string rowChange;

	float& operator[](int i)
	{
		return arr[i];
	}
};

int main()
{
	Matrix matrix;
	matrix.a_rows = 0;
	matrix.a_columns = 0;
	matrix.rowChange = ';';

	//Input Matrix A
	cout << "Enter Matrix A: " << endl;
	cin.getline(matrix.ch, 99);
	istringstream sinA(matrix.ch);

	while (sinA)
	{
		if (sinA >> matrix.floatInput)
		{
			//Get the number of columns in first row
			if (matrix.a_rows < 1)
				++matrix.a_columns;
		}
		else
		{
			if (sinA.str() != matrix.rowChange) {
				//Get the number of rows
				sinA.clear();
				char c = sinA.get();
				++matrix.a_rows;
			}
			else
				cout << "Invalid";
		}
	}

	Matrix* mtxA = new Matrix[matrix.a_rows];
	for (int i = 0; i < matrix.a_rows; ++i)
		mtxA[i].arr = new float[matrix.a_columns];

	//Getting input for matrix A
	sinA = istringstream(matrix.ch);
	int iA = 0;
	int jA = 0;
	while (sinA)
	{
		if (sinA >> matrix.floatInput)
		{
			mtxA[iA][jA] = matrix.floatInput;
			++jA;
		}
		else
		{
			sinA.clear();
			sinA.get();
			++iA;
			jA = 0;
		}
	}

	matrix.b_rows = 0;
	matrix.b_columns = 0;

	//Input Matrix B
	cout << "Enter Matrix B: " << endl;
	cin.getline(matrix.ch, 99);
	istringstream sinB(matrix.ch);

	while (sinB)
	{
		if (sinB >> matrix.floatInput)
		{
			//Get the number of columns in first row
			if (matrix.b_rows < 1)
				++matrix.b_columns;
		}
		else
		{
			//Get the number of rows
			sinB.clear();
			char c = sinB.get();
			++matrix.b_rows;
		}
	}

	Matrix* mtxB = new Matrix[matrix.b_rows];
	for (int i = 0; i < matrix.b_rows; ++i)
		mtxB[i].arr = new float[matrix.b_columns];

	//Getting input for matrix B
	sinB = istringstream(matrix.ch);
	int iB = 0;
	int jB = 0;
	while (sinB)
	{
		if (sinB >> matrix.floatInput)
		{
			mtxB[iB][jB] = matrix.floatInput;
			++jB;
		}
		else
		{
			sinB.clear();
			sinB.get();
			++iB;
			jB = 0;
		}
	}
	//Check if the sizes of the two input matrices do not match
	if (matrix.a_columns != matrix.b_rows) {
		cout << "Invalid sizes of the two input matrices!";
		return 0;
	}

	//Check if the element of matrices
	for (iA = 0; iA < matrix.a_rows; ++iA)
	{
		for (jA = 0; jA < matrix.a_columns; ++jA)
			if (mtxA[iA][jA] != -431602080.)
				continue;
			else
				mtxA[iA][jA] = 1;
	}

	for (iB = 0; iB < matrix.b_rows; ++iB)
	{
		for (jB = 0; jB < matrix.b_columns; ++jB)
			if (mtxB[iB][jB] != -431602080.)
				continue;
			else
				mtxB[iB][jB] = 1;
	}
	cout << endl;

	////Outputing matrix
	//for (iA = 0; iA < matrix.a_rows; ++iA)
	//{
	//	for (jA = 0; jA < matrix.a_columns; ++jA)
	//		cout << mtxA[iA][jA] << ' ';
	//	cout << endl;
	//}

	////Outputing matrix
	//for (iB = 0; iB < matrix.b_rows; ++iB)
	//{
	//	for (jB = 0; jB < matrix.b_columns; ++jB)
	//		cout << mtxB[iB][jB] << ' ';
	//	cout << endl;
	//}

	//cout << matrix.a_rows << endl;
	//cout << matrix.a_columns << endl;
	//cout << matrix.b_rows << endl;
	//cout << matrix.b_columns << endl;

	//Calculation for the product of two matrices
	double product[10][10];

	for (int i = 0; i < matrix.a_rows; ++i)
		for (int j = 0; j < matrix.b_columns; ++j) {
			product[i][j] = 0;
		}
	for (int i = 0; i < matrix.a_rows; ++i)
		for (int j = 0; j < matrix.b_columns; ++j)
			for (int k = 0; k < matrix.a_columns; ++k) {
				product[i][j] += mtxA[i][k] * mtxB[k][j];
			}
	
	//Outputing the answer
	try {

		//Measuring the time cost
		auto start = std::chrono::steady_clock::now();
		cout << "Product of the two matrices is:" << endl;
		for (int i = 0; i < matrix.a_rows; ++i)
			for (int j = 0; j < matrix.b_columns; ++j) {
				cout << product[i][j];
				cout << endl;
			}
		auto end = std::chrono::steady_clock::now();
		std::cout
		    << "Product Calculation took "
		    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl;
	}
	catch (int e) {
		cout << "input error, try again" << endl;
	}
	delete[] mtxA;
	delete[] mtxB;

	return 0;
}