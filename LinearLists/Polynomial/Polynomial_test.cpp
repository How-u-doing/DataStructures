#include<iostream>
#include"Polynomial.h"
using namespace std;
int main() 
{
	
	Polynomial polyn1{ "x-1" },
		polyn2 =  "9-x^ 43 + x^12 -x +  2 x^312-87.2x^13+90.92x^16-x^22+6x^724-99x^100",
		polyn3{ polyn2 }, polyn4, polyn5;
	
	cout << "polyn3, without sorting:\n";
	cout << polyn3 << "\n\n";
	polyn3.sort();
	cout <<"Aftrer sorting, polyn3:\n";
	cout << polyn3 << "\n\n";

	cout << "polyn1:\n";
	cout << polyn1 << "\n\n";

	// here we enter x^2+x+1
	cout << "Please enter polyn2: ";
	cin >> polyn2;
	cout << "\npolyn2:\n";
	cout << polyn2 << "\n\n";


	cout << "----------------------addition test----------------------------\n\n";	
	polyn3 = polyn1 + polyn2;
	cout << "polyn3 = polyn1 + polyn2:\n";
	cout << polyn3 << "\n\n";
	cout << "-------------------end of addition test------------------------\n\n";


	cout << "--------------------subtraction test---------------------------\n\n";
	polyn4 = polyn1 - polyn2;
	cout << "polyn4 = polyn1 - polyn2:\n";
	cout << polyn4 << "\n\n";
	cout << "--------------------subtraction test---------------------------\n\n";


	cout << "----------------------product test-----------------------------\n\n";
	polyn5 = polyn1 * polyn2;
	cout << "polyn5 = polyn1 * polyn2:\n";
	cout << polyn5 << "\n\n";
	cout << "-------------------end of product test-------------------------\n\n";
	
	return 0;

}
