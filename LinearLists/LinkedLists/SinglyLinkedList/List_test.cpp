#include<iostream>
#include<string>
#include<iostream>
#include "List.h"
using namespace std;
int main()
{
	List<string> ls,ls2,ls3,ls4,ls5,ls6;
	ls.append("Ania");
	ls.append("Rachel");
	ls.insert(1, "Joey");
	List<string> ls1 = ls;
	ls1.insert(2, "Honey");
	ls1.append("Marvlous");
	ls1.remove(2);
	cout << "LS: " << endl;
	ls.output();
	cout << "LS1: " << endl;
	ls1.output();
	ls2 = ls;
	ls3 = ls1;
	ls2.Union(ls1);
	cout << "LS2, union of ls & ls1: " << endl;
	ls2.output();
	cout << "LS3, intersection of ls & ls1: " << endl;
	ls3.Intersection(ls);
	ls3.output();
	ls4 = ls;
	cout << "LS4(=LS): " << endl;
	ls4.output();
	ls4.Export("fantastic.dat");
	ls5.Import("fantastic.dat");
	cout << "LS5(imported from fantastic.dat): " << endl;
	ls5.output();
	cout << "ls5-> member 2:" << endl << ls5.getVal(2) << endl;
	ls5.insert(0, "Chandler");
	ls5.append("Monica");
	ls5.output();
	cout << "ls5-> member 1:" << endl << ls5.getVal(1) << endl;
	ls6.input();
	cout << "LS6, input via console window: " << endl;
	ls6.output();
	cout << "ls6-> member 2:" << endl << ls6.getVal(2) << endl;
	ls6.append("Stunning view");
	ls6.input();
	ls6.output();
	return 0;
}
