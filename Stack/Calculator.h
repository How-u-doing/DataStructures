// calculator header
#pragma once
#ifndef CALCULATOR
#define CALCULATOR

#include "LinkedStack.h"

class Calculator {
public:
	Calculator() {}
	~Calculator() {}
	void execPostfixExpr(const std::string& postfix);
	void readInfixExpr(std::string& infix);
	void clear();
	void output();
	std::string infix_to_postfix(const std::string& Infix);

private:
	LinkedStack<double> stack;
	void get2Operands(double& left, double& right);
	void doOperator(char op);
	bool isOper(char ch);
	int isp(char op);
	int icp(char op);
	std::string eatWhitespace(const std::string& str);

};

#endif // !CALCULATOR
