#include<string>
#include"SeqList.h"
using namespace std;

int main()
{
	SeqList<string> LA(50), LB, LC, LD;
	// input Friends characters:
	// Chandler
	// Ross
	// Joey
	// Monica
	// Rachel
	// Phoebe
	// ^Z
	LA.input();
	cout << "\nLA:\n";
	LA.output();
	LB = LA;
	LB.remove(1);
	LB.remove(2);
	LB.remove(3);
	cout << "\nLB:\n";
	LB.output();
	LB.insert(0, "Mark");
	LB.insert(3, "Gorgeous");
	LB.insert(2, "Ania");
	cout << "\nLB_insert_3:\n";
	LB.output();
	LC = LB;
	LC.Union(LA);
	cout << "\nUnion of LA & LB:\n";
	LC.output();
	LD = LB;
	LD.Intersection(LA);
	cout << "\nIntersection of LA & LB:\n";
	LD.output();
	SeqList<string> LE(LC);
	cout << "\nLE:" << endl;
	LinearList<string>* p = &LE;
	p->output();
	string* sptr = LE.getPtr2data();
	cout << endl << *(sptr + 2) << " is one of my favorite people, & the most beatiful girl I ever know real life!" << endl;
	cout << "I'm somehow insanely into YOU-" << p->getData(3) << " for no reason!!" << endl;

	return 0;
}