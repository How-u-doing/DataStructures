#include "SMatrix.h"
#include <iostream>

using namespace std;

int main()
{
	int m = 20, n = 20;
	SMatrix<int> A(m, n, { {17,3,20}, { 9,14,90 }, { 15,12,50 } ,
		{3,8,10}, {11,4, 80}, {7,12,30}, {9,11,60}, {15,4,70} });
	cout << "Original sparse matrix A (" << m << 'x' << n << "):\n"
		 << A << "\n";
	
	auto B = A.transpose();
	cout << "B = A.transpose():\n" 
		 << B << "\n";

	auto C = A.fast_transpose();
	cout << "\nC = A.fast_transpose():\n"
		 << C << "\n";


	cout << "\n**Addition test**\n";
	SMatrix<int> D(m, n, { {19,3,20}, { 6,3,90 }, { 15,12,50 } ,
		{3,8,10}, {11,4, 80}, {5,2,30}, {4,10,60}, {16,4,70} });
	cout << "Another sparse matrix D (" << m << 'x' << n << "):\n"
		 << D << "\n";

	auto E = A + D;
	cout << "\nE = A + D:\n"
		 << E << "\n";

	return 0;
}
