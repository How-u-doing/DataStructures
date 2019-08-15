// linear list header
#pragma once
#ifndef LINEARLIST_H
#define LINEARLIST_H

template<typename T>
class LinearList {
public:
	virtual ~LinearList() {};								// virtual destructor
	virtual int size()const = 0;							// get the maximum volume of the list	
	virtual int length()const = 0;							// get the number of pactical existing items
	virtual int search(const T& x)const = 0;				// search specified item x and return its logical sequence number 
	virtual bool getVal(int i, T& x)const = 0;				// get the i-th item value via reference x, and return true if succeed & return false otherwise
	virtual T& getVal(int i)const = 0;						// get the i-th item value & return its reference
	virtual void setVal(int i, const T& x) = 0;				// set value x for the i-th item
	virtual bool insert(int i, const T& x) = 0;				// insert value x after the i-th item, 0<=i<=index_of_last_one_in_logical
	virtual bool append(const T& x) = 0;			 		// add value x at the end of list
	virtual bool remove(int i, T& x) = 0;					// remove the i-th item & store the removed value
	virtual bool remove(int i) = 0;							// remove the i-th item without storing the removed value
	virtual bool isEmpty()const = 0;						// tell if the list is empty
	virtual bool isFull()const = 0;							// tell if the list is full
	virtual void sort() = 0;								// sort the list in a specific manner
	virtual void input() = 0;								// input data
	virtual void output()const = 0;							// output data
	virtual void clear() = 0;								// erase all items
	virtual T& operator[](int index)const = 0;				// access list items via []
	virtual void Import(const std::string& filename, const std::string& mode_selection_text_or_binary) = 0;	    // Read corresponding data from a local host file
	virtual void Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const = 0;// Write corresponding data into a local host file	
	virtual void swap(int i, int j) = 0;					// exchange i-th item with j-th item 

};
#endif // !LINEARLIST_H

