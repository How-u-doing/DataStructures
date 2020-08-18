#include "IndexPQ.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace myIndexPQ;

template<typename T> void print_queue(T& q) {
	while (!q.empty()) {
		cout << q.top() << " ";
		q.pop();
	}
	cout << '\n';
}

int main() 
{
	IndexPQ<string> pq(20);
	//IndexPQ<string, less<string>> pq(20, less<string>{});

	vector<string> s{ "YOU","are","one","of","my","favorite","people" };

	for (auto i = 0; i < s.size(); ++i) {
		pq.insert(i, s[i]);
	}

	pq.erase(2);
	pq.erase(3);
	pq.change(s.size() - 1, "girl");

	pq.insert(2, "Darling");

	print_queue(pq);

	return 0;
}