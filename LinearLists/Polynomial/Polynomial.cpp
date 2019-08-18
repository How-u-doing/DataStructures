#include"Polynomial.h"

Polynomial::Polynomial(const Polynomial& polyn) {
	/* copy constructor */
	Term* curr = head = new Term;
	Term* ptr = polyn.getHead()->next;
	while (ptr != nullptr) {
		curr->next = new Term(ptr->coef, ptr->exp);
		curr = curr->next;
		ptr = ptr->next;
	}
}

Polynomial::Polynomial(const char str[]) {
	/* constructor by a char string, e.g. Polynomial polyn("x^2-x+0.2") */
	head = new Term;
	std::string polyn = str;
	if (has_ws(polyn))
		polyn = erase_ws(polyn);				// standardize polynomial string
	str2polyn(polyn);
}

Polynomial::Polynomial(const std::string& str) {
	/* constructor by a string, e.g. Polynomial polyn("x^2-x+0.2") */
	head = new Term;
	std::string polyn = str;
	if (has_ws(polyn))
		polyn = erase_ws(polyn);				// standardize polynomial string
	str2polyn(polyn);
}

void Polynomial::assign(const Polynomial& polyn) {
	/* assign via a polynomial */
	clear();
	Term* curr = head;
	Term* ptr = polyn.getHead()->next;
	while (ptr != nullptr) {
		curr->next = new Term(ptr->coef, ptr->exp);
		curr = curr->next;
		ptr = ptr->next;
	}
}

Polynomial& Polynomial::operator=(const Polynomial& polyn) {
	/* overload = to assign via a polynomial */
	assign(polyn);
	return *this;
}

void Polynomial::assign(const char str[]) {
	/* assign via a char string */
	clear();
	std::string polyn = str;
	if (has_ws(polyn))
		polyn = erase_ws(polyn);			// standardize polynomial string
	str2polyn(polyn);
}

Polynomial& Polynomial::operator=(const char str[]) {
	/* overload = to assign a polynomial via a char string */
	assign(str);
	return *this;
}

void Polynomial::assign(const std::string& str) {
	/* assign via string */
	clear();
	std::string polyn = str;
	if (has_ws(polyn))
		polyn = erase_ws(polyn);			// standardize polynomial string
	str2polyn(polyn);
}

Polynomial& Polynomial::operator=(const std::string& str) {
	/* overload = to assign via a string */
	assign(str);
	return *this;
}

void Polynomial::clear() {
	/* erase all nodes */
	Term* curr = head->next, *del;
	while (curr != nullptr) {
		del = curr;
		curr = curr->next;
		delete del;
	}
}

int Polynomial::order()const {
	/* max order of the polynomial */

	// we store the polynomial in descending order, 
	// therefore the max order is in the first node
	return head->next->exp;
}

bool Polynomial::has_ws(const std::string& str)const {
	/* tell if a string has whitespace(s) */
	const char* p = &str[0];
	while (*p) {
		if (*p++ == ' ') 
			return true;
	}
	return false;
}

char* Polynomial::erase_ws(const std::string& str) {
	/* (private) tool member function to erase whitespaces of a string */ 	
	auto n = str.size();
	char* desptr = new char[n], *newstr = desptr;
	const char* srcptr = &str[0];
	while (*srcptr)
		if (*srcptr != ' ')
			*desptr++ = *srcptr++;
		else
			++srcptr;
	*desptr = '\0';
	return newstr;
}

int Polynomial::str2int(const std::string str) {
	/* convert a string to an int number, e.g. "97234", "1234567" */
	auto n = str.size();
	int e = 1;
	int result = 0;
	int char_zero = '0';
	const char* p = &str[n - 1];
	while (n--) {
		result += ((int)(*p) - char_zero)*e;
		e *= 10;
		--p;
	}
	return result;
}

float Polynomial::str2float(const std::string str) {
	/* convert a string to a float, e.g. "+12.94", "3.14", "97", "-66.6" */
	int sign = 1;
	const char* p = &str[0];
	if (*p == '-') { sign = -1; ++p; }
	else
		if (*p == '+') ++p;

	int integer = 0;
	float decimal = 0.0;
	float result = 0.0;
	std::string intstr = "";
	std::string decstr = "";

	// process integer part
	while (*p != '.' && *p != '\0')
		intstr += *p++;
	integer = str2int(intstr);

	// doesn't have decimal part
	if (*p == '\0') {
		result = float(integer);
		return sign * result;
	}

	if (*p == '.')
		++p;

	float e = 0.1f;
	int char_zero = '0';
	// process decimal part
	while (*p) {
		decimal += ((int)(*p) - char_zero)*e;
		e *= float(0.1);
		++p;
	}

	result = integer + decimal;
	return result * sign;
}

