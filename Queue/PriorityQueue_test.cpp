#include "PriorityQueue.h"
#include<iostream>

using namespace std;

int main()
{
	PriorityQueue<int> a, b;

	try {
		a.push(10);
		a.push(20);
		a.push(15);
		a.push(90);
		a.push(100);
		a.push(5);
		a.push(60);

		b = a;

		cout << "priority queue a:\n";
		while (!a.isEmpty()) {
			cout << a.top() << ' ';
			a.pop();
		}
		cout << endl;


		b.pop();
		b.pop();
		b.push(1024);

		cout << "priority queue b:\n";
		while (!b.isEmpty()) {
			cout << b.top() << ' ';
			b.pop();
		}
	}
	catch (const char* e) {
		cout << e << endl;
	}

	return 0;
}