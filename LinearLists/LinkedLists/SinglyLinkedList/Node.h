// node header
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
	Nodeptr<T> operator=(Node<T>* p) { ptr = p; }
	Nodeptr<T> operator=(Nodeptr<T> p) { ptr = p; }
	bool operator==(Nodeptr<T> p) { return ptr == p.ptr ? true : false; }
	bool operator==(Node<T>* p) { return ptr == p ? true : false; }
	bool operator!=(Nodeptr<T> p) { return ptr == p.ptr ? false : true; }
	bool operator!=(Node<T>* p) { return ptr == p ? false : true; }
	Nodeptr<T> operator+=(int n) {
		if (n < 0) { cerr << "Invalid argument n, n must be no less than 0." << endl; exit(1); }		
		while (n--) {
			++(*this);
			if (this->ptr == nullptr) {
				cerr << "Maximum backward-moving-steps exceeded." << endl;
				exit(1);
			}
		}
		return *this;		
	}
	Node<T>* operator->() { return ptr; }			// dereference to access member
	Node<T>& operator*() { return *ptr; }			// dereference to access whole object
};
