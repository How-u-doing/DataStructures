#include "mySort.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct stu {
	//int Number:3;	   // using only 3 bits for Number
	int Number;
	float Math;
	float English;
	float Computer;
	float total_score = sum();
	float sum()const { return Math + English + Computer; }

	stu(int num = 0, float math = .0f, float english = .0f, float computer = .0f)
		:Number(num), Math(math), English(english), Computer(computer) {}

	friend ostream& operator<< (ostream & os, const stu & s) {
		os << s.Number << "\t" << s.Math << "\t" << s.English << "\t" << s.Computer << "\t" << s.total_score << endl;
		return os;
	}

	static void printHeader() {
		cout << " Number\t      Math   English  Computer  Sum\n";
	}
};

bool sum_comp(const stu& s1, const stu& s2) {
	return s1.total_score > s2.total_score;
}

int main()
{
	vector<stu> vs;

	vs.push_back(stu(10170401,82,79,91));
	vs.push_back(stu(10170402, 61.5, 70, 61));
	vs.push_back(stu(10170403, 92, 89, 98));
	vs.push_back(stu(10170404, 70, 94, 90));
	vs.push_back(stu(10170405, 83, 76, 91));
	vs.push_back(stu(10170406, 78, 79, 81.5));
	vs.push_back(stu(10170407, 92, 97, 100));
	vs.push_back(stu(10170408, 65, 90.5, 79));
	vs.push_back(stu(10170409, 90, 69, 71));

	cout << "Original list\n";
	stu::printHeader();
	for (auto& item : vs) {
		cout << item;
	}

	// using std sorting algorithm
	/*std::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		return s1.total_score > s2.total_score;
		});*/

	// using my sorting algorithm
	mySortingAlgo::sort(vs.begin(), vs.end(), [](const stu& s1, const stu& s2) {
		return s1.total_score > s2.total_score;
		}, mySortingAlgo::Mode::BubbleSort);

	// try default sort (Quicksort)
	//mySortingAlgo::sort(vs.begin(), vs.end(), sum_comp);

	cout << "\n\nAfter sorting by total score\n";
	stu::printHeader();
	for (auto& item : vs) {
		cout << item;
	}

	return 0;
}