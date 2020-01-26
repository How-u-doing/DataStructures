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
	while (doAgain == 'y' || doAgain == 'Y') {

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

		cout << "Would you like to do it again (y or n)?\n";
		//while (char kill = getchar() != '\n');
		cin >> doAgain;
	}

	return 0;
}
