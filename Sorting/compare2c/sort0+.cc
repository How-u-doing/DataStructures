// C++ program to demonstrate performance of 
// C qsort() and C++ sort() routines
#include <algorithm> 
#include <iostream> 
#include <ctime> 
#include <stdlib.h> 
#include <string.h>

#if defined _WIN32 || defined _WIN64
#pragma warning(disable:4996) // for Windows VS using c functions
#endif

using namespace std; 

enum Gender /*: char*/  {M, W};

struct Employee {
	int id;
	//Gender gender;
	char addr[396];
	//double salary;
	//double s;
	//int t;	
};

const int N = (int) 1e6;

struct Comp {
	bool operator()(const Employee& lhs, const Employee& rhs) {
		return lhs.id < rhs.id;
	}
} comp;

inline bool comp_fp(const Employee& lhs, const Employee& rhs) {
		return lhs.id < rhs.id;
}

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
	static Employee arr[N], dupArr[N], dupArr2[N];
	
	cout << sizeof(Employee) << "\t" << sizeof(arr) << endl;
	
	// seed for random input 
	srand(time(NULL)); 

	// to measure time taken by qsort and sort 
	clock_t begin, end; 
	double time_spent; 

	// generate random input 
	for (int i = 0; i < N; i++){
		dupArr2[i].id = dupArr[i].id = arr[i].id = rand()%10000001; // 0~1e7
		/**/
		//dupArr2[i].gender = dupArr[i].gender = arr[i].gender = static_cast<Gender>(rand()%2);

		//strcpy(dupArr2[i].addr, "Berlin"); // small string
		strcpy(arr[i].addr, "This is a large string of 390 charactars; This is a large string of 390 charactars"
			"This is a large string of 390 charactars; This is a large string of 390 charactars"
			"This is a large string of 390 charactars; This is a large string of 390 charactars"
			"This is a large string of 390 charactars; This is a large string of 390 charactars"
			"This is a large string of 390 charactars; This is a large str");
		
		strcpy(dupArr[i].addr, arr[i].addr); strcpy(dupArr2[i].addr, arr[i].addr);
		//dupArr2[i].salary = dupArr[i].salary = arr[i].salary = static_cast<double>(rand()% (int)1e6 + 1e5);
		//dupArr2[i].s = dupArr[i].s = arr[i].s = static_cast<double>(rand()% (int)1e6 + 1e5);
		//dupArr2[i].t = dupArr[i].t = arr[i].t = rand()%1000001; // 0~1e6 	    
	}
			
	begin =  clock(); 
	sort(dupArr, dupArr + N, comp); 
	end = clock(); 

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 

	cout << "Time taken by C++ functor sort(): "
		<< time_spent << '\n'; 

    begin = clock(); 
	qsort(arr, N, sizeof(int), compare); 
	end = clock(); 

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 

	cout << "Time taken by C qsort(): "
		<< time_spent << '\n'; 

	begin = clock(); 
	sort(dupArr2, dupArr2 + N, comp_fp);
	end = clock(); 

	time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 

	cout << "Time taken by C++ function pointer sort(): "
		<< time_spent << '\n'; 

	return 0; 
} 


