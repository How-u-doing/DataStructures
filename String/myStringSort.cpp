#include "myStringSort.h"

void myStringSort::LSD(std::string a[], int n, int w)
{
	auto aux = new std::string[n];
	// sort d-th digit by counting sort
	for (int d = w - 1; d >= 0; --d) {
		auto count = new int[Radix + 1]{ 0 };

		// count frequencies
		for (int i = 0; i < n; ++i)
			++count[a[i][d] + 1];

		// transform counts into indices
		for (int r = 0; r < Radix; ++r)
			count[r + 1] += count[r];

		// distribute
		for (int i = 0; i < n; ++i)
			aux[count[a[i][d]]++] = a[i];

		// copy back
		for (int i = 0; i < n; ++i)
			a[i] = aux[i];

		delete[] count;
	}
	delete[] aux;
}
