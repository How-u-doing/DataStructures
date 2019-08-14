/* Program for Josephus problem, implementation by circular doubly linked list */
#include<iostream>
#include<string>
#include "DblList.h"
using namespace std;

struct Student {
	char name[15];
	int num;
	char sex;
	char city[15];
	friend ostream& operator<<(ostream& os, Student& s) {
		os.width(15);
		os.setf(ios::left);
		os << s.name << '\t' << s.num << '\t' << s.sex << '\t' << s.city;
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
	// declaration of function Josephus
	void Josephus(int n, int m, DblList<Student> lst);

	DblList<Student>  ls;
	// input following content via console window or import data from "Josephus_input.txt"
	/*	Mark 1001 M Chongqing
		Ania 1002 W Guangzhou
		Joey 1003 M NewYork
		Ross 1004 W London
		Rachel 1005 W Pairs
		Chandler 1006 M Berlin
		Monica 1007 W Seattle
		Phoebe 1008 W Toronto
		Tony 1009  M NewYork
		Sam 1010 M Seattle
		Ben 1011 M Boston
		Alice 1012 W LosAngeles
		Jacob 1013 M Chicago
		Richard 1014 M Sydney
		Sophia 1015 W Pairs
		Jack 1016 M HongKong
		Janny 1017 W Washington
		Elizabeth 1018 W London
		Ctrl+Z
	**/

	ls.Import("Josephus_input.txt", "text");
	int n = ls.length();
	cout << "There are " << n << " people in this list:\n";
	ls.output();

	int m;
	cout << "Length of circle: n = " << n << "\nPlease enter m(count to choose next): ";
	cin >> m;
	Josephus(n, m, ls);
	return 0;
}


void Josephus(int n, int m, DblList<Student> lst)
{	/* Here we don't use reference of lst(i.e. &lst), simply beacuse we
	 * just wanna pick out the only one left but not intend to change
	 * the list's structure. Therefore what we need is just a copy of list.
	**/
	auto p = lst.getHead(), del = p;

	// make tail node link to first node(the one after head node)
	p->prev->next = p->next;
	p->next->prev = p->prev;

	// move p to first node
	p = p->next;

	// now head node is no longer useful, but we can't delete it
	// since the list destructor will be called & it needs it
	del->next = del;						// render head node self-circled(null list)
	del->prev = del;
		
	int i = 1;								// i-th round
	cout << "\n\n---------------Records of people to be out----------------------\n";
	// loop until only one left
	while (n-- > 1) {
		// move (m-1) nodes backward to locate the one to be out
		for (int j = 1; j <= m-1; ++j)
			p = p->next;
		// now *p is to be out
		del = p;

		// supposing n<100
		printf("#%2d", i++);
		cout << "|	 " << del->data << "\n";
		cout << "----------------------------------------------------------------\n";

		// build new links
		p->prev->next = p->next;
		p->next->prev = p->prev;

		// move p to the node after the one that is just out
		p = p->next;
		
		// delete the node to be out
		delete del;
	}

	cout << "\n\nLast person left standing (Josephus Position) is:\n\n";
	cout << p->data << endl;
}