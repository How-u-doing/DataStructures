#pragma once
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include<iostream>
#include<string>

struct Term {
	Term(float coefficient = 0.0, int exponent = -1, Term* next = nullptr) : coef(coefficient), exp(exponent), next(next) {}
	friend std::ostream& operator<<(std::ostream& os, const Term& term);
	
	float coef;								// coefficient
	int exp;								// exponent	
	Term* next;								// link to next term
};

class Polynomial {

public:
	Polynomial() { head = new Term; }
	Polynomial(const Polynomial& polyn);
	Polynomial(const char str[]);
	Polynomial(const std::string& str);						// constructor by a string, e.g. Polynomial polyn("x^2-x+0.2")
	~Polynomial() { clear(); }

	int order()const;										// max order of the polynomial
	Term* getHead()const { return head; }					// gain the pointer to the head node	
	void insert_after(Term* ptr, float coefficient, int exponent);
	void assign(const Polynomial& polynomial);
	void assign(const char str[]);
	void assign(const std::string& str);					// assign via string
	void clear();											// erase all nodes
	int length()const;
	Term* locate(int i);
	Term* shift(Term* position, int distance);
	void swap(int i, int j);
	void sort();
	Polynomial& opposite();									// opposite of a polynomial (multiply it by -1)
	Polynomial& operator=(const Polynomial& polyn);
	Polynomial& operator=(const char str[]);
	Polynomial& operator=(const std::string& str);			// overload = to assign a polynomial via a string
	
	friend std::ostream& operator<<(std::ostream& os, const Polynomial& polynomial);
	friend std::istream& operator>>(std::istream& is, Polynomial& polynomial);
	friend Polynomial operator+(const Polynomial& fx, const Polynomial& gx);
	friend Polynomial operator-(const Polynomial& fx, const Polynomial& gx);
	friend Polynomial operator*(const Polynomial& fx, const Polynomial& gx);

private:
	int str2int(const std::string str);
	float str2float(const std::string str);					// convert a string to a float, e.g. "12.94", "-3.14"
	Term str2term(const std::string str);					// convert a string to a term, e.g. "+2.9x^4", "-x"
	void str2polyn(const std::string& str);					// tool member function to convert a string to a polynomial
	bool has_ws(const std::string& str)const;				// tell if a string has whitespace(s)
	char* erase_ws(const std::string& str);					// tool member function to erase whitespaces of a string
	Term* head;

};

#endif // !POLYNOMIAL_H
