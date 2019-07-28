#pragma once
#ifndef LINEARLIST_H
#define LINEARLIST_H
template<typename T>
class LinearList {
public:
	//LinearList() {};										// Constructor
	//~LinearList() {};										// Destructor
	virtual int size()const = 0;							// Get the maximum volume of the list
	virtual void resize(int newsz)=0;						// Change the size(maximum volume) of the list
	virtual int length()const = 0;							// Get the number of pactical existing items
	virtual int search(const T& x)const = 0;				// Search specified item x and return its logical sequence number 
	virtual bool getData(int i, T& x)const = 0;				// Get the i-th item value via reference x, and return true if succeed & return false otherwise
	virtual void setData(int i, const T& x) = 0;			// Set value x for the i-th item
	virtual bool insert(int i, const T& x) = 0;				// Insert value x after the i-th item, 0<=i<=index_of_last_one_in_logical
	virtual bool append(int i, const T& x) = 0;		 		// Add value x at the end of list
	virtual bool remove(int i, T& x) = 0;					// Remove the i-th item & store the removed value
	virtual bool remove(int i) = 0;							// Remove the i-th item without storing the removed value
	virtual bool isEmpty()const = 0;						// Tell if the list is empty
	virtual bool isFull()const = 0;							// Tell if the list is full
	virtual void sort() = 0;								// Sort the list in a specific manner
	virtual void input() = 0;								// Input data
	virtual void import() = 0;								// Import corresponding data from a local host file
	virtual void output()const = 0;							// Output data
	virtual void export()const = 0;							// Export corresponding data into a local host file
	//virtual LinearList<T> operator=(LinearList<T>& L) = 0;// Copy Constructor
};
#endif // !LINEARLIST_H

