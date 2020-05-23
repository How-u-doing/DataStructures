// Sparse Matrix header
#pragma once
#ifndef SMATRIX_H
#define SMATRIX_H
//#include <initializer_list>
#include <iostream>
#include <cassert>

constexpr size_t defaultSize = 20;

// tri-tuple term for parse matrix by the form <row, col, value>
template<typename T>
class TriTuple {
	template<typename U> friend class SMatrix;
	template<typename U> // enable friend of SMatrix access private members of TriTuple
	friend std::ostream& operator<<(std::ostream& os, const SMatrix<U>& M);
private:
	size_t _row, _col;
	T _val;
public:
	TriTuple(size_t row = 0, size_t col = 0, T value = {})
		: _row(row), _col(col), _val(value) {}

	TriTuple<T>& operator=(const TriTuple<T>& term) {
		_row = term._row;
		_col = term._col;
		_val = term._val;
		return *this;
	}
};

template<typename T>
class SMatrix {
public:
	SMatrix(size_t maxSize = defaultSize);
	SMatrix(size_t rows, size_t cols, std::initializer_list<TriTuple<T>> elemList, size_t maxSize = defaultSize);
	SMatrix(const SMatrix<T>& M);
	~SMatrix() { delete[] _arr; };

	void printHeader()const;
	SMatrix<T>& operator=(const SMatrix<T>& M);
	SMatrix<T> transpose()const;
	SMatrix<T> fast_transpose()const;
	SMatrix<T> add(const SMatrix<T>& M)const;
	SMatrix<T> multiply(const SMatrix<T>& M)const;

private:
	size_t _rows, _cols;// # of rows & columns
	size_t _terms;		// # of terms
	TriTuple<T>* _arr;	// stored by 1-dimensional array
	size_t _maxSize;

	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, const SMatrix<U>& M);
};

template<typename T>
inline SMatrix<T>::SMatrix(size_t maxSize)
	:_rows(0), _cols(0), _terms(0), _maxSize(maxSize)
{
	_arr = new TriTuple<T>[maxSize] {};
	assert(_arr != nullptr);
}

template<typename T>
inline SMatrix<T>::SMatrix(size_t rows, size_t cols, std::initializer_list<TriTuple<T>> elemList, size_t maxSize)
	: _rows(rows), _cols(cols), _maxSize(maxSize)
{
	// assignment examples: {{1,3,20.6},{9,7,18.9},{15,12,21.3}}	
	_arr = new TriTuple<T>[maxSize];
	assert(_arr != nullptr);

	size_t i = 0;
	for (auto it = elemList.begin(); it != elemList.end() && i < maxSize; ++it)
		_arr[i++] = *it;

	_terms = i;
}

template<typename T>
inline SMatrix<T>::SMatrix(const SMatrix<T>& M)
	:_rows(M._rows), _cols(M._cols), _terms(M._terms), _maxSize(M._maxSize)
{
	_arr = new TriTuple<T>[_maxSize];
	assert(_arr != nullptr);
	for (size_t i = 0; i < _terms; ++i)
		_arr[i] = M._arr[i];
}

template<typename T>
inline void SMatrix<T>::printHeader()const {
	std::cout << "-----------------------------------\n";
	std::cout << "row            col            value\n";
	std::cout << "-----------------------------------\n";
}

template<typename T>
inline SMatrix<T>& SMatrix<T>::operator=(const SMatrix<T>& M) {
	_rows = M._rows;
	_cols = M._cols;
	_terms = M._terms;
	_arr = new TriTuple<T>[M._maxSize];
	assert(_arr != nullptr);
	for (size_t i = 0; i < _terms; ++i)
		_arr[i] = M._arr[i];

	return *this;
}

template<typename T>
SMatrix<T> SMatrix<T>::transpose()const
{
	SMatrix<T> B(_maxSize);
	B._rows = _rows;
	B._cols = _cols;
	B._terms = _terms;
	if (_terms > 0) {
		size_t posB = 0;
		// Since sparse matrix is stored by rows, we need to traverse by
		// columns to find those nonzeros & exchange their <row, col>.
		for (size_t j = 0; j < _cols; ++j) {
			for (size_t k = 0; k < _terms; ++k) {
				if (_arr[k]._col == j) {
					B._arr[posB]._row = j;
					B._arr[posB]._col = _arr[k]._row;
					B._arr[posB]._val = _arr[k]._val;
					++posB;
				}
			}
		}
	}
	return B;
}

template<typename T>
inline SMatrix<T> SMatrix<T>::fast_transpose()const
{
	return SMatrix<T>();
}

template<typename T>
inline SMatrix<T> SMatrix<T>::add(const SMatrix<T>& M)const
{
	return SMatrix<T>();
}

template<typename T>
inline SMatrix<T> SMatrix<T>::multiply(const SMatrix<T>& M)const
{
	
	return SMatrix<T>();
}

template<typename U>
std::ostream& operator<<(std::ostream& os, const SMatrix<U>& M)
{
	M.printHeader();
	for (size_t i = 0; i < M._terms; ++i) {
		os << M._arr[i]._row << "\t\t" << M._arr[i]._col << "\t\t" << M._arr[i]._val << '\n';
	}
	return os;
}

#endif // !SMATRIX_H

