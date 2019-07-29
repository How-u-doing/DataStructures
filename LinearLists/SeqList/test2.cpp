#include<iostream>
#include<string>
#include"SeqList.h"
using namespace std;

class Student
{
public:
	int num;
	string name[15];
	float score[3];

};

int main()
{
	SeqList<string> SL(50),SL1(10);
	cout << "SL:" << endl;
	SL.input();
	SL.Export("Friends.dat");
	SL1.Import("Friends.dat");
	cout << "SL1(imported from Friends.dat):" << endl;
	SL1.output();
	return 0;
}