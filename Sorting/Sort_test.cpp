#include <limits> // for std::numeric_limits
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// define quciksort partition scheme: Hoare's partition by default
//#define QUICK3WAY_PARTITION
//#define FAST3WAY_PARTITION
//#define LOMUTO_PARTITION

#include "mySort.h"

using namespace std;

class stu {
private:
	//int Number:3;	   // using only 3 bits for Number
	int Number;
	float Math;
	float English;
	float Computer;
	float total_score;

public:
	stu(int num = 0, float math = .0f, float english = .0f, float computer = .0f)
		:Number(num), Math(math), English(english), Computer(computer), total_score(calc_sum()) {}

	stu(const stu& s) :Number(s.Number), Math(s.Math), English(s.English),
		Computer(s.Computer), total_score(s.total_score) {}

	stu& operator=(const stu& s) {
		Number = s.Number;
		Math = s.Math;
		English = s.English;
		Computer = s.Computer;
		total_score = s.total_score;
		return *this;
	}

	static void printHeader() {
		cout << " Number\t      Math   English  Computer  Sum\n";
	}

	friend ostream& operator<< (ostream & os, const stu & s) {
		os << s.Number << "\t" << s.Math << "\t" << s.English << "\t" << s.Computer << "\t" << s.total_score;
		return os;
	}

	float calc_sum()const { return Math + English + Computer; }
	float get_sum()const { return total_score; }
};

bool sum_comp(const stu& a, const stu& b) {
	return a.get_sum() > b.get_sum();
}

constexpr int square(int n) {
	return n * n;
}

void kill_extra_char()
{
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void sort_by_mode(vector<stu>& vs, int a[], int b[], int c[], int d[], int n, int mode) {
	assert(0 <= mode && mode <= 10);
	auto Mode = static_cast<mySortingAlgo::Mode>(mode);

	// sort in descending order
	mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		return s1.get_sum() > s2.get_sum();
		}, Mode);

	// sort in ascending order
	mySortingAlgo::sort(a, a + n, [](const int x_1, const int x_2) {
		return x_1 < x_2;
		}, Mode);
	mySortingAlgo::sort(b, b + n, [](const int x_1, const int x_2) {
		return x_1 < x_2;
		}, Mode);
	mySortingAlgo::sort(c, c + n, [](const int x_1, const int x_2) {
		return x_1 < x_2;
		}, Mode);
	mySortingAlgo::sort(d, d + n, [](const int x_1, const int x_2) {
		return x_1 < x_2;
		}, Mode);
}

void printArr(int a[], int b[], int c[], int d[], int n) {
	cout << "no repeat --> a[n]:   ";
	for (int i = 0; i < n; ++i) {
		i < n - 1 ? cout << a[i] << ", " : cout << a[i] << '\n';
	}
	cout << "all equal --> b[n]:   ";
	for (int i = 0; i < n; ++i) {
		i < n - 1 ? cout << b[i] << ", " : cout << b[i] << '\n';
	}
	cout << "some repeat --> c[n]: ";
	for (int i = 0; i < n; ++i) {
		i < n - 1 ? cout << c[i] << ", " : cout << c[i] << '\n';
	}
	cout << "only 012 --> d[n]:    ";
	for (int i = 0; i < n; ++i) {
		i < n - 1 ? cout << d[i] << ", " : cout << d[i] << '\n';
	}
}

