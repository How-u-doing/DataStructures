#include "SeqQueue.h"
#include<iostream>

using namespace std;

int main()
{
	SeqQueue<int> qi(5), qi2(4);
	qi.push(1);
	qi.push(2);
	qi.push(3);
	qi.push(4);
	qi.push(5);
	qi.push(6);
	qi.push(7);
	qi.push(8);
	qi.push(9);
	qi.push(10);
	cout << "qi.front()=" << qi.front() << endl;
	cout << "qi.back()=" << qi.back() << endl;
	cout << "qi.size()=" << qi.size() << endl;
	cout << "qi.capacity()=" << qi.capacity() << endl;
		
	qi2 = qi;
	qi2.pop();
	cout << "\nqi2.front()=" << qi2.front() << endl;
	cout << "qi2.back()=" << qi2.back() << endl;
	cout << "qi2.size()=" << qi2.size() << endl;
	cout << "qi2.capacity()=" << qi2.capacity() << endl;

	qi2.resize(25);
	cout << "\ni2.back()=" << qi2.back() << endl;
	cout << "qi2.size()=" << qi2.size() << endl;
	cout << "qi2.capacity()=" << qi2.capacity() << endl;
	return 0;
}
