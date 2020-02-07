// linked stack header
#pragma once
#ifndef LINKEDSTACK
#define LINKEDSTACK

#include "Stack.h"
#include <iostream>
#include <cassert>

template<typename T>
struct Node {
	T data;
	Node<T>* next;
	Node(Node<T>* Next = nullptr) { next = Next; }
	Node(const T& Val, Node<T>* Next = nullptr) { data = Val; next = Next; }
};

template<typename T>
class LinkedStack : public Stack<T> {
public:
	LinkedStack() : _top(nullptr) {}
	LinkedStack(const LinkedStack<T>& stack);
	LinkedStack<T>& operator=(const LinkedStack<T>& stack);
	~LinkedStack() { clear(); }
	void push(const T& x);
	void pop(T& x);
	void pop();
	T& top()const;
	void clear();
	bool isEmpty()const { return _top == nullptr; }
	int size()const;
	//template<typename T>	// uncomment this line under a Linux/gcc compiler
	friend std::ostream& operator<<(std::ostream& os, const LinkedStack<T>& stack);
	
private:
	Node<T>* _top;
};


template<typename T>
LinkedStack<T>::LinkedStack(const LinkedStack<T>& stack) { // copy constructor
	Node<T>* srcptr = stack._top, *desptr;
	if (srcptr != nullptr) {
		desptr = _top = new Node<T>(srcptr->data);
		assert(_top != nullptr);
		srcptr = srcptr->next;
	}
	else {	_top = nullptr;	return;	}
	
	while (srcptr != nullptr) {
		desptr->next = new Node<T>(srcptr->data);
		assert(desptr->next != nullptr);
		desptr = desptr->next;
		srcptr = srcptr->next;
	}	
}


template<typename T>
LinkedStack<T>& LinkedStack<T>::operator=(const LinkedStack<T>& stack) { // copy assignment
	if (&stack == this) return *this;
	clear();
	Node<T>* srcptr = stack._top, * desptr;
	if (srcptr != nullptr) {
		desptr = _top = new Node<T>(srcptr->data);
		assert(_top != nullptr);
		srcptr = srcptr->next;
	}
	else { _top = nullptr;	return; }

	while (srcptr != nullptr) {
		desptr->next = new Node<T>(srcptr->data);
		assert(desptr->next != nullptr);
		desptr = desptr->next;
		srcptr = srcptr->next;
	}
	return *this;
}

template<typename T>
void LinkedStack<T>::push(const T& x) { // add a new element at the top of the stack	
	_top = new Node<T>(x, _top);
	assert(_top != nullptr);
}

template<typename T>
void LinkedStack<T>::pop(T& x) { // pop off the top element of the stack & get its value via x
	if (isEmpty()) throw "fun@pop: stack_underflow\n";
	x = _top->data;
	Node<T>* del = _top;
	_top = _top->next;
	delete del;
}

template<typename T>
void LinkedStack<T>::pop() { // pop off the top element of the stack
	if (isEmpty()) throw "fun@pop: stack_underflow\n";
	Node<T>* del = _top;
	_top = _top->next;
	delete del;
}

template<typename T>
T& LinkedStack<T>::top()const { // get the top element
	if (isEmpty()) throw "fun@top: stack_underflow\n";
	return _top->data;
}

template<typename T>
int LinkedStack<T>::size()const { // the length of the stack
	int n = 0;
	Node<T>* curr = _top;
	while (curr != nullptr) {
		++n;
		curr = curr->next;
	}
	return n;
}

template<typename T>
void LinkedStack<T>::clear() { // erase all elements
	Node<T>* del;
	while (_top != nullptr) {
		del = _top;
		_top = _top->next;
		delete del;
	}
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const LinkedStack<T>& s) { // print all elements from top to bottom
	int i = s.size();
	if (i == 0) { os << "The stack is empty, no element to print\n"; return; }
	else
		os << "There are " << i << " element(s) in this stack, from top to bottom they are:\n";

	Node<T>* curr = s._top;
	while (curr != nullptr) {
		printf("#%2d: ", i--);
		os << curr->data << '\n';
		curr = curr->next;
	}
	return os;
}

#endif // !LINKEDSTACK

