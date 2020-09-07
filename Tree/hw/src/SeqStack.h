// sequential stack header
#pragma once
#ifndef SEQSTACK_H
#define SEQSTACK_H

#include "Stack.h"
#include <iostream>
#include <cassert>

const int defaultStackSize = 30;
const int stackIncreament = 20;		// let it double its capacity 

template<typename T>
class SeqStack : public Stack<T> {
public:
	SeqStack(int sz = defaultStackSize);
	SeqStack(const SeqStack<T>& stack);
	SeqStack<T>& operator=(const SeqStack<T>& stack);
	~SeqStack() { delete[] elem; }
	void push(const T& x);
	void pop();
	void pop(T& x);
	T& top()const;
	void clear() { _top = -1; }
	inline bool isEmpty()const { return _top == -1; }
	bool isFull()const { return _top == maxSize - 1; }
	int max_size()const { return maxSize; }
	inline int size()const { return _top + 1; }
	void bottom_up_traverse()const;

private:
	T* elem;					// pointer to stack array
	int _top;					// index of stack top: [0, size), when stack's empty, _top=-1
	int maxSize;				// maximum volume of the stack
	void overflowProcess();	
};

template<typename T>
SeqStack<T>::SeqStack(int sz) {	// construct an empty stack with a size of sz
	_top = -1;
	maxSize = sz;
	elem = new T[maxSize];
	assert(elem != nullptr);
}

template<typename T>
SeqStack<T>::SeqStack(const SeqStack<T>& stack) { // copy constructor
	_top = stack._top;
	maxSize = stack.maxSize;
	elem = new T[maxSize];
	assert(elem != nullptr);
	for (int i = 0; i <= _top; ++i) elem[i] = stack.elem[i];
}

template<typename T>
SeqStack<T>& SeqStack<T>::operator=(const SeqStack<T>& stack) { // copy assignment
	if (&stack == this)	return *this;
	delete[] elem;
	_top = stack._top;
	maxSize = stack.maxSize;
	elem = new T[maxSize];
	assert(elem != nullptr);
	for (int i = 0; i <= _top; ++i) elem[i] = stack.elem[i];
	return *this;
}

template<typename T>
void SeqStack<T>::push(const T& x){ // add a new element at the top of the stack
	if (isFull()) overflowProcess();
	elem[++_top] = x;
}

template<typename T>
void SeqStack<T>::pop() { // pop out the top element of the stack
	assert(!isEmpty());
	--_top;
}

template<typename T>
void SeqStack<T>::pop(T& x) { // pop out the top element of the stack and assign it to x
	assert(!isEmpty());
	x = elem[_top--];
}

template<typename T>
T& SeqStack<T>::top() const { // get the top element
	assert(!isEmpty());
	return elem[_top];
}

template<typename T>
void SeqStack<T>::bottom_up_traverse() const
{
	for (int i = 0; i <= _top; ++i) {
		std::cout << elem[i] << '\t';
	}
}

template<typename T>
void SeqStack<T>::overflowProcess() { // private member function, expanding the stack's size 
	T* newArray = new T[maxSize += stackIncreament];
	assert(newArray != nullptr);
	for (int i = 0; i <= _top; ++i) newArray[i] = elem[i];
	delete elem;
	elem = newArray;
}

#endif // !SEQSTACK_H
