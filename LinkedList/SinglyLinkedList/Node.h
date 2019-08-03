#pragma once
#ifndef NODE_H
#define NODE_H
#endif // !NODE_H

#include<iostream>
using namespace std;
template<typename T>
struct Node {
	T data;
	Node<T>* next;
	Node(Node<T>* ptr = nullptr) { next = ptr; }
	Node(const T& val, Node<T>* ptr = nullptr) { data = val; next = ptr; }
	friend ostream& operator<<(ostream& os, Node<T>& node) { os << node.data; return os; }
	friend istream& operator>>(istream& is, Node<T>& node) { is >> node.data; return is; }
};

// simple ugly iterator for Node
template<typename T>
struct Nodeptr {
	Node<T>* ptr;									// data item
	Nodeptr(Node<T>* p = nullptr) :ptr(p) {}		// constructor
	Nodeptr& operator++() {
		// preincrement
		ptr = ptr->next;
		return *this;
	}
	Nodeptr operator++(int) {
		// postincrement
		Nodeptr tmp(*this);
		ptr = ptr->next;							// ++(*this)
		return tmp;
	}
	Node<T>* operator->() { return ptr; }			// dereference to access member
	Node<T>& operator*() { return *ptr; }			// dereference to access whole object
};
