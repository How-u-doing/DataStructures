#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

struct Employee {
	int id;
	char others[396];
};

const int N = (int)1e6;

inline int compare(const void* a, const void* b)
{
	return (((Employee*)a)->id - ((Employee*)b)->id);
}

int main(int argc, const char* argv[])
{
	static Employee arr[N];
	static int data[N];

	const char* filename = "1M.dat";
	if (argc == 2) filename = argv[1];

	// read in data	
	ifstream ifs{ filename, ios::in | ios::binary };
	if (!ifs) { cerr << "Error opening file \"" << filename << "\"" << endl; return 1; }
	ifs.read((char*)data, sizeof(data));

	for (int i = 0; i < N; ++i) {
		arr[i].id = data[i];
		// strcpy(arr[i].others, "not set"); // need to #include <cstring>
	}

	auto begin = chrono::high_resolution_clock::now();
	qsort(arr, N, sizeof(Employee), compare);
	auto stop = chrono::high_resolution_clock::now();

	auto time_spent = chrono::duration_cast<chrono::microseconds>(stop - begin).count();

	cout << sizeof(Employee) << "\t" << sizeof(arr) << '\n';
	cout << "Time taken by C qsort(): " << time_spent / 1e6 << "s\n";

	return 0;
}

