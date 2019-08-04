// sequential list header
#pragma once
#ifndef SEQLIST_H
#define SEQLIST_H

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include"LinearList.h"
using namespace std;

const int defaultSize = 100;						// Default size of list, you can modify it here

template<typename T>
class SeqList : public LinearList<T> {
public:	
	SeqList(int sz=defaultSize);					// Constructor	
	SeqList(SeqList<T>& L);							// Copy constructor
	SeqList<T>& operator=(SeqList<T>& L);			// Operator= overloading
	virtual ~SeqList() { delete[] data; }			// Virtual destructor
	T* getPtr2data() { return data; }				// New function in derived class SeqList to get the pointer to data(i.e. &data[0]). Use it carfully!
	void resize(int newsz);							// New function in derived class SeqList to change the size(maximum volume) of the list	
	virtual int size()const { return maxSize; }		// Get the maximum volume of the list
	virtual int length()const { return last + 1; }	// Get the number of pactical existing items
	virtual int search(const T& x)const;			// Search specified item x and return its logical sequence number 
	virtual bool insert(int i, const T& x);			// Insert value x after the i-th item, 0<=i<=index_of_last_one_in_logical
	virtual bool append(const T& x);			 	// Add value x at the end of list
	virtual bool remove(int i, T& x);				// Remove the i-th item & store the removed value
	virtual bool remove(int i);						// Remove the i-th item without storing the removed value	
	virtual void Union(SeqList<T>& L2);				// Union of two lists, and store the result in *this
	virtual void Intersection(SeqList<T>& L2);		// Intersection of two lists, and store the result in *this
	virtual void input();							// Input data via the console window
	virtual void Import(const string& filename);	// Import corresponding data from a local host file
	virtual void output()const;						// Output data via the console window
	virtual void Export(const string& filename)const;// Export corresponding data into a local host file
	virtual bool isEmpty()const { return last == -1 ? true : false; }
	virtual bool isFull()const { return last + 1 == maxSize ? true : false; }
	virtual void sort() {/* Do a sort in a specific manner which hinges on the data type T. */ }
	virtual T& getVal(int i)const {
		// Get the i-th item value & return its reference
		if (i > 0 && i <= last + 1)
			return data[i - 1];		
		else{
			cerr << "Error! Invalid i, index i must range from 1 to " << last + 1 << endl;
			//exit(1);
		}
	}
	virtual bool getVal(int i, T& x)const {
		// Get the i-th item value via reference x, and return true if succeed & return false otherwise
		if (i > 0 && i <= last + 1) {
			x = data[i - 1];
			return true;
		}
		else
			return false;
	}
	virtual void setVal(int i, const T& x) {
		// Set value x for the i-th item	
		if (i > 0 && i <= last + 1)
			data[i - 1] = x;
		else
			cerr << "Error! Invalid i, index i must range from 1 to " << last + 1 << endl;
	}
private:
	T* data;
	int maxSize;									// The maximum volume of the list
	int last;									    // The physical index number(starting from 0) of the last existing item
	
};

template<typename T>
SeqList<T>::SeqList(int sz) {
	if (sz > 0) {
		maxSize = sz;
		last = -1;									// set practical length of the list null
		data = new T[maxSize];
		if (data == nullptr) { cerr << "Memory allocation error!" << endl; exit(1); }
	}
}

template<typename T>
SeqList<T>::SeqList(SeqList<T>& L) {
	maxSize = L.maxSize;
	last = L.length()-1;
	data = new T[maxSize];
	if (data == nullptr) { cerr << "Memory allocation error!" << endl; exit(1); }	
	int n = last + 1;
	T *ptrsrc = L.data, *ptrdes = data;
	// copy each item of list L	to new array
	while (n--) *ptrdes++ = *ptrsrc++;
}

template<typename T>
SeqList<T>& SeqList<T>::operator=(SeqList<T>& L)
{
	if (&L != this) {
		T* data0 = data;
		last = L.length() - 1;
		maxSize = L.size();

		data = new T[maxSize];
		if (data == nullptr) { cerr << "Memory allocation error!" << endl; exit(1); }
		int n = last + 1;
		T *ptrsrc = L.data, *ptrdes = data;
		// copy each item of list L	to new array
		while (n--) *ptrdes++ = *ptrsrc++;

		delete[] data0;
	}
	return *this;
}

template<typename T>
int SeqList<T>::search(const T& x)const {
	for (int i = 0; i <= last; ++i)
		if (data[i] == x)
			return i + 1;
	return 0;										// not found
}

template<typename T>
void SeqList<T>::resize(int newsz) {
	if (newsz <= 0) { cerr << "Invalid newsz! newsz must be a positive integer." << endl; return; }
	if (newsz != maxSize) {
		if (newsz < last + 1) {
			cout << "Warning, newsz is smaller than the practical length of the list. Data lose may occur.\n";
			cout << "Are you sure to go on?('y' or 'n')\n";
			char c;
			// clear input buffer
			while ((c = getchar()) != '\n');

			cin >> c;
			if (c != 'y' && c != 'Y') 
				return;
			// else execute following instructions
		}
		maxSize = newsz;
		T* newdata = new T[maxSize];
		if (newdata == nullptr) { cerr << "Memory allocation error!" << endl; exit(1); }
		int n;
		if (newsz >= last + 1) n = last + 1;			
		else { n = newsz; last = newsz - 1; }			

		T *ptrsrc = data, *ptrdes = newdata;
		// copy existing items to new array
		while (n--) *ptrdes++ = *ptrsrc++;

		delete[] data;
		data = newdata;
	}
}

