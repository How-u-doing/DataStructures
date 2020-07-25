#include "myStringSort.h"
#include <iostream>

using namespace std;

int main()
{
	using namespace myStringSort;

	string a[13] = { "4PGC938",
					"2IYE230",
					"3CIO720",
					"1ICK750",
					"1OHV845",
					"4JZY524",
					"1ICK750",
					"3CIO720",
					"1OHV845",
					"1OHV845",
					"2RLA629",
					"2RLA629",
					"3ATW723" };

	LSD(a, 13, 7);

	for (const auto& x : a)
		cout << x << endl;

	return 0;
}

