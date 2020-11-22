#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

struct Employee {
	int id;
	char others[1020];
};

const int N = (int)1e6;

struct Comp {
	bool operator()(Employee* lhs, Employee* rhs) {
		return lhs->id < rhs->id;
	}
} comp;

int main(int argc, const char* argv[])
{
	static Employee arr[N];
	static int data[N];
	vector<Employee*> ve(N, nullptr);

	const char* filename = "1M.dat";
	if (argc == 2) filename = argv[1];

	// read in data	
	ifstream ifs{ filename, ios::in | ios::binary };
	if (!ifs) { cerr << "Error opening file \"" << filename << "\"" << endl; return 1; }
	ifs.read((char*)data, sizeof(data));

	for (int i = 0; i < N; ++i) {
		arr[i].id = data[i];
		// strcpy(arr[i].others, "not set"); // need to #include <cstring>
		ve[i] = arr + i;
	}

	auto begin = chrono::high_resolution_clock::now();
	sort(&ve[0], &ve[0] + N, comp);
	auto stop = chrono::high_resolution_clock::now();

	auto time_spent = chrono::duration_cast<chrono::microseconds>(stop - begin).count();

	cout << sizeof(Employee) << "\t" << sizeof(arr) << '\n';
	cout << "Time taken by C++ sort() for pointers: " << time_spent / 1e6 << "s\n";

	return 0;
}

