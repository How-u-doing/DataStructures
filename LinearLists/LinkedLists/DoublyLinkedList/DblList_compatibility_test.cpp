#include<iostream>
#include<string>
#include "List.h"
#include "CircList.h"
#include "DblList.h"
using namespace std;

struct Student {
	/* Note: It's hugely dangerous to use string for data members & try to write
	 * the class(struct) object in binary form while without designing carefully
	 * for writing or reading the string members into or from a file.
	 * Reason: (copied from Page 1133 in "C++ Primer Plus 6th Edition")
	 * The problem is that a string object doesn't actually contain the string
	 * within itself; instead, it contains a pointer to the memory location where
	 * the string is stored. So if you copy the structure to a file, you don't copy
	 * the string data, you just copy the address of where the string was stored.
	 * When you run the program again, that address is meaningless(& if you try to
	 * read it, it will cause memory access violation which is dead dangerous).
	**/
	char name[15];		// using char[] instead of string to define members name & city
						// if you don't wanna redesign a function(might be complicated)
						// to export or import data into or from a file in binary form
	int num;			// consist of 4 digits
	char sex;
	char city[15];
	friend ostream& operator<<(ostream& os, Student& s) {
		os.width(15);
		os.setf(ios::left);
		os << s.name << '\t' << s.num << '\t' << s.sex << '\t';
		os << s.city;
		return os;
	}
	friend ofstream& operator<<(ofstream& ofs, Student& s) {
		ofs << s.name << " " << s.num << " " << s.sex << " " << s.city;
		return ofs;
	}
	friend istream& operator>>(istream& is, Student& s) {
		is >> s.name >> s.num >> s.sex >> s.city;
		return is;
	}
	friend ifstream& operator>>(ifstream& ifs, Student& s) {
		ifs >> s.name >> s.num >> s.sex >> s.city;
		return ifs;
	}
	bool operator==(const Student& s) {
		return (strcmp(name, s.name) == 0 && sex == s.sex && num == s.num && strcmp(city, s.city) == 0) ? true : false;
	}
};

/* test for compatibility among different list types*/
int main()
{
	CircList<Student> cll;
	cll.Import("circular_list_input.txt", "text");
	List<Student> sll;
	sll.Import("list_input.txt", "text");

	DblList<Student>  dll{ cll }, dll2{ sll }, dll3, dll4;
	cout << "DLL(copied from circular linked list CLL):\n";
	dll.output();
	cout << "\n\nDLL2(copied from singly linked list SLL):\n";
	dll2.output();	
	
	dll3 = cll;
	cout << "\n\nDLL3(assigned by circular linked list CLL):\n";
	dll3.output();

	dll4 = sll;
	cout << "\n\nDLL4(assigned by singly linked list SLL):\n";
	dll4.output();

	// Union(), Intersection() compatibility test
	dll3.Union(sll);
	cout << "\nDLL3, union of DLL3 & SSL: " << endl;
	dll3.output();

	dll4.Intersection(cll);
	cout << "\nDLL4, intersection of DLL4 & CLL: " << endl;
	dll4.output();
	
	cout << "\nEnd of the test." << endl;
	return 0;
}