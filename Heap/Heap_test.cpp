#include "Heap.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace myHeap;

struct stu {
	stu(int num = 0) :_num(num) {}
	int _num;
	friend bool operator<(const stu& a, const stu&  b){ return a._num < b._num; }
	friend ostream& operator<<(ostream& os, const stu& s) {
		os << s._num << ' ';
		return os;
	}
};

template <typename T>
void print_heap(T& heap) {
	while (!heap.empty()) {
		cout << heap.top() << " ";
		heap.pop();
	}
	cout << '\n';
}

struct my_greater{
	bool operator()(const stu& a, const stu& b) { return a._num > b._num; };
};

void vector_test()
{
	vector<int> v{ 3, 1, 4, 1, 5, 9 };
	auto v2{ v };
	
	cout << "original v: ";
	for (auto i : v) cout << i << ' ';
	cout << '\n';

	heapsort(v2.begin(), v2.end());
	cout << "after heapsort v2: ";
	for (auto i : v2) cout << i << ' ';
	cout << '\n';

	make_heap(v.begin(), v.end()/*, [](auto a, auto b) {//c++14 required
		return a < b;
		}*/);

	cout << "after make_heap: ";
	for (auto i : v) cout << i << ' ';
	cout << '\n';

	v.push_back(6);
	cout << "before push_heap: ";
	for (auto i : v) cout << i << ' ';
	cout << '\n';

	push_heap(v.begin(), v.end());
	cout << "after push_heap: ";
	for (auto i : v) cout << i << ' ';
	cout << '\n';

	sort_heap(v.begin(), v.end());
	cout << "after sort_heap: ";
	for (auto i : v) cout << i << ' ';
	cout << '\n';
}

void array_test()
{
	stu list[10]{ 1,8,5,6,3,4,0,9,7,2 };

	// using (friend) operator< in class stu
	Heap<stu>heap(list, 10);
	print_heap(heap);

	Heap<stu, my_greater>heap2(list, 10);
	heap2.pop();
	heap2.pop();
	heap2.pop();
	heap2.push(6);
	print_heap(heap2);

	auto lambda_greater = [](const stu& a, const stu& b) { return a._num > b._num; };
	Heap<stu, decltype(lambda_greater)> heap3(lambda_greater);
	for (const auto& x : list) {
		heap3.push(x);
	}
	print_heap(heap3);
}

int main()
{
	cout << "Heap in user-defined array test\n";
	array_test();
	cout << "\n\nHeap in vector test\n";
	vector_test();

	return 0;
}