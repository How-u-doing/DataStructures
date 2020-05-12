// priority queue, implemented by array
#pragma once
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <algorithm>	// using swap
#include <cassert>

const size_t defaultSize = 20;

template<typename T>
class PriorityQueue {
public:
	PriorityQueue(size_t sz = defaultSize);
	PriorityQueue(const PriorityQueue<T>& que);
	PriorityQueue<T>& operator=(const PriorityQueue<T>& que);
	~PriorityQueue() { delete[] elem; }
	void push(const T& _Val);
	void pop();
	T& top()const;
	inline size_t size()const { return count; }
	inline size_t capacity()const { return maxSize; }
	inline void clear() { count = 0; }
	inline bool isEmpty()const { return count == 0; }
	bool isFull()const { return count == maxSize; }

private:
	T* elem;
	size_t count;
	size_t maxSize;
	void adjust();
};

template<typename T>
PriorityQueue<T>::PriorityQueue(size_t sz) :maxSize(sz), count(0) { // construct an empty queue with a size of sz	
	elem = new T[maxSize];
	assert(elem != nullptr);
}

template<typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& que) { // copy constructor
	maxSize = que.maxSize; count = que.count;
	elem = new T[maxSize];
	assert(elem != nullptr);
	for (size_t i = 0; i < count; ++i)
		elem[i] = que.elem[i];
}

template<typename T>
PriorityQueue<T>& PriorityQueue<T>::operator=(const PriorityQueue<T>& que) { // copy assignment
	if (&que == this)	return *this;
	delete[] elem;
	maxSize = que.maxSize; count = que.count;
	elem = new T[maxSize];
	assert(elem != nullptr);
	for (size_t i = 0; i < count; ++i)
		elem[i] = que.elem[i];
	return *this;
}

template<typename T>
void PriorityQueue<T>::push(const T& _Val) { // add a new element at rear and adjust it by priority
	assert(!isFull());
	elem[count++] = _Val;
	adjust();
}

template<typename T>
void PriorityQueue<T>::adjust() { // adjust rear element so that all elements are in particular order
								  // here we make the first element the highest priority	
	for (size_t i = count - 1; i >= 1; --i)
		if (elem[i] > elem[i - 1])	// here we require class T are comparable( overloaded operator> )
			std::swap(elem[i], elem[i - 1]);
}

template<typename T>
void PriorityQueue<T>::pop() { // pop off the first element
	assert(!isEmpty());
	for (size_t i = 1; i < count; ++i)
		elem[i - 1] = elem[i];		// move forward respectively
	--count;
}

template<typename T>
T& PriorityQueue<T>::top() const { // get the first element
	assert(!isEmpty());
	return elem[0];
}

#endif // !PRIORITYQUEUE_H
