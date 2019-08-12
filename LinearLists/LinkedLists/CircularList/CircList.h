// circular singly linked list(with head node) header
#pragma once
#ifndef CIRCLIST_H
#define CIRCLIST_H

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include"LinearList.h"

template<typename T>
struct CLLNode {
	// Circular Linked List Node structure
	T data;
	CLLNode<T>* next;
	CLLNode(CLLNode<T>* ptr = nullptr) { next = ptr; }
	CLLNode(const T& val, CLLNode<T>* ptr = nullptr) { data = val; next = ptr; }
};

template<typename T>
class CircList : public LinearList<T> {
public:
	CircList();										// constructor with no infomation in data zone of head node
	CircList(const T& Info_for_head_node);			// constructor, & store info in head node
	CircList(CircList<T>& L); 						// copy constructor
	CircList<T>& operator=(CircList<T>& L);			// assignment operator= overloading	
	virtual ~CircList() { clear(); delete head; }	// virtual destructor		
	virtual int size()const { return length(); }	// get the size of the list
	virtual int length()const;						// get the number of pactical existing items	
	virtual int search(const T& x)const;			// search specified item x and return its logical sequence number 	
	virtual bool insert(int i, const T& x);			// insert value x after the i-th item, 0<=i<=index_of_last_one_in_logical
	virtual bool append(const T& x);			 	// add value x at the end of list
	virtual bool remove(int i, T& x);				// remove the i-th item & store the removed value
	virtual bool remove(int i);						// remove the i-th item without storing the removed value	
	virtual void Union(CircList<T>& L2);			// union of two lists, and store the result in *this
	virtual void Intersection(CircList<T>& L2);		// intersection of two lists, and store the result in *this
	virtual void input();							// input data via the console window
	virtual void output()const;						// output data via the console window	
	virtual bool isFull()const { return false; }	// function derived from base class LinearList, no practical meaning in linked list
	virtual bool isEmpty()const { return head->next == head ? true : false; }
	virtual void sort() {/* do a sort in a specific manner which hinges on the data type T. */ }
	virtual T& getVal(int i)const;
	virtual bool getVal(int i, T& x)const;
	virtual void setVal(int i, const T& x);
	virtual void Import(const std::string& filename, const std::string& mode_selection_text_or_binary);	    // Read corresponding data from a local host file
	virtual void Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const;// Write corresponding data into a local host file	
	// new functions in derived class CircList
	void clear();									// erase all
	CLLNode<T>* getHead()const { return head; }		// get the pointer to head node
	void setHead(CLLNode<T>* p);					// make head pointer point to some node that is also pointed by p
	CLLNode<T>* locate(int i)const;					// locate the i-th item	and return the pointer to this node	
	CLLNode<T>* find(const T& x)const;				// find specified item x and return the pointer to this node
	T& operator[](int index);						// access list items via []
private:
	CLLNode<T>* head;								// pointer to the head node of the list 
	CLLNode<T>* tail;								// pointer to the last node of the list
};


template<typename T>
CircList<T>::CircList() { 
	// constructor with no infomation in data zone of head node
	head = new CLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	head->next = head;
}

template<typename T>
CircList<T>::CircList(const T& Info_for_head_node) {
	// constructor, & store some info in head node
	head = new CLLNode<T>(Info_for_head_node);
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	head->next = head;
}

template<typename T>
CircList<T>::CircList(CircList<T>& L) {
	// copy constructor
	CLLNode<T>* srcptr = L.getHead();
	CLLNode<T>* desptr = head = new CLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != L.head) {
		// copy data from L & construct dynamic linked list
		desptr->next = new CLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	// record tail pointer & make it point to head node
	tail = desptr;
	tail->next = head;
}

template<typename T>
CircList<T>& CircList<T>::operator=(CircList<T>& L) {
	// assignment operator= overloading
	CLLNode<T>* srcptr = L.getHead();
	CLLNode<T>* desptr = head = new CLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != L.head) {
		// copy data from L & construct dynamic linked list
		desptr->next = new CLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	// record tail pointer & make it point to head node
	tail = desptr;
	tail->next = head;
	return *this;
}

