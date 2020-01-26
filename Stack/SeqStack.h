// sequential stack header
#pragma once
#ifndef SEQSTACK_H
#define SEQSTACK_H

#include "Stack.h"
#include <iostream>
#include <assert.h>

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
	void clear() { TOP = -1; }
	inline bool isEmpty()const { return TOP == -1 ? true : false; }
	bool isFull()const { return TOP == maxSize-1 ? true : false; }
	int max_size()const { return maxSize; }
	inline int size()const { return TOP + 1; }

private:
	T* elem;					// pointer to stack array
	int TOP;					// index of stack top: [0, size), when stack's empty, TOP=-1
	int maxSize;				// maximum volume of the stack
	void overflowProcess();	
};

template<typename T>
SeqStack<T>::SeqStack(int sz) {	// construct an empty stack with a size of sz
	TOP = -1;
	maxSize = sz;
	elem = new T[maxSize];
	assert(elem != nullptr);
}

template<typename T>
SeqStack<T>::SeqStack(const SeqStack<T>& stack) { // copy constructor
	TOP = stack.TOP;
	maxSize = stack.maxSize;
	elem = new T[maxSize];
	assert(elem != nullptr);
	for (int i = 0; i <= TOP; ++i) elem[i] = stack.elem[i];
}

template<typename T>
SeqStack<T>& SeqStack<T>::operator=(const SeqStack<T>& stack) { // copy assignment
	if (&stack == this)	return *this;
	delete[] elem;
	TOP = stack.TOP;
	maxSize = stack.maxSize;
	elem = new T[maxSize];
	assert(elem != nullptr);
	for (int i = 0; i <= TOP; ++i) elem[i] = stack.elem[i];
	return *this;
}

template<typename T>
void SeqStack<T>::push(const T& x){ // add a new element at the top of the stack
	if (isFull()) overflowProcess();
	elem[++TOP] = x;
}

template<typename T>
void SeqStack<T>::pop() { // pop out the top element of the stack
	if (isEmpty()) throw "fun@pop: stack_underflow\n";
	--TOP;
}

template<typename T>
void SeqStack<T>::pop(T& x) { // pop out the top element of the stack and assign it to x
	if (isEmpty()) throw "fun@pop: stack_underflow\n";
	x = elem[TOP--];
}

template<typename T>
T& SeqStack<T>::top() const { // get the top element
	if (isEmpty()) throw "fun@top: stack_underflow\n";
	return elem[TOP];
}

template<typename T>
void SeqStack<T>::overflowProcess() { // private member function, expanding the stack's size 
	T* newArray = new T[maxSize += stackIncreament];
	assert(newArray != nullptr);
	for (int i = 0; i <= TOP; ++i) newArray[i] = elem[i];
	delete elem;
	elem = newArray;
}

#endif // !SEQSTACK_H
