#include<iostream>
#include<string>
#include"SeqList.h"
using namespace std;


int main()
{
	SeqList<string> SL(50),SL1(20);
	cout << "SL:" << endl;
	//SL.input();
	//SL.Export("Friends.dat");
	SL1.Import("Friends.dat");
	string s = SL1.getVal(4);
	SL1.setVal(3, "Joey Tribbiani");
	cout << "SL1(imported from Friends.dat):" << endl << s << endl << SL1.length() << endl << SL1.size() << endl;
	SL1.output();
	cout << "End of SL1.\n";
	SL.output();
	cout << "That's it!\n" << endl;
	return 0;
}