template<typename T>
T& CircList<T>::operator[](int index) {
	// access list items via [], 0<=index<=length(), equivalent to getVal(int i)
	// i=0, return head node info(may be null)
	if (index < 0) { std::cerr << "Using operator[] error! Reason: Invalid argument index, index must be no less than 0." << std::endl; exit(1); }
	CLLNode<T>* curr = head;
	while (index--) {
		curr = curr->next;
		if (curr == head) {
			std::cerr << "Using operator[] error! Reason: Argument index exceeded the list's length." << std::endl;
			exit(1);
		}
	}
	return curr->data;
}

template<typename T>
void CircList<T>::setHead(CLLNode<T>* p) {
	// make head pointer point to some node that is also pointed by p
	head = tail = p;
	// make tail pointer point to the changed last node	
	while (tail->next != head)
		tail = tail->next;
	// after the loop, we have: tail->next = head
}

template<typename T>
void CircList<T>::clear() {
	// erase all nodes but head node
	CLLNode<T>* curr = head->next, *del;
	head->next = tail = head;
	while (curr != head) {
		del = curr;
		curr = curr->next;
		delete del;
	}
}

template<typename T>
int CircList<T>::length()const {
	// get the list's length 
	CLLNode<T>* curr = head->next;
	int count = 0;
	while (curr != head) {
		curr = curr->next;
		++count;
	}
	return count;
}

template<typename T>
CLLNode<T>* CircList<T>::locate(int i)const {
	/* locate the i-th node & return its pointer, i>=0
	 * In particular,	 i=0,	  return head(i.e. the pointer to head node)
	 *				  i>length(), i := i-length()-1, then go on and on like this
	 *---------------------------------------------------------------------------------------
	 * Still, something's worth noting: If i is too large, it will go on loop again and again
	 * until 0<=i<=length(), which does effortless work & consumes a little bit CPU resource
	 * and consequently decrease the program's performance. Even so it can be acceptable if i 
	 * isn't that ridiculous.
	 *---------------------------------------------------------------------------------------
	**/
	if (i < 0) { std::cerr << "Locating error! Reason: Invalid argument i, i must be no less than 0." << std::endl; exit(1); }
	CLLNode<T>* curr = head;
	while (i--)
		curr = curr->next;
	return curr;
}

template<typename T>
CLLNode<T>* CircList<T>::find(const T& x)const {
	// find val x in the list & return its pointer. If cannot find return nullptr
	CLLNode<T>* curr = head->next;
	while (curr != head) {
		if (curr->data == x) {
			break;
			return curr;
		}			
		else
			curr = curr->next;
	}
	return nullptr;
}

template<typename T>
int CircList<T>::search(const T& x)const {
	// search val x in the list & return its index. If cannot find return 0
	CLLNode<T>* curr = head->next;
	int index = 1;
	while (curr != head) {
		if (curr->data == x) return index;
		else { curr = curr->next; ++index; }
	}
	return 0;										// not found
}

template<typename T>
T& CircList<T>::getVal(int i)const {
	// return the value of i-th item, i>=0.  i=0,     return head node info(may be null)
    //								      i>length(), i:=i-length()-1, go on and on like this
	return locate(i)->data;
}

template<typename T>
bool CircList<T>::getVal(int i, T& x)const {
	// assign the value of i-th item to x, i>=0.
	x = locate(i)->data;
	return true;
}

template<typename T>
void CircList<T>::setVal(int i, const T& x) {
	// set i-th item the value x. If i>length(), i:=i-length()-1, go on and on like this
	locate(i)->data = x;
}

