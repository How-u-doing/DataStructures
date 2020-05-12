// circular sequential queue header
#pragma once
#ifndef SEQQUEUE_H
#define SEQQUEUE_H

#include "Queue.h"
#include <iostream>
#include <cassert>

const size_t defaultSize = 20;

template<typename T>
class SeqQueue :public Queue<T> {
public:
	SeqQueue(size_t sz = defaultSize);
	SeqQueue(const SeqQueue<T>& que);
	SeqQueue<T>& operator=(const SeqQueue<T>& que);
	~SeqQueue() { delete[] elem; }
	virtual void push(const T& x);
	virtual void pop();
	virtual T& front()const;
	virtual T& back()const;
	virtual inline size_t size()const { return (maxSize + Back - Front) % maxSize; }
	virtual inline void clear() { Front = Back = 0; }
	virtual inline bool isEmpty()const { return Front == Back; }
	inline bool isFull()const { return (Back + 1) % maxSize == Front; }
	inline size_t capacity()const { return maxSize - 1; }
	void resize(size_t newSize = maxSize * 2);	// double it by default
	template<typename T>
	friend std::ostream& operator<< (std::ostream& os, const SeqQueue<T>& que);

private:	
	size_t Front, Back;	// element scope: [Front, Back)
	T* elem;
	size_t maxSize;
};

template<typename T>
SeqQueue<T>::SeqQueue(size_t sz) {	// construct an empty queue with a size of sz
	Front = Back = 0;
	maxSize = sz;
	elem = new T[maxSize];
	assert(elem != nullptr);
}

template<typename T>
SeqQueue<T>::SeqQueue(const SeqQueue<T>& que) { // copy constructor
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
SeqQueue<T>& SeqQueue<T>::operator=(const SeqQueue<T>& que) { // copy assignment
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
void SeqQueue<T>::push(const T& x) { // add a new element at rear
	if (isFull()) resize(2 * maxSize);
	elem[Back] = x;
	Back = (Back + 1) % maxSize;
}

template<typename T>
void SeqQueue<T>::resize(size_t newSize) { // expanding the queue's size
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
void SeqQueue<T>::pop() { // pop off the front element
	assert(!isEmpty());
	Front = (Front + 1) % maxSize;
}

template<typename T>
T& SeqQueue<T>::front() const { // get the front element
	assert(!isEmpty());
	return elem[Front];
}

template<typename T>
T& SeqQueue<T>::back() const { // get the rear element
	assert(!isEmpty());
	return elem[(Back - 1 + maxSize) % maxSize];
	// or return elem[(Back - 1 + Back == 0 ? maxSize : 0];
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const SeqQueue<T>& que) {
	for (auto i = que.Front; i != que.Back; i = (i + 1) % que.maxSize)
		os << i << ": " << que.elem[i] << std::endl;
	return os;
}
#endif // !SEQQUEUE_H
