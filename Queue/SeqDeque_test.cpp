#include "SeqDeque.h"
#include <iostream>
//#include <queue>

using namespace std;

int main()
{
	SeqDeque<int> qi(7), qi2(10);
	try {
		qi.push_back(1); qi.push_back(2); qi.push_front(3);
		cout << "qi.front()=" << qi.front() << endl;
		cout << "qi.back()=" << qi.back() << endl;
		cout << "qi.size()=" << qi.size() << endl;
		cout << "qi.capacity()=" << qi.capacity() << endl;

		qi2 = qi;
		qi2.pop_back(); 
		qi2.push_front(5); qi2.push_front(6); qi2.push_front(7);
		cout << "\nAll elements in qi2:\n" << qi2;


		qi2.resize(25);
		cout << "\nAfter resizing:\n";
		cout << "qi2.front()=" << qi2.front() << endl;
		cout << "qi2.back()=" << qi2.back() << endl;
		cout << "qi2.size()=" << qi2.size() << endl;
		cout << "qi2.capacity()=" << qi2.capacity() << endl;
	}
	catch (const char* e) {
		cout << e << endl;
	}

	return 0;
}