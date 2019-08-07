#include<iostream>
#include<string>
#include "List.h"
using namespace std;

int main()
{
	List<string>  ls{ "*** Friends Characters ***" }, ls2, ls3, ls4, ls5;
	// input following content via console window
	// Chandler Ross Joey Monica Rachel Phoebe
	// Ctrl+Z

	// input() test
	ls.input();
	cout << "LS: " << endl << ls.getHead()->data << endl;
	ls.output();

	// getVal() & operator[] test 
	cout << ls.getVal(3) << " once had a huge crush on " << ls[5] << endl;

	// copy constructor test
	List<string>  ls1{ ls };				// i.e.  List<string>  ls2(ls);

	// remove(), insert() & append() test
	ls1.remove(6);
	ls1.remove(1);
	ls1.insert(0,"Iron Man");
	ls1.append("Captain America");
	cout << "\nLS1: " << endl;
	ls1.output();
	
	// operator= test 
	ls2 = ls;
	ls3 = ls1;

	// Union(), Intersection() test
	ls2.Union(ls1);
	cout << "\nLS2, union of LS & LS1: " << endl;
	ls2.output();
	ls3.Intersection(ls);
	cout << "\nLS3, intersection of LS & LS1: " << endl;
	ls3.output();

	// Export(), Import() test
	ls.Export("Friends.dat");
	ls4.Import("Friends.dat");
	cout << "\nLS4, imported from LS: " << endl; 
	ls4.output();

	cout << "\nEnd of test " << endl;
	return 0;
}
