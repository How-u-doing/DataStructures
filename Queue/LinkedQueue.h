// linked queue header
#pragma once
#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include "Queue.h"
#include <iostream>

template<typename T>
struct Node {
	T data;
	Node<T>* next;
	Node(Node<T>* Next = nullptr) { next = Next; }
	Node(const T& Val, Node<T>* Next = nullptr) { data = Val; next = Next; }
};

template<typename T>
class LinkedQueue :public Queue<T> {
public:
	LinkedQueue() :Front(nullptr), Back(nullptr) {}
	LinkedQueue(const LinkedQueue<T>& que);
	LinkedQueue<T>& operator=(const LinkedQueue<T>& que);
	~LinkedQueue() { clear(); }
	virtual void push(const T& x);
	virtual void pop();
	virtual T& front()const;
	virtual T& back()const;
	virtual void clear();
	virtual bool isEmpty()const { return Front == nullptr ? true : false; }
	virtual size_t size()const;
	template<typename T>
	friend std::ostream& operator<< (std::ostream& os, const LinkedQueue<T>& que);

private:
	Node<T>* Front, * Back;		// elements scope: [Front, Back]
};

template<typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& que) { // copy constructor
	Node<T>* srcptr = que.Front, * desptr;
	if (srcptr != nullptr) {
		desptr = Front = new Node<T>(srcptr->data);
		if (Front == nullptr) throw "memory_allocation_failure\n";
		srcptr = srcptr->next;
	}
	else { Front = Back = nullptr;	return; }

	while (srcptr != nullptr) {
		desptr->next = new Node<T>(srcptr->data);
		if (desptr->next == nullptr) throw "memory_allocation_failure\n";
		desptr = desptr->next;
		srcptr = srcptr->next;
	}
	Back = desptr;
}

template<typename T>
LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& que) { // copy assignment
	if (&que == this) return *this;
	clear();
	Node<T>* srcptr = que.Front, * desptr;
	if (srcptr != nullptr) {
		desptr = Front = new Node<T>(srcptr->data);
		if (Front == nullptr) throw "memory_allocation_failure\n";
		srcptr = srcptr->next;
	}
	else { Front = Back = nullptr;	return *this; }

	while (srcptr != nullptr) {
		desptr->next = new Node<T>(srcptr->data);
		if (desptr->next == nullptr) throw "memory_allocation_failure\n";
		desptr = desptr->next;
		srcptr = srcptr->next;
	}
	Back = desptr;
	return *this;
}

template<typename T>
void LinkedQueue<T>::push(const T& x) { // add a new element at rear
	if (Front == nullptr) {
		Front = Back = new Node<T>(x, nullptr);
		if (Front == nullptr) throw "memory_allocation_failure\n";
		return;
	}
	Back->next = new Node<T>(x, nullptr);
	if (Back->next == nullptr) throw "memory_allocation_failure\n";
	Back = Back->next;	
}

template<typename T>
void LinkedQueue<T>::pop() { // pop off the front element
	if (isEmpty()) throw "fun@pop: queue_underflow\n";
	Node<T>* del = Front;
	Front = Front->next;
	delete del;
}

template<typename T>
T& LinkedQueue<T>::front()const { // get the front element
	if (isEmpty()) throw "fun@front: queue_underflow\n";
	return Front->data;
}

template<typename T>
T& LinkedQueue<T>::back()const { // get the rear element
	if (isEmpty()) throw "fun@back: queue_underflow\n";
	return Back->data;
}

template<typename T>
size_t LinkedQueue<T>::size()const { // the length of the queue
	size_t n = 0;
	Node<T>* curr = Front;
	while (curr != nullptr) {
		++n;
		curr = curr->next;
	}
	return n;
}

template<typename T>
void LinkedQueue<T>::clear() { // erase all elements
	Node<T>* del;
	while (Front != nullptr) {
		del = Front;
		Front = Front->next;
		delete del;
	}
	Back = nullptr;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const LinkedQueue<T>& que) { // print all elements from front to back
	size_t i = que.size();
	if (i == 0) { os << "The que is empty, no element to print\n"; return os; }
	else
		os << "There are " << i << " element(s) in this que, from front to rear they are:\n";

	Node<T>* curr = que.Front;
	i = 0;
	while (curr != nullptr) {
		printf("#%2d: ", i++);
		os << curr->data << '\n';
		curr = curr->next;
	}
	return os;
}

#endif // LINKEDQUEUE_H
