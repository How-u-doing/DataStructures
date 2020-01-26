#include "Calculator.h"
#include <string>

using namespace std;

int main() 
{
	Calculator calc;
	string infix{ "10+(2+8.7/3)*2=" }, postfix;

	try {
		calc.execPostfixExpr("12 34 56+*");
		calc.output();
	
		calc.execPostfixExpr("1024 2 10^+");
		calc.output();
	
		calc.clear();
			
		postfix = calc.infix_to_postfix(infix);
		cout << "Infix: " << infix << endl;
		cout << "Postfix: " << postfix << endl;
		calc.execPostfixExpr(postfix);
		calc.output();
	}
	catch (const char* e) {
		cout << e << endl;
	}

	return 0;
}

