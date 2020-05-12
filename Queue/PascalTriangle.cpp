// to be finished, print pascal's triangle using queue

#include "LinkedQueue.h"
#include <iostream>

using namespace std;

void pascal(int n)
{
	LinkedQueue<int> que;
	int i = 1, j, s = 0, k = 0, t, u;
	que.push(i); que.push(i);
	for (int i = 1; i <= n; ++i) {
		cout << endl;
		que.push(k);
		for (j = 1; j <= i + 2; ++j) {
			t = que.front();
			que.pop();
			u = s + t;
			que.push(u);
			s = t;
			if (j != t + 2)cout << s << ' ';
		}
	}
}

int main()
{
	pascal(8);
	return 0;
}