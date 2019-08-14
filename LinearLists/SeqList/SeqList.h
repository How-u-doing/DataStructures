// sequential list header
#pragma once
#ifndef SEQLIST_H
#define SEQLIST_H

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include"LinearList.h"

const int defaultSize = 100;						// Default size of list, you can modify it here

template<typename T>
class SeqList : public LinearList<T> {
public:	
	SeqList(int sz=defaultSize);					// Constructor	
	SeqList(const SeqList<T>& L);					// Copy constructor
	SeqList<T>& operator=(const SeqList<T>& L);		// Operator= overloading
	T& operator[](int index) { return getVal(index); }
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
	virtual void Union(const SeqList<T>& L2);		// Union of two lists, and store the result in *this
	virtual void Intersection(const SeqList<T>& L2);// Intersection of two lists, and store the result in *this
	virtual void input();							// Input data via the console window	
	virtual void output()const;						// Output data via the console window
	virtual void Import(const std::string& filename, const std::string& mode_selection_text_or_binary);	    // Read corresponding data from a local host file
	virtual void Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const;// Write corresponding data into a local host file	
	virtual bool isEmpty()const { return last == -1 ? true : false; }
	virtual bool isFull()const { return last + 1 == maxSize ? true : false; }
	virtual void sort() {/* Do a sort in a specific manner which hinges on the data type T. */ }
	virtual T& getVal(int i)const {
		// Get the i-th item value & return its reference
		if (i > 0 && i <= last + 1)
			return data[i - 1];		
		else{
			std::cerr << "Error! Invalid i, index i must range from 1 to " << last + 1 << std::endl;
			exit(1);
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
			std::cerr << "Error! Invalid i, index i must range from 1 to " << last + 1 << std::endl;
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
		if (data == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	}
}

template<typename T>
SeqList<T>::SeqList(const SeqList<T>& L) {
	maxSize = L.maxSize;
	last = L.length()-1;
	data = new T[maxSize];
	if (data == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	int n = last + 1;
	T *ptrsrc = L.data, *ptrdes = data;
	// copy each item of list L	to new array
	while (n--) *ptrdes++ = *ptrsrc++;
}

template<typename T>
SeqList<T>& SeqList<T>::operator=(const SeqList<T>& L)
{
	if (&L != this) {
		T* data0 = data;
		last = L.length() - 1;
		maxSize = L.size();

		data = new T[maxSize];
		if (data == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
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
	if (newsz <= 0) { std::cerr << "Invalid newsz! newsz must be a positive integer." << std::endl; return; }
	if (newsz != maxSize) {
		if (newsz < last + 1) {
			std::cout << "Warning, newsz is smaller than the practical length of the list. Data lose may occur.\n";
			std::cout << "Are you sure to go on?('y' or 'n')\n";
			char c;
			// clear stdin buffer to avoid cin extracting '\n' for c
			std::cin.clear();
			std::cin.sync();

			std::cin >> c;
			if (c != 'y' && c != 'Y') 
				return;
			// else execute following instructions
		}
		maxSize = newsz;
		T* newdata = new T[maxSize];
		if (newdata == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
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
		std::cerr << "List is full, cannot insert!\n";
		return false;			
	}
	if (i<0 || i>last + 1) {
		std::cerr << "Invalid index i, i must be satisfy 0<=i<=" << last + 1 << std::endl;
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
		std::cerr << "List's full, failed to insert!\n";
		return false;
	}
	// self-increase of "last" & insert x
	data[++last] = x;
	return true;
}

template<typename T>
bool SeqList<T>::remove(int i, T& x) {
	// Remove the i-th item & store the value to be removed
	if (last == -1) {
		std::cerr << "List is null, cannot remove!" << std::endl;
		return false;
	}
	if (i<1 || i>last + 1) {
		std::cerr << "Invalid index i, i must be satisfy 1<=i<=" << last + 1 << std::endl;
		return false;
	}
	// store the item that is going to be removed
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
		std::cerr << "List is null, cannot remove!" << std::endl;
		return false;
	}
	if (i<1 || i>last + 1) {
		std::cerr << "Invalid index i, i must be satisfy 1<=i<=" << last + 1 << std::endl;
		return false;
	}
	
	for (int j = i; j <= last; ++j)
		// move forward respectively
		data[j - 1] = data[j];

	--last;
	return true;
}

template<typename T>
void SeqList<T>::Union(const SeqList<T>& L2) {
	int m = L2.length();
	T x;
	for (int i = 1; i <= m; ++i) {
		L2.getVal(i, x);
		if (!search(x))									// not found x in this list
			append(x);
	}
}

template<typename T>
void SeqList<T>::Intersection(const SeqList<T>& L2) {
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
		std::cout << "Warning, the list is not null. Input new data will cover the original data\n";
		std::cout << "Are you sure to go on?('y' or 'n')\n";
		char c;
		// clear stdin buffer to avoid cin extracting '\n' for c
		std::cin.clear();
		std::cin.sync();

		std::cin >> c;
		if (c != 'y' && c != 'Y')
			return;
		// else execute following instructions
	}
	std::cout << "Please input data.  (Attention: to end up with Ctrl+Z)" << std::endl;
	int i = 0;
	last = -1;
	while (std::cin >> data[i++]) {
		++last;
		if (last == maxSize - 1) {
			std::cout << "The list is full." << std::endl;
			break;
		}
	}
	std::cin.clear();								// reset the iostate of cin to good
	std::cout << "You have input " << last + 1 << " data item(s)." << std::endl;
}

template<typename T>
void SeqList<T>::output()const {
	for (int i = 0; i <= last; ++i) {
		// modify here for aesthetic according to practical lenth of the list
		printf("#%4d:", i+1);
		std::cout << data[i] << std::endl;
	}		
}

template<typename T>
void SeqList<T>::Import(const std::string& filename, const std::string& mode_selection_text_or_binary) {
	if (last != -1) {
		std::cout << "Warning, the list is not null. Input new data will cover the original data\n";
		std::cout << "Are you sure to go on?('y' or 'n')\n";
		char c;
		// clear stdin buffer to avoid cin extracting '\n' for c
		std::cin.clear();
		std::cin.sync();

		std::cin >> c;
		if (c != 'y' && c != 'Y')
			return;
		// else execute following instructions
	}
	// match import mode
	if (mode_selection_text_or_binary == "text") {
		// read in ASCII text form

		// open file
		std::ifstream ifs(filename, ios_base::in);
		if (!ifs) {
			std::cerr << "Error in opening file for reading! Can't find file \"" << filename << "\".\n"
					  << "Please check the validity of its directory or filename." << std::endl;
			exit(1);
		}

		// read data		
		int i = 0;
		last = -1;
		while (ifs >> *(data + i++)) {
			++last;
			if (last == maxSize - 1) {
				std::cout << "The list is full." << std::endl;
				break;
			}
		}

		// roughly tell if read correctly & error handling
		if (ifs.eof())									// finish reading the file(successfully)
			ifs.clear();								// reset the iostate of ifs to good
		else {											// file readed may not match with the data type
			std::cerr << "Error in reading file " << "\"" << filename << "\"!\n"
				<< "The file you're trying to read may not match the data type." << std::endl;
			exit(1);
		}

		ifs.close();
	}
	else
		if (mode_selection_text_or_binary == "binary") {
			// read in binary form

			// open file
			std::ifstream ifs(filename, ios_base::in | ios_base::binary);
			if (!ifs) {
				std::cerr << "Error in opening file for reading! Can't find file \"" << filename << "\".\n"
						  << "Please check the validity of its directory or filename." << std::endl;
				exit(1);
			}

			// read data
			int i = 0;
			last = -1;
			while (ifs.read((char*)(data + i++), sizeof(T))) {
				++last;
				if (last == maxSize - 1) {
					std::cout << "The list is full." << std::endl;
					break;
				}
			}			
			
			// roughly tell if read correctly & error handling
			if (ifs.eof())									// finish reading the file(successfully)
				ifs.clear();								// reset the iostate of ifs to good
			else {											// file readed may not match with the data type
				std::cerr << "Error in reading file " << "\"" << filename << "\"!\n"
					<< "The file you're trying to read may not match the data type." << std::endl;
				exit(1);
			}

			ifs.close();
		}
		// type argment 2 wrongly
		else {
			std::cerr << "Mode choosing error! It must be either \"text\" or \"binary\" mode." << std::endl;
			exit(1);
		}

	std::cout << "You have imported " << last + 1 << " data item(s) from file \"" << filename << "\".\n\n";
}

template<typename T>
void SeqList<T>::Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const {
	// match export mode
	if (mode_selection_text_or_binary == "text") {
		// write in ASCII text form

		// open file
		std::ofstream ofs(filename, ios_base::out | std::ios::_Noreplace);
		if (!ofs) {
			std::cerr << "Error in opening file for writing! File \"" << filename << "\" has already existed." << std::endl;
			exit(1);
		}

		// write data
		for (int i = 0; i <= last; ++i)
			ofs << *(data + i) << '\n';

		ofs.close();
	}
	else
		if (mode_selection_text_or_binary == "binary") {
			// write in binary form

			// open file
			std::ofstream ofs(filename, ios_base::out | ios_base::binary | std::ios::_Noreplace);
			if (!ofs) {
				std::cerr << "Error in opening file for writing! File \"" << filename << "\" has already existed." << std::endl;
				exit(1);
			}

			// write data
			for (int i = 0; i <= last; ++i)
				ofs.write((char*)(data + i), sizeof(T));

			ofs.close();
		}
		// type argment 2 wrongly
		else {
			std::cerr << "Mode choosing error! It must be either \"text\" or \"binary\" mode." << std::endl;
			exit(1);
		}
}

#endif // !SEQLIST_H

