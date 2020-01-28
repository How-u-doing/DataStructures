#include "LinkedQueue.h"
#include <iostream>

using namespace std;

int main()
{
	LinkedQueue<int> qi, qi2;

	try {
		qi.push(1);	qi.push(2);	qi.push(3);	qi.push(4);	qi.push(5);
		cout << "qi.front()=" << qi.front() << endl;
		cout << "qi.back()=" << qi.back() << endl;
		cout << "qi.size()=" << qi.size() << endl;

		qi2 = qi;
		qi2.pop();	qi2.pop();
		qi2.push(100);	qi2.push(200);	qi2.push(300);

		cout << qi2;

	}
	catch (const char* e) {
		cout << e << endl;
	}

	return 0;
}