int main()
{
	vector<stu> vs;

	vs.emplace_back(10170401, 82, 79, 91);
	vs.emplace_back(10170402, 61, 70, 61);
	vs.emplace_back(10170403, 92, 89, 98);
	vs.emplace_back(10170404, 70, 94.5, 90);
	vs.emplace_back(10170405, 83, 76, 93);
	vs.emplace_back(10170406, 78, 79, 81.5);
	vs.emplace_back(10170407, 92, 97, 100);
	vs.emplace_back(10170408, 82, 91, 79);
	vs.emplace_back(10170409, 90, 69, 71);

	cout << "Original list\n";
	stu::printHeader();
	for (auto& item : vs) {
		cout << item << '\n';
	}

	// test build-in array
	constexpr int n = square(4);
	//int a[n] = { 4, 9, 1, 3, 3, 4, 7, 2, 4 };
	int a[n] = { 3,15,14,9,1,16,4,12,6,2,11,7,8,5,13,10 }; // no repeat
	int b[n] = {}; // test for all equal elements
	int c[n] = { 4,9,1,3,3,4,37,2,4,56,14,7,2,4,36,37 }; // random, some repeating numbers
	int d[n] = { 0, 1, 1, 0, 1, 2, 1, 2, 0, 1, 0, 1, 2, 0, 0, 1 }; // only have 0, 1, 2

	cout << "\nOriginal int arrays\n";
	printArr(a, b, c, d, n);


	cout << "\nPlease choose a sorting algorithm\n";
	cout << "** Insertion Sort------------------------- 0 \n";
	cout << "** Selection Sort ------------------------ 1 \n";
	cout << "** Merge Sort ---------------------------- 2 \n";
	cout << "** Heapsort ------------------------------ 3 \n";
	cout << "** Quicksort ----------------------------- 4 \n";
	cout << "** Shellsort ----------------------------- 5 \n";
	cout << "** Bubble Sort --------------------------- 6 \n";
	cout << "** Cumb Sort ----------------------------- 7 \n";
	cout << "** Counting Sort ------------------------- 8 \n";
	cout << "** Bucket Sort --------------------------- 9 \n";
	cout << "** Radix Sort --------------------------- 10 \n";
	cout << "Enter your option (0-10) here: ";

	int mode{};
	while (true)
	{
		cin >> mode;
		while (cin.fail()) {// possibly a formatting error, e.g. inputed a non-digit first, say w#@9, a7, etc.
			cin.clear();
			kill_extra_char();
			cout << "Invalid char detected! Please input your option (0-10) again: ";
			cin >> mode;
		}

		if (0 <= mode && mode <= 10) {
			sort_by_mode(vs, a, b, c, d, n, mode);
			break;
		}
		else {
			cout << "Invalid option! Please input your option (0-10) again: ";
			kill_extra_char();
		}
	}

	string method[]{ "Insertion Sort", "Selection Sort", "Merge Sort", "Heapsort", "Quicksort", "Shellsort", 
					 "Bubble Sort", "Comb Sort", "Counting Sort", "Bucket Sort", "Radix Sort"};

	string print_msg = "The student list and build-in int arrays sorted by \"" + method[mode] + "\" are as follows\n";
	cout << print_msg;

	cout << "\n**Student** --> After sorting by total score in descending order\n";
	stu::printHeader();
	for (auto& item : vs) {
		cout << item << '\n';
	}

	cout << "\n**Build-in arrays** --> After sorting in ascending order\n";
	printArr(a, b, c, d, n);

#if defined _WIN32 ||  _WIN64
	cout << "\nPress any key to leave...\n";
	char wait;
	kill_extra_char();
	cin >> noskipws >> wait;
#endif // defined _WIN32 || _WIN64

	return 0;

	// Appendix: using std sorting algorithm (Quicksort)
	/*std::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		return s1.get_sum() > s2.get_sum();
		});*/

	// Not allowed. The types [Type1] and [Type2] must be such that an object of type 
	// RandomIt can be dereferenced and then implicitly converted to both of them.
	// see also <https://en.cppreference.com/w/cpp/algorithm/sort>
	/*std::sort(vs.begin(), vs.end(), [](const stu* s1, const stu* s2) {
		return s1->get_sum() > s2->get_sum();
		});*/

	// bad, invalid comparator, return value can be only true(!= 0) or false(== 0)
	/*std::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		if (s1.get_sum() > s2.get_sum()) return 1;
		else if (s1.get_sum() < s2.get_sum()) return -1;
		else return 0;
		});*/
}