template<typename T>
bool CircList<T>::insert(int i, const T& x) {
	/* Insert a new element x after the i-th node, i>=0
	 * In particular, i=0, insert x after head node.
	**/
	CLLNode<T>* curr = locate(i), *newNode = new CLLNode<T>(x);
	if (newNode == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
 
	newNode->next = curr->next;
	curr->next = newNode;
	// if it is inserted after tail node, we need to update tail pointer 	
	if (curr == tail)
		tail = tail->next;

	return true;
}

template<typename T>
bool CircList<T>::append(const T& x) {
	// add a new element x after the last node	
	CLLNode<T>* newNode = new CLLNode<T>(x);
	if (newNode == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

	newNode->next = head;
	tail->next = newNode;
	// update tail pointer
	tail = tail->next;

	return true;
}

template<typename T>
bool CircList<T>::remove(int i, T& x) {
	// remove the i-th node & store the value to be removed
	if (i < 1) { std::cout << "Deletion error! Reason: Invalid argument i, i must be no less than 1." << std::endl; exit(1); }
	CLLNode<T>* delpre = locate(i - 1), *del = delpre->next;
	if (del == head) {
		std::cout << "Deletion error! Cannot delete head node." << std::endl;
		return false;
	}

	if (del == tail)
		tail = delpre;

	delpre->next = del->next;
	x = del->data;
	delete del;

	return true;
}

template<typename T>
bool CircList<T>::remove(int i) {
	// remove the i-th node without storing it
	if (i < 1) { std::cout << "Deletion error! Reason: Invalid argument i, i must be no less than 1." << std::endl; exit(1); }
	CLLNode<T>* delpre = locate(i - 1), *del = delpre->next;
	if (del == head) {
		std::cout << "Deletion error! Cannot delete head node." << std::endl;
		return false;
	}

	if (del == tail)
		tail = delpre;

	delpre->next = del->next;
	delete del;

	return true;
}

template<typename T>
void CircList<T>::Union(CircList<T>& L2) {
	// union of two lists & store the result in *this
	CLLNode<T>* curr = L2.head->next;
	T x;
	while (curr != L2.head) {
		x = curr->data;
		if (!search(x))								// not found x in this list
			append(x);
		curr = curr->next;
	}
}

template<typename T>
void CircList<T>::Intersection(CircList<T>& L2) {
	// intersection of two lists & store the result in *this
	CLLNode<T>* curr = head->next, *prev = head;
	T x;
	while (curr != head) {
		x = curr->data;
		if (!L2.search(x)) {						// not found x in list L2			
			// erase x in this list
			prev->next = curr->next;
			CLLNode<T>* del = curr;
			// if the node to be deleted is tail node, move tail pointer to previous node 
			if (del == tail)
				tail = prev;

			curr = curr->next;
			delete del;
		}
		else {										// retain nodes with identical data
			curr = curr->next;
			prev = prev->next;
		}
	}
}

template<typename T>
void CircList<T>::input() {
	if (head->next != head) {						// this list has at least one node
		std::cout << "Warning, the list is not null. Input new data will cover the original data\n";
		std::cout << "Are you sure to go on?('y' or 'n')\n";
		char c;
		// clear stdin buffer to avoid cin extracting '\n' for c
		std::cin.clear();
		std::cin.sync();

		std::cin >> c;
		if (c != 'y' && c != 'Y') 					// cancel
			return;
		// else execute following instructions
	}
	clear();										// erase all existing nodes
	CLLNode<T>* curr = head;
	T tmp;
	std::cout << "Please input data. (end up with Ctrl+Z)" << std::endl;
	std::cin.clear();								// reset the iostate of cin to good
	while (std::cin >> tmp) {
		curr->next = new CLLNode<T>(tmp);
		if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		curr = curr->next;
	}
	// after the loop, curr points to the last node	
	tail = curr;
	tail->next = head;
}

template<typename T>
void CircList<T>::output()const {
	CLLNode<T>* curr = head->next;
	int i = 1;
	while (curr != head) {
		std::cout << "#";	
		std::cout.setf(std::ios::left);
		std::cout.width(4);							// modify here for aesthetic according to practical lenth of the list
		std::cout << i++ << ": " << curr->data << '\n';
		curr = curr->next;
	}
}

template<typename T>
void CircList<T>::Import(const std::string& filename, const std::string& mode_selection_text_or_binary) {
	if (head->next != head) {						// this list has at least one node
		std::cout << "Warning, the list is not null. Input new data will cover the original data\n";
		std::cout << "Are you sure to go on?('y' or 'n')\n";
		char c;
		// clear stdin buffer to avoid cin extracting '\n' for c
		std::cin.clear();
		std::cin.sync();

		std::cin >> c;
		if (c != 'y' && c != 'Y')					// cancel
			return;
		// else execute following instructions
	}
	clear();										// erase all existing nodes
	// match import mode
	if (mode_selection_text_or_binary == "text") {
		// read in ASCII text format

		// open file
		std::ifstream ifs(filename, ios_base::in);
		if (!ifs) {
			std::cerr << "Error in opening file for reading! Can't find file \"" << filename << "\".\n"
				<< "Please check the validity of its directory or filename." << std::endl;
			exit(1);
		}

		// read data
		T tmp; CLLNode<T>* curr = head;
		while (ifs >> tmp) {
			curr->next = new CLLNode<T>(tmp);
			if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
			curr = curr->next;
		}
		tail = curr;
		tail->next = head;

		// roughly tell if read correctly & error handling
		if (ifs.eof())								// finish reading the file(successfully)
			ifs.clear();							// reset the iostate of ifs to good
		else {										// file readed may not match with the data type
			std::cerr << "Error in reading file " << "\"" << filename << "\"!\n"
				<< "The file you're trying to read may not match the data type." << std::endl;
			exit(1);
		}

		ifs.close();
	}
	else
		if (mode_selection_text_or_binary == "binary") {
			// read in binary format

			// open file
			std::ifstream ifs(filename, ios_base::in | ios_base::binary);
			if (!ifs) {
				std::cerr << "Error in opening file for reading! Can't find file \"" << filename << "\".\n"
					<< "Please check the validity of its directory or filename." << std::endl;
				exit(1);
			}

			// read data
			T tmp; CLLNode<T>* curr = head;
			while (ifs.read((char*)&tmp, sizeof(tmp))) {
				curr->next = new CLLNode<T>(tmp);
				if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
				curr = curr->next;
			}
			tail = curr;
			tail->next = head;

			// roughly tell if read correctly & error handling
			if (ifs.eof())							// finish reading the file(successfully)
				ifs.clear();						// reset the iostate of ifs to good
			else {									// file readed may not match with the data type
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
}

template<typename T>
void CircList<T>::Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const {
	// match export mode
	if (mode_selection_text_or_binary == "text") {
		// write in ASCII text format

		// open file
		std::ofstream ofs(filename, ios_base::out | std::ios::_Noreplace);
		if (!ofs) {
			std::cerr << "Error in opening file for writing! File \"" << filename << "\" has already existed." << std::endl;
			exit(1);
		}

		// write data
		CLLNode<T>* curr = head->next;
		while (curr != head) {
			ofs << curr->data << '\n';
			curr = curr->next;
		}

		ofs.close();
	}
	else
		if (mode_selection_text_or_binary == "binary") {
			// write in binary format

			// open file
			std::ofstream ofs(filename, ios_base::out | ios_base::binary | std::ios::_Noreplace);
			if (!ofs) {
				std::cerr << "Error in opening file for writing! File \"" << filename << "\" has already existed." << std::endl;
				exit(1);
			}

			// write data
			CLLNode<T>* curr = head->next;
			while (curr != head) {
				ofs.write((char*) &(curr->data), sizeof(curr->data));
				curr = curr->next;
			}

			ofs.close();
		}
		// type argment 2 wrongly
		else {
			std::cerr << "Mode choosing error! It must be either \"text\" or \"binary\" mode." << std::endl;
			exit(1);
		}
}

#endif // !CIRCLIST_H

