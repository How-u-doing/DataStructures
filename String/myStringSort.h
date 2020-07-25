#ifndef MYSTRINGSORT_H
#define MYSTRINGSORT_H
#include <string>

namespace myStringSort {

const int Radix = 128;	// radix of ASCII (7-bit) character
void LSD(std::string a[], int n, int w);
void MSD(std::string a[], int n, int w);

}// namespace myStringSort


#endif // !MYSTRINGSORT_H