template<typename T>
bool SeqList<T>::insert(int i, const T& x) {
	/* Insert a new element x after the i-th item, 0<=i<=last+1. 
	   In particular, i=0, insert x at the beginning position. */ 
	if (last == maxSize - 1) {
		cerr << "List is full, cannot insert!\n";
		return false;			
	}
	if (i<0 || i>last + 1) {
		cerr << "Invalid index i, i must be satisfy 0<=i<=" << last + 1 << endl;
		return false;
	}

	for (int j = last; j >= i; --j) 
		// move backward respectively
		data[j + 1] = data[j];

	data[i] = x;
	++last;
	return true;
}

template<typename T>
bool SeqList<T>::append(const T& x) {
	// add a new element x at the end of the list
	if (last == maxSize - 1) {
		cerr << "List's full, failed to insert!\n";
		return false;
	}
	// self-increase of "last" & insert x
	data[++last] = x;
	return true;
}

template<typename T>
bool SeqList<T>::remove(int i, T& x) {
	// Remove the i-th item & store the removed value
	if (last == -1) {
		cerr << "List is null, cannot remove!" << endl;
		return false;
	}
	if (i<1 || i>last + 1) {
		cerr << "Invalid index i, i must be satisfy 1<=i<=" << last + 1 << endl;
		return false;
	}
	// store the item that is going to be romoved
	x = data[i-1];

	for (int j = i; j <= last; ++j)
		// move forward respectively
		data[j - 1] = data[j];

	--last;
	return true;
}

template<typename T>
bool SeqList<T>::remove(int i) {
	// Remove the i-th item without storing it
	if (last == -1) {
		cerr << "List is null, cannot remove!" << endl;
		return false;
	}
	if (i<1 || i>last + 1) {
		cerr << "Invalid index i, i must be satisfy 1<=i<=" << last + 1 << endl;
		return false;
	}
	
	for (int j = i; j <= last; ++j)
		// move forward respectively
		data[j - 1] = data[j];

	--last;
	return true;
}

template<typename T>
void SeqList<T>::Union(SeqList<T>& L2) {
	int m = L2.length();
	T x;
	for (int i = 1; i <= m; ++i) {
		L2.getVal(i, x);
		if (!search(x))									// not found x in this list
			append(x);
	}
}

template<typename T>
void SeqList<T>::Intersection(SeqList<T>& L2) {
	int n = last + 1, i = 1;
	T x;
	while (i <= n) {
		getVal(i, x);
		if (!L2.search(x)) {
			// not found x in list L2
			remove(i);
			--n;
		}
		else
			++i;
	}
}

template<typename T>
void SeqList<T>::input() {
	if (last != -1) {
		cout << "Warning, the list is not null. Input new data will cover the original data\n";
		cout << "Are you sure to go on?('y' or 'n')\n";
		char c;
		// clear input buffer
		while ((c = getchar()) != '\n');

		cin >> c;
		if (c != 'y' && c != 'Y')
			return;
		// else execute following instructions
	}
	cout << "Please input data.  (Attention: to end up with Ctrl+Z)" << endl;
	int i = 0;
	last = -1;
	while (cin >> data[i++]) {
		++last;
		if (last == maxSize - 1) {
			cout << "The list is full." << endl; 
			break;
		}
	}
	cout << "You have input " << last + 1 << " data item(s)." << endl;	
}


template<typename T>
void SeqList<T>::Import(const string& filename) {
	if (last != -1) {
		cout << "Warning, the list is not null. Input new data will cover the original data\n";
		cout << "Are you sure to go on?('y' or 'n')\n";
		char c;
		// clear input buffer
		while ((c = getchar()) != '\n');

		cin >> c;
		if (c != 'y' && c != 'Y')
			return;
		// else execute following instructions
	}
	ifstream is(filename, ios::in | ios::binary);
	if (!is) {
		cerr << "Open file \"" << filename << "\" error! Can't find this file.\n";
		cout<<"Please check the validity of its directory or filename." << endl;
		exit(1);
	}
	int i = 0;
	last = -1;
	while (is.read((char*)(data + i++), sizeof(T))) {
		++last;
		if (last == maxSize - 1) {
			cout << "The list is full." << endl;
			break;
		}
	}
	is.close();
	cout << "You have imported " << last + 1 << " data item(s) from file \"" << filename << "\"." << endl;
}

template<typename T>
void SeqList<T>::output()const {
	for (int i = 0; i <= last; ++i)
		cout << "#" << i + 1 << ": " << data[i] << endl;
}


template<typename T>
void SeqList<T>::Export(const string& filename)const {
	ofstream os(filename, ios::out | ios::binary | ios::_Noreplace);
	if (!os) {
		cerr << "Open file error! File \"" << filename << "\" has already existed." << endl;
		exit(1);
	}
	
	for (int i = 0; i <= last; ++i)
		os.write((char*)(data + i), sizeof(T));
	os.close();
}

#endif // !SEQLIST_H

