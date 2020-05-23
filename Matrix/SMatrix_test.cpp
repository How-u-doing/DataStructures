#include "SMatrix.h"
#include <iostream>

using namespace std;

int main()
{
	SMatrix<int> A(10, 10, { {1,7,20},{2,6,90},{5,4,10} });
	cout << "Original A:\n";
	cout << A << "\n";
	
	auto B = A.transpose();
	cout << "After transposing:\n";
	cout << B << "\n";

	return 0;
}
