#include "Calculator.h"
#include <string>

using namespace std;

int main()
{
	cout << "+===========================+\n";
	cout << "|      Simple Calculator    |\n";
	cout << "+===========================+\n\n\n";

	cout << "Please input your expression: for example, 2 + 5^3 / ( 1-2.2*3 )\n-->";

	Calculator calc;
	string infix{}, postfix;

	char doAgain = 'y';
	bool isFirstTime = true;
	while (doAgain == 'y' || doAgain == 'Y') {
		if (!isFirstTime) {
			std::cout << "Please input your expression: -->";
		}

		try {
			calc.readInfixExpr(infix);
			postfix = calc.infix_to_postfix(infix);
			cout << "Infix: " << infix << endl;
			cout << "Postfix: " << postfix << endl;
			calc.execPostfixExpr(postfix);
			calc.output();
			calc.clear();
		}
		catch (const char* e) {
			cout << e << endl;
		}

		isFirstTime = false;
		cout << "\nWould you like to do it again (y or n)?\n";
		cin >> doAgain;		
	}

	return 0;
}
