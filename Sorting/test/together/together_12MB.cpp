#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

struct Employee {
	int id;
	char others[8]; // try { 0, 4, 8, 16, 28, 124, 252, 396, 508, 1020 }
};

const int N = (int)1e6;

struct Comp {
	bool operator()(const Employee& lhs, const Employee& rhs) {
		return lhs.id < rhs.id;
	}
} comp;

inline int compare(const void* a, const void* b)
{
	return (((Employee*)a)->id - ((Employee*)b)->id);
}

int main()
{
	static Employee arr[N], dupArr[N];
	static int data[N];

	// read in data
	ifstream ifs{ "../1M.dat", ios::in | ios::binary };
	if (!ifs) { cerr << "Error opening file" << endl; return 1; }
	ifs.read((char*)data, sizeof(data));

	for (int i = 0; i < N; ++i) {
		dupArr[i].id = arr[i].id = data[i];
		// strcpy(arr[i].others, "not set"); // need to #include <cstring>
		// strcpy(dupArr[i].others, "not set");
	}

	double time_spent[2];

	auto begin = chrono::high_resolution_clock::now();
	qsort(arr, N, sizeof(Employee), compare);
	auto stop = chrono::high_resolution_clock::now();
	time_spent[0] = chrono::duration_cast<chrono::microseconds>(stop - begin).count() / 1e6;

	begin = chrono::high_resolution_clock::now();
	sort(dupArr, dupArr + N, comp);
	stop = chrono::high_resolution_clock::now();
	time_spent[1] = chrono::duration_cast<chrono::microseconds>(stop - begin).count() / 1e6;

	cout << sizeof(Employee) << "\t" << sizeof(arr) << '\n';
	cout << "Time taken by C   qsort(): " << time_spent[0] << '\n';
	cout << "Time taken by C++ sort() : " << time_spent[1] << '\n';

	// append results to csv file
	// initial contents ",qsort,sort\n" or add header afterward
	ofstream ofs{ "results.csv", ios::out | ios::app };
	ofs << sizeof(arr) / 1000000 << "MB,"
		<< time_spent[0] << ',' << time_spent[1] << '\n';

	return 0;
}

