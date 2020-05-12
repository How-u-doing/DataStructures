// circular sequential deque header
#pragma once
#ifndef SEQDEQUE_H
#define SEQDEQUE_H

#include "Deque.h"
#include <iostream>
#include <cassert>

template<typename T>
class SeqDeque :public Deque<T> {
public:
	SeqDeque(size_t sz = defaultSize);
	SeqDeque(const SeqDeque<T>& que);
	~SeqDeque() { delete[] elem; }
	SeqDeque<T>& operator=(const SeqDeque<T>& que);
	virtual void push_front(const T& _Val);
	virtual void push_back(const T& _Val);
	virtual void pop_back();
	virtual void pop_front();
	virtual T& front()const;
	virtual T& back()const;
	virtual inline size_t size()const { return (maxSize + Back - Front) % maxSize; }
	virtual inline void clear() { Front = Back = 0; }
	virtual inline bool isEmpty()const { return Front == Back; }
	inline bool isFull()const { return (Back + 1) % maxSize == Front; }
	inline size_t capacity()const { return maxSize - 1; }
	void resize(size_t newSize = maxSize * 2);	// double it by default
	template<typename T>
	friend std::ostream& operator<< (std::ostream& os, const SeqDeque<T>& que);

private:
	size_t Front, Back;	// element scope: [Front, Back)
	T* elem;
	size_t maxSize;
};

template<typename T>
SeqDeque<T>::SeqDeque(size_t sz) {	// construct an empty queue with a size of sz
	Front = Back = 0;
	maxSize = sz;
	elem = new T[maxSize];
	assert(elem != nullptr);
}

template<typename T>
SeqDeque<T>::SeqDeque(const SeqDeque<T>& que) { // copy constructor
	Front = que.Front;
	Back = que.Back;
	maxSize = que.maxSize;
	elem = new T[maxSize];
	assert(elem != nullptr);
	size_t i = Front;
	while (i != Back) {
		elem[i] = que.elem[i];
		i = (i + 1) % maxSize;
	}
}

template<typename T>
SeqDeque<T>& SeqDeque<T>::operator=(const SeqDeque<T>& que) { // copy assignment
	if (&que == this)	return *this;
	delete[] elem;
	Front = que.Front;
	Back = que.Back;
	maxSize = que.maxSize;
	elem = new T[maxSize];
	assert(elem != nullptr);
	size_t i = Front;
	while (i != Back) {
		elem[i] = que.elem[i];
		i = (i + 1) % maxSize;
	}
	return *this;
}

template<typename T>
T& SeqDeque<T>::front() const { // get the first element
	assert(!isEmpty());
	return elem[Front];
}

template<typename T>
T& SeqDeque<T>::back() const { // get the last element
	assert(!isEmpty());
	return elem[(Back - 1 + maxSize) % maxSize];
	// or return elem[(Back - 1 + Back == 0 ? maxSize : 0];
}

template<typename T>
void SeqDeque<T>::push_back(const T& _Val) { // add a new element at rear
	if (isFull()) resize(2 * maxSize);
	elem[Back] = _Val;
	Back = (Back + 1) % maxSize;
}

template<typename T>
void SeqDeque<T>::push_front(const T& _Val) {
	if (isFull()) resize(2 * maxSize);
	Front = (Front - 1 + maxSize) % maxSize;
	elem[Front] = _Val;
}

template<typename T>
void SeqDeque<T>::resize(size_t newSize) { // expanding the queue's size
	if (newSize <= maxSize)	return;		   // do NOT shrink	
	T* newArray = new T[newSize];
	assert(newArray != nullptr);

	// copy start from Front
	size_t i = Front, j = i;
	while (i != Back) {
		newArray[j] = elem[i];
		i = (i + 1) % maxSize;
		j = (j + 1) % newSize;
	}
	Back = j;
	maxSize = newSize;

	delete[] elem;
	elem = newArray;
}

template<typename T>
void SeqDeque<T>::pop_front() { // pop off the front element
	assert(!isEmpty());
	Front = (Front + 1) % maxSize;
}

template<typename T>
void SeqDeque<T>::pop_back() {
	assert(!isEmpty());
	Back = (Back - 1 + maxSize) % maxSize;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const SeqDeque<T>& que) {
	for (auto i = que.Front; i != que.Back; i = (i + 1) % que.maxSize)
		os << i << ": " << que.elem[i] << std::endl;
	return os;
}
#endif // !SEQDEQUE_H
