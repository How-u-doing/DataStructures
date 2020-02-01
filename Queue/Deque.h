// deque (double-ended queue) header
#pragma once
#ifndef DEQUE_H
#define DEQUE_H

template<typename T>
class Deque {
public:
	virtual void push_front(const T& x) = 0;
	virtual void push_back(const T& x) = 0;
	virtual void pop_front() = 0;
	virtual void pop_back() = 0;
	virtual T& front()const = 0;
	virtual T& back()const = 0;
	virtual void clear() = 0;
	virtual bool isEmpty()const = 0;
	virtual size_t size()const = 0;
};

#endif // !DEQUE_H
