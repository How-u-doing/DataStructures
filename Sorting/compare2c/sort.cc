// C++ program to demonstrate performance of 
// C qsort() and C++ sort() routines
#include "../mySort.h"
#include "mySort2.h"
#include <algorithm> 
#include <iostream> 
#include <ctime> 
#include <stdlib.h> 
#include <string.h>

#if defined _WIN32 || defined _WIN64
#pragma warning(disable:4996) // for Windows VS using c functions
#endif

using namespace std; 

enum Gender : char  {M, W};

struct Employee {
	int id;
	Gender gender;
	char addr[10];
	double salary;
	double s;
	int t;	
};
	
const int N = (int) 1e7;

struct Comp {
	bool operator()(const Employee& lhs, const Employee& rhs) {
		return lhs.id < rhs.id;
	}
} comp;

struct Comp_p {
	bool operator()(const Employee* lhs, const Employee* rhs) {
		return lhs->id < rhs->id;
	}
} comp_p;

// A comparator function used by qsort 
inline int compare(const void* a, const void* b)
{ 
	return ( ((Employee*)a)->id - ((Employee*)b)->id ); 	
} 

// Driver program to test above functions 
int main()
{
	// using `static' to store them in .bss segment,
	// which is a part of heap?
	// too large to hold in stack
	static Employee arr1[N], arr2[N], arr3[N], arr4[N];
	
	cout << sizeof(Employee) << "\t" << sizeof(arr1) << endl;
	
	// seed for random input 
	srand(time(NULL)); 

	// to measure time taken by qsort and sort 
	clock_t begin, end; 
	double time_spent; 

	// generate random input 
	for (int i = 0; i < N; i++){
		arr1[i].id = rand() % 10000001; // 0~1e7 
		arr1[i].gender = static_cast<Gender>(rand() % 2);
		strcpy(arr1[i].addr, "Berlin");
		arr1[i].salary = static_cast<double>(rand() % (int)1e6 + 1e5);
		arr1[i].s = static_cast<double>(rand() % (int)1e6 + 1e5);
		arr1[i].t = rand() % 1000001; // 0~1e6 	
	}
	memcpy(arr2, arr1, sizeof(arr1));
	memcpy(arr3, arr1, sizeof(arr1));
	memcpy(arr4, arr1, sizeof(arr1));

	begin = clock(); 
	qsort(arr1, N, sizeof(int), compare); 
	end = clock(); 

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 

	cout << "Time taken by C qsort(): "
		<< time_spent << '\n'; 

	begin =  clock(); 
	sort(arr2, arr2 + N, comp); 
	end = clock(); 

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 

	cout << "Time taken by C++ functor sort(): "
		<< time_spent << '\n'; 

	begin = clock(); 
	mySortingAlgo::quicksort(arr3, arr3 + N, comp);
	end = clock(); 

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 

	cout << "Time taken by C++ functor mySort(), passed by object reference: "
		<< time_spent << '\n'; 

	begin = clock();
	mySortingAlgo2::quicksort(arr4, arr4 + N, comp_p);
	end = clock();

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	cout << "Time taken by C++ functor mySort2(), passed by object pointer: "
		<< time_spent << '\n';

	return 0; 
} 



