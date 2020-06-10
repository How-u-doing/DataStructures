#pragma once
#ifndef MYSTRINGSEARCH_H
#define MYSTRINGSEARCH_H
#include <string>
#include <vector>

std::vector<int> BF_search(std::string pat, std::string txt);
std::vector<int> FL_search(std::string pat, std::string txt);

std::vector<int> KMP_search(std::string pat, std::string txt);
void KMP_table(int pi[], std::string P, int m);

std::vector<int> BM_search(std::string pat, std::string txt);


#endif // !MYSTRINGSEARCH_H
