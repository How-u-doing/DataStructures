#include "LinkedStack.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	LinkedStack<int> si, si2;

	si.push(23);
	si.push(12);
	si.push(67);
	cout << si;

	si2 = si;
	si2.pop();
	si2.push(100);
	cout << si2;

	return 0;
}