Term Polynomial::str2term(const std::string str) {
	/* convert a string to a term, e.g. "+2.9x^14", "+x^2", "x^2", "-x", "3.14" */
	Term term;	
	const char* p = &str[0];
	std::string coefstr = "";
	std::string  expstr = "";

	// extract coefficient
	while (*p != 'x' && *p != '\0')
		coefstr += *p++;
	// term that omits coef (= 1 or -1), e.g. "+x^2", "x^2", "-x"
	if (coefstr == "+" || coefstr == "-" || coefstr == "")
		coefstr += '1';

	term.coef = str2float(coefstr);

	// term that likes "3.14", no exponent part
	if (*p == '\0') {
		term.exp = 0;
		return term;	
	}

	++p;

	// extract exponent
	if (*p == '\0')
		term.exp = 1;
	else
		if (*p == '^') {
			++p;
			while (*p)
				expstr += *p++;
			term.exp = str2int(expstr);
		}
		
	return term;
}

void Polynomial::str2polyn(const std::string& str) {
	/* (private) tool member function to convert a standard polynomial 
	 * string to a polynomial obeject, e.g. "-6.7x^3+2x^2-x+0.53"  
	**/
	const char* p = &str[0];
	Term* curr = head;
	std::string term;
	while (*p) {		
		term = "";

		// extract the sign of each term, the first term doesn't have '+' if its coef is positive
		if (*p == '-') { term = "-"; ++p; }
		else
			if (*p == '+') { term = "+"; ++p; }

		// stop at char '+' or '-'
		while (*p != '+' && *p != '-' && *p)
			term += *p++;
		
		Term ter = str2term(term);
		curr->next = new Term(ter.coef, ter.exp);
		curr = curr->next;
	}
}

int Polynomial::length()const {
	/* get the list's length (the number of terms) */
	Term* curr = head->next;
	int count = 0;
	while (curr != nullptr) {
		curr = curr->next;
		++count;
	}
	return count;
}

Term* Polynomial::locate(int i) {
	/* locate the i-th node & return its pointer, 0<=i<=length()
	 * In particular,	 i=0,	  return head(i.e. the pointer to head node)
	 *				  i>length(), return nullptr
	**/
	if (i < 0) { std::cerr << "Locating error! Reason: Invalid argument i, i must be no less than 0." << std::endl; exit(1); }
	Term* curr = head;
	while (i-- && curr != nullptr)
		curr = curr->next;
	return curr;
}

Term* Polynomial::shift(Term* position, int distance) {
	/* locate the node that is after "position" with "distance" node(s)  */
	Term* curr = position;
	while (distance--)
		curr = curr->next;
	return curr;
}

void Polynomial::swap(int i, int j) {
	/* exchange the value of i-th item with that of j-th item,
	 * implementation by exchanging their links while not their values
	**/
	if (i < 1 || j < 1) {
		std::cerr << "Invalid arguments, i & j must be no less than 1." << std::endl;
		exit(1);
	}
	if (i == j) return;								// no need to swap
	if (i > j) { int tmp = i; i = j; j = tmp; }		// render j the larger one
	Term* prev_i = locate(i - 1),
		*prev_j = shift(prev_i, j - i),				/* i.e. *prev_j = locate(j - 1); */
		*ptr_j = prev_j->next, *next_i = prev_i->next->next;

	if (j - i != 1) {
		// when j=i+1, i.e. prev_j = ptr_i =  prev_i->next, 
		// executing  "prev_j->next = prev_i->next"  means: 
		// ptr_i->next = ptr_i, endless loop!!
		prev_j->next = prev_i->next;
		prev_i->next->next = ptr_j->next;

		prev_i->next = ptr_j;
		ptr_j->next = next_i;	
	}
	else {
		// case: ptr_i->next = ptr_j
		prev_i->next->next = ptr_j->next;
		ptr_j->next = prev_i->next;
		prev_i->next = ptr_j;
	}	
}

void Polynomial::sort() {
	/* sort  those  terms of the polynomial in descending 
	 * order without changing their values but their links
	**/
	Term* curr, *maxptr;
	int n = length();
	int k, j;
	for (int i = 1; i <= n - 1; ++i) {
		maxptr = locate(i);
		curr = maxptr->next;
		k = i;
		j = i + 1;
		while (curr != nullptr) {
			if (curr->exp > maxptr->exp) {
				k = j;
				maxptr = curr;
			}				
			curr = curr->next;
			++j;
		}
		if (k != i)
			swap(i, k);
	}	
}

