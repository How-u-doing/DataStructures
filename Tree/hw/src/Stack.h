// stack header, abstract base class for interfaces
#pragma once
#ifndef STACK_H
#define STACK_H

template<typename T>
class Stack {
public:
	virtual void push(const T& x) = 0;
	virtual void pop() = 0;
	virtual void pop(T& x) = 0;
	virtual T& top()const = 0;
	virtual void clear() = 0;
	virtual bool isEmpty()const = 0;
	virtual int size()const = 0;
};

#endif // !STACK_H
