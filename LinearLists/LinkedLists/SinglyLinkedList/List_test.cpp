#include<iostream>
#include<string>
#include "List.h"
using namespace std;

struct Student {
	/* Note: It's hugely dangerous to use string for data members & try to write 
	 * the class(struct) object in binary form while without designing carefully 
	 * for writing or reading the string members into or from a file.
	 * Reason: (copied from Page 1133 in "C++ Primer Plus 6th Edition")
	 * The problem is that a string object doesn¡¯t actually contain the string 
	 * within itself; instead, it contains a pointer to the memory location where
	 * the string is stored. So if you copy the structure to a file, you don¡¯t copy
	 * the string data, you just copy the address of where the string was stored.
	 * When you run the program again, that address is meaningless(& if you try to
	 * read it, it will cause memory access violation which is dead dangerous).
	**/
	char name[20];		// using char[] instead of string to define members name & addr
						// if you don't wanna redesign a function(might be complicated)
						// to export or import data into or from a file in binary form
	int num;
	char sex;
	char addr[30];
	friend ostream& operator<<(ostream& os, Student& s) {
		os << s.name << " " << s.num << " " << s.sex << " " << s.addr << endl;
		return os;
	}
	friend istream& operator>>(istream& is, Student& s) {
		is >> s.name >> s.num >> s.sex >> s.addr;
		return is;
	}
	bool operator==(const Student& s) {
		return (strcmp(name, s.name) == 0 && sex == s.sex && num == s.num && strcmp(addr, s.addr) == 0) ? true : false;
	}
};

int main()
{

	List<Student>  ls, ls2, ls3, ls4;
	// input following content via console window:
	/*  Mark 1001 M Chongqing
		Ania 1002 W Guangzhou
		Joey 1003 M NewYork
		Nancy 1004 W London
		Rachel 1005 W Pairs
		Chandler 1006 M Berlin
		Ctrl+Z
	**/

	// input(), output() test
	ls.input();
	cout << "LS: " << endl;
	ls.output();

	// getVal() & operator[] test 
	cout << ls.getVal(3).name << " once had a huge crush on " << ls[5].name << endl;

	// copy constructor test
	List<Student>  ls1{ ls };				// i.e.  List<Student>  ls1(ls);

	// remove(), insert() & append() test
	ls1.remove(6);
	ls1.remove(1);
	ls1.insert(0, { "Ross",1000,'M',"NewYork" });
	ls1.append({ "Monica", 1007, 'M', "NewYork" });
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
	ls.input();								// input with existing data test 
	ls.output();
	ls.Export("stu.dat");
	ls4.Import("stu.dat");
	cout << "\nLS4, imported from LS: " << endl;
	ls4.output();

	cout << "\nEnd of test " << endl;
	return 0;
}
