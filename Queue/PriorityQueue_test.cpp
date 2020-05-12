#include "PriorityQueue.h"
#include <iostream>

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

		cout << "a.push(10);\n";
		cout << "a.push(20);\n";
		cout << "a.push(15);\n";
		cout << "a.push(90);\n";
		cout << "a.push(100);\n";
		cout << "a.push(5);\n";
		cout << "a.push(60);\n";
		cout << "b = a;\n";

		cout << "\npriority queue a:\n";
		while (!a.isEmpty()) {
			cout << a.top() << ' ';
			a.pop();
		}
		cout << endl;


		b.pop();
		b.pop();
		b.push(1024);
		b.push(17);

		cout << "\n\nb.pop();\n";
		cout << "b.pop();\n";
		cout << "b.push(1024);\n";
		cout << "b.push(17);\n";

		cout << "\npriority queue b:\n";
		while (!b.isEmpty()) {
			cout << b.top() << ' ';
			b.pop();
		}
	}
	catch (const char* e) {
		cout << e << endl;
	}

	cout << "\n\n\nPress any key to leave...\n";
	char wait;
	cin >> noskipws >> wait;

	return 0;
}