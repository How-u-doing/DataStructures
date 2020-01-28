// queue header, base class for interfaces
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

template<typename T> 
class Queue {
public:
	virtual void push(const T& x) = 0;
	virtual void pop() = 0;
	virtual T& front()const = 0;
	virtual T& back()const = 0;
	virtual void clear() = 0;
	virtual bool isEmpty()const = 0;
	virtual size_t size()const = 0;
};


#endif // !QUEUE_H
