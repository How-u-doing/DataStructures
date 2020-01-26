#include "Calculator.h"
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>


void Calculator::get2Operands(double& left, double& right) {
	if (stack.isEmpty()) { throw "fun@get2Operands: lack_of_right_operand\n"; }
	stack.pop(right);
	if (stack.isEmpty()) { throw "fun@get2Operands: lack_of_left_operand\n"; }
	stack.pop(left);
}

void Calculator::doOperator(char op) {
	double left, right;
	get2Operands(left, right);
	switch (op) {
	case '+': stack.push(left + right); break;
	case '-': stack.push(left - right); break;
	case '*': stack.push(left * right); break;
	case '/':
		if (right == 0.0) throw "divide_by_0\n";
		else stack.push(left / right);
		break;
	case '^': stack.push(pow(left, right)); break;
	default: throw "fun@doOperator: Invalid_operator\n";
	}
}

void Calculator::readInfixExpr(std::string& infix) {
	getline(std::cin, infix);
	while (infix.length() == 0)		// kill blank line(s)
		getline(std::cin, infix);
}

// execute a postfix expression, e.g. ABCD-*+EF/- (i.e. infix: A+B*(C-D)-E/F)
// for instance, postfix="12.5 34 56+*"
void Calculator::execPostfixExpr(const std::string& postfix) {
	char ch;
	double newOperand;
	std::istringstream iss{ postfix + '=' };	// add a '=' at the end to stop reading
	while (iss >> ch, ch != '=') {
		if (ch >= '0' && ch <= '9' || ch == '.') {
			iss.putback(ch);
			iss >> newOperand;
			stack.push(newOperand);
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') doOperator(ch);
		else throw "fun@execPostfixExpr: Invalid_char\n";
	}
}

// in stack priority
int Calculator::isp(char op){
	switch (op) {
	case '=':			return 0;
	case '(':			return 1;
	case '+':case '-':	return 3;
	case '*':case '/':	return 5;
	case '^':			return 7;
	case ')':			return 8;
	default: throw "fun@isp: Invalid_operator\n";
	}
}

// in coming stack priority
int Calculator::icp(char op) {
	switch (op) {
	case '=':			return 0;
	case ')':			return 1;
	case '+':case '-':	return 2;
	case '*':case '/':	return 4;
	case '^':			return 6;
	case '(':			return 8;
	default: throw "fun@icp: Invalid_operator\n";
	}
}

bool Calculator::isOper(char ch) {
	return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
		ch == '^' || ch == '(' || ch == ')' || ch == '=') ?
		true : false;
}

// e.g. infix="2.2+3^2*5-(2+8.7/3)*2"
std::string Calculator::infix_to_postfix(const std::string& Infix) {
	std::string infix = eatWhitespace(Infix);	// kill all whitesapces
	infix += '=';	// add '=' for stopping sign
	SeqStack<char> s;
	char ch = '=';	// push '=' onto stack bottom
	s.push(ch);
	std::string postfix{};
	int i = 0;
	while (!s.isEmpty() || infix[i] != '=') {
		if (infix[i] >= '0' && infix[i] <= '9' || infix[i] == '.')
			postfix += infix[i++];
		else if (isOper(infix[i])) {
			postfix += ' ';
			if (icp(infix[i]) > isp(s.top()))
				s.push(infix[i++]);
			else if (icp(infix[i]) < isp(s.top())) { 
				s.pop(ch); postfix += ch;
			}
			else {
				s.pop(ch); if (ch == '(')	++i;
			}
		}
		else throw "fun@infix_to_postfix: Invalid_char\n";
	}
	return postfix;
}

// clear the stack
void Calculator::clear() {
	stack.clear();
}

// print the ultimate result
void Calculator::output() {
	std::cout << "The result is: " << stack.top() << "\n";
}

std::string Calculator::eatWhitespace(const std::string& str) {
	std::string newStr{};
	for (auto &x : str)
		if (x != ' ')
			newStr += x;
	return newStr;
}