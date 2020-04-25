#include "mySort.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

	friend bool operator<(const stu& a, const stu& b) {
		return a.get_sum() < b.get_sum();
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
#if defined _WIN32 || defined _WIN64
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#else  // on Linux/Unix
	cin.ignore(1024, '\n');
#endif // defined _WIN32 || defined _WIN64
}

int main()
{
	vector<stu> vs;

	vs.push_back(stu(10170401,82,79,91));
	vs.push_back(stu(10170402, 61.5, 70, 61));
	vs.push_back(stu(10170403, 92, 89, 98));
	vs.push_back(stu(10170404, 70, 94, 90));
	vs.push_back(stu(10170405, 83, 76, 93));
	vs.push_back(stu(10170406, 78, 79, 81.5));
	vs.push_back(stu(10170407, 92, 97, 100));
	vs.push_back(stu(10170408, 65, 90.5, 79));
	vs.push_back(stu(10170409, 90, 69, 71));

	cout << "Original list\n";
	stu::printHeader();
	for (auto& item : vs) {
		cout << item << '\n';
	}

	// test build-in array	
	int a[square(3)] = { 4,9,1,3,5,7,2,8,6 };
	cout << "\nOriginal int array\n";
	for (int i = 0; i < 9; ++i) {
		i < 8 ? cout << a[i] << ", " : cout << a[i] << '\n';
	}


	cout << "\nPlease choose sorting algorithm\n";
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

	char isValid = 'y';
	int mode{};
	while (isValid == 'y')
	{		
		cin >> mode;
		while (cin.fail()) {// possibly a formatting error, e.g. inputed a non-digit first
			cin.clear();
			kill_extra_char();
			cout << "Invalid char detected! Please input your option (0-10) again: ";
			cin >> mode;
		}

		switch (mode)
		{
		case 0: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::InsertionSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::InsertionSort);

			isValid = 'n'; break;

		case 1: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::SelectionSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::SelectionSort);

			isValid = 'n'; break;

		case 2: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::MergeSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::MergeSort);

			isValid = 'n'; break;

		case 3: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::Heapsort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::Heapsort);

			isValid = 'n'; break;

		case 4: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::Quicksort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::Quicksort);

			isValid = 'n'; break;

		case 5: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::Shellsort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::Shellsort);

			isValid = 'n'; break;

		case 6: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::BubbleSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::BubbleSort);

			isValid = 'n'; break;

		case 7: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::CombSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::CombSort);

			isValid = 'n'; break;

		case 8: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::CountingSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::CountingSort);

			isValid = 'n'; break;

		case 9: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::BucketSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::BucketSort);

			isValid = 'n'; break;

		case 10: mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
			return s1.get_sum() > s2.get_sum();
			}, mySortingAlgo::Mode::RadixSort);

			mySortingAlgo::sort(a, a + 9, [](const int a, const int b) {
				return a < b;
				}, mySortingAlgo::Mode::RadixSort);

			isValid = 'n'; break;

		default:
			cout << "Invalid option! Please input your option (0-10) again: ";
			kill_extra_char();
			break;
		}
	}

	string method[]{ "Insertion Sort", "Selection Sort", "Merge Sort", "Heapsort", "Quicksort", "Shellsort", 
					 "Bubble Sort", "Comb Sort", "Counting Sort", "Bucket Sort", "Radix Sort"};

	string print_msg = "The student list and build-in int array sorted by \"" + method[mode] + "\" are as follows\n";
	cout << print_msg;

	cout << "\n**Student** --> After sorting by total score in descending order\n";
	stu::printHeader();
	for (auto& item : vs) {
		cout << item << '\n';
	}

	cout << "\n**Build-in array** --> After sorting in ascending order\n";
	for (int i = 0; i < 9; ++i) {
		i < 8 ? cout << a[i] << ", " : cout << a[i] << '\n';
	}

	cout << "\nPress any key to leave...\n";
	char wait;
	kill_extra_char();
	cin >> wait;	
	return 0;

	// Appendix: using std sorting algorithm (Quicksort)
	/*std::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		return s1.get_sum() > s2.get_sum();
		});*/
	// bad, invalid comparator, return value can be only true(!= 0) or false(== 0)
	// see also <https://en.cppreference.com/w/cpp/algorithm/sort>
	/*std::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		if (s1.get_sum() > s2.get_sum()) return 1;
		else if (s1.get_sum() < s2.get_sum()) return -1;
		else return 0;
		});*/
}

