#include<iostream>
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


int main()
{
	DblList<Student>  ls, ls2, ls3, ls4, ls5;
	// input following content via console window:
	/*  Mark 1001 M Chongqing
		Ania 1002 W Guangzhou
		Joey 1003 M NewYork
		Nancy 1004 W London
		Rachel 1005 W Pairs
		Chandler 1006 M Berlin
		Sam 1010 M Seattle
		Jennifer 1011 W Boston
		Willian 1012 M Edinburgh
		Peter 1013 M NewYork
		Tony 1014 M Singapore
		Lucy 1015 W Toronto
		Ctrl+Z
	**/

	// input(), output() test
	ls.input();
	cout << "LS: " << endl;
	ls.output();

	// getVal() & operator[] test 
	cout << ls.getVal(3).name << " once had a huge crush on " << ls[5].name << endl;
	// No.17 is equal to 17-12-1=4, i.e. #4: Nancy
	cout << "No.17: " << ls.locate(17)->data << endl;

	cout << "\n\nDisplay LS in reverse order:\n";
	int i = 1;
	for (auto p = ls.getHead()->prev; p != ls.getHead(); p = p->prev) {
		printf("#%4d: ", i++);
		cout << p->data << '\n';
	}

	// setHead() test
	ls.setHead(ls.locate(7));					// now circluar list starts from No.7
	cout << "\nNow output LS starting from No.7:\n";
	ls.output();

	// copy constructor test
	// here = is not overloaded, and it is calling copy constructor CircList(CircList<T>& L) 
	DblList<Student>  ls1 = ls;				// i.e.  List<Student>  ls1(ls) or ls1{ls};

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

	// swap() test
	ls.swap(2, 5);
	cout << "\n\nAfter swapping member 2 & member 5 of LS:\n";
	ls.output();
	return 0;

	// Export(), Import() test
	//ls.input();								// input with existing data test 
	//ls.output();
	ls2.Export("stud.txt", "text");
	ls2.Export("stud.dat", "binary");

	// read in correct format
	ls4.Import("stud.txt", "text");
	cout << "\nLS4, imported from LS2(text file): " << endl;
	ls4.output();

	ls5.Import("stud.dat", "binary");
	cout << "\nLS5, imported from LS2(binary file): " << endl;
	ls5.output();

	// try to read in wrong manner
	DblList<Student> error_read1, error_read2;
	/* Somehow, in my computer system, it can read text file & binary file(even binary file
	 * does not match with the data type) if I turn on binary mode, and finish reading at
	 * position EOF(end-of-file) so that it will escape the read check. And messy code as
	 * well as data insecurity will occure resultantly.
	 */
	error_read2.Import("stud.dat", "text");					// program will exit & tell u some error has justed happened
	cout << "\nerror_read2, imported from LS2(read binary file in text form): " << endl;
	error_read2.output();

	error_read1.Import("stud.txt", "binary");				// just read, without telling u some error happened
	cout << "\nerror_read1, imported from LS2(read text file in binary form): " << endl;
	error_read1.output();

	cout << "\nEnd of the test." << endl;
	return 0;
}