void Polynomial::insert_after(Term* ptr, float coefficient, int exponent) {
	/* insert a term after ptr */
	Term* curr = new Term(coefficient, exponent);
	if (curr == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	curr->next = ptr->next;
	ptr->next = curr;
}

std::istream& operator>>(std::istream& is, Polynomial& polyn) {
	/* input & read a polynomial from console window */	
	if (polyn.head->next != nullptr) polyn.clear();
	std::string str;
	std::getline(is, str);
	if (polyn.has_ws(str))
		str = polyn.erase_ws(str);				// standardize polynomial string
	polyn.str2polyn(str);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Term& term){
	/* print a term */

	// do not output when coefficient is 0
	if (term.coef == 0.0) return os;

	// when exponent is nonzero, we do not output the coefficient 
	// if it is 1, and only ouput '-' when it is -1
	switch (term.exp) {
	case 0: 
		os << term.coef; 
		break;

	case 1:
		if (term.coef == -1.0)
			os << "-x";
		else
			if (term.coef == 1.0)
				os << "x";
			else
				os << term.coef << "x";
		break;

	default:// exp > 1
		if (term.coef == -1.0)
			os << "-x^" << term.exp;
		else
			if (term.coef == 1.0)
				os << "x^" << term.exp;
			else
				os << term.coef << "x^" << term.exp;
		break;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& polyn) {
	/* print a polynomial */
	Term* curr = polyn.getHead()->next;
	// print 0 when it is null list 
	if (curr == nullptr) {
		os << "0";
		return os;
	}
	bool isFirstTerm = true;
	while (curr != nullptr) {
		// we do not output the sign of first term if its coefficient is positive
		if (isFirstTerm == false && curr->coef > 0.0) os << '+';
		isFirstTerm = false;
		os << *curr;
		curr = curr->next;
	}
	return os;
}

Polynomial operator+(const Polynomial& A, const Polynomial& B) {
	/* add two polynomials together */
	// Note: please make sure A & B are sorted before adding them together
	Polynomial C;
	Term* pa = A.getHead()->next, *pb = B.getHead()->next, *pc = C.getHead();
	while (pa != nullptr && pb != nullptr) {
		if (pa->exp > pb->exp) {
			C.insert_after(pc, pa->coef, pa->exp);
			pc = pc->next;
			pa = pa->next;
		}
		else
			if (pa->exp < pb->exp) {
				C.insert_after(pc, pb->coef, pb->exp);
				pc = pc->next;
				pb = pb->next;
			}
			else {  // pa->exp == pb->exp
				float sum = pa->coef + pb->coef;
				if (sum != 0.0) {
					C.insert_after(pc, sum, pa->exp);
					pc = pc->next;
				}
				pa = pa->next;
				pb = pb->next;
			}
	}
	Term* residual;
	if (pa == nullptr)
		residual = pb;
	else
		residual = pa;
	// copy rest terms of polynomial A or B
	while (residual != nullptr) {
		C.insert_after(pc, residual->coef, residual->exp);
		pc = pc->next;
		residual = residual->next;
	}
	return C;
}

Polynomial& Polynomial::opposite() {
	/* opposite of a polynomial (multiply it by -1) */
	Term* curr = head->next;
	while (curr != nullptr) {
		curr->coef = -curr->coef;
		curr = curr->next;
	}
	return *this;
}

Polynomial operator-(const Polynomial& A, const Polynomial& B) {
	/* subtract polynomial B from A */
	// Note: please make sure A & B are sorted before subtracting B from A	

	// perform B.opposite() will change B, which, however is not what we want.
	// Therefore we need a copy of B to perform this task.
	Polynomial copy_of_B = B;
	return A + copy_of_B.opposite();

}

Polynomial operator*(const Polynomial& A, const Polynomial& B) {
	/* solve the product of two polynomials */
	// Note: please make sure A & B are sorted before multiplying them
	Polynomial C;
	Term* pa = A.getHead()->next, *pb = B.getHead()->next, *pc = C.getHead();
	int maxExp = A.order() + B.order();
	int order;
	// store those coefficients of the product of two terms of various exponent
	float* result = new float[maxExp + 1]{};			// default initialization with the value of 0.0 for each element
	while (pa != nullptr) {
		while (pb != nullptr) {
			order = pa->exp + pb->exp;
			result[order] = result[order] + pa->coef*pb->coef;
			pb = pb->next;
		}
		pa = pa->next;
		pb = B.getHead()->next;
	}
	for (int i = maxExp; i >= 0; --i) {
		if (result[i] != 0.0) {
			C.insert_after(pc, result[i], i);
			pc = pc->next;
		}
	}
	return C;
}

