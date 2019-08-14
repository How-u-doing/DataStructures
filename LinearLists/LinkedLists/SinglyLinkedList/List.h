// singly linked list(with head node) header
#pragma once
#ifndef LIST_H
#define LIST_H

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include"LinearList.h"

template<typename T>
struct Node {
	T data;
	Node<T>* next;
	Node(Node<T>* ptr = nullptr) { next = ptr; }
	Node(const T& val, Node<T>* ptr = nullptr) { data = val; next = ptr; }
};

template<typename T>
class List: public LinearList<T> {
public:	
	List() { head = new Node<T>; }					// constructor with no infomation in data zone of head node
	List(const T& Info_for_head_node) { head = new Node<T>(Info_for_head_node); }	// constructor, & store info in head node
	List(const List<T>& L); 						// copy constructor
	List<T>& operator=(const List<T>& L);			// assignment operator= overloading	
	virtual ~List() { clear(); delete head; }		// virtual destructor		
	virtual int size()const { return length(); }	// get the size of the list
	virtual int length()const;						// get the number of pactical existing items	
	virtual int search(const T& x)const;			// search specified item x and return its logical sequence number 	
	virtual bool insert(int i, const T& x);			// insert value x after the i-th item, 0<=i<=index_of_last_one_in_logical
	virtual bool append(const T& x);			 	// add value x at the end of list
	virtual bool remove(int i, T& x);				// remove the i-th item & store the removed value
	virtual bool remove(int i);						// remove the i-th item without storing the removed value	
	virtual void Union(List<T>& L2);				// union of two lists, and store the result in *this
	virtual void Intersection(List<T>& L2);			// intersection of two lists, and store the result in *this
	virtual void input();							// input data via the console window
	virtual void output()const;						// output data via the console window	
	virtual bool isFull()const { return false; }	// function derived from base class LinearList, no practical meaning in linked list
	virtual bool isEmpty()const { return head->next == nullptr ? true : false; }
	virtual void sort() {/* do a sort in a specific manner which hinges on the data type T. */ }
	virtual T& getVal(int i)const;
	virtual bool getVal(int i, T& x)const;
	virtual void setVal(int i, const T& x);
	virtual void Import(const std::string& filename, const std::string& mode_selection_text_or_binary);	    // Read corresponding data from a local host file
	virtual void Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const;// Write corresponding data into a local host file	
	// new functions in derived class List
	void clear();									// erase all
	Node<T>* getHead()const { return head; }		// get the pointer to head node
	Node<T>* locate(int i)const;					// locate the i-th item	and return the pointer to this node	
	Node<T>* find(const T& x)const;					// find specified item x and return the pointer to this node
	T& operator[](int index);						// access list items via []
private:
	Node<T>* head;									// pointer to the head node of the list 	
};


template<typename T>
List<T>::List(const List<T>& L) {
	// copy constructor
	Node<T>* srcptr = L.getHead();
	Node<T>* desptr = head = new Node<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != nullptr) {
		// copy data from L & construct dynamic linked list
		desptr->next = new Node<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& L) {
	// assignment operator= overloading
	if (head->next != nullptr) clear();				// erase all nodes but head node if list is not null
	Node<T>* srcptr = L.getHead();
	Node<T>* desptr = head = new Node<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != nullptr) {
		// copy data from L & construct dynamic linked list
		desptr->next = new Node<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	return *this;
}

template<typename T>
T& List<T>::operator[](int index) {	
	// access list items via [], 0<=index<=length(), equivalent to getVal(int i)
	// i=0, return head node info(may be null)
	if (index < 0) { std::cerr << "Using operator[] error! Reason: Invalid argument index, index must be no less than 0." << std::endl; exit(1); }
	Node<T>* curr = head;
	while (index--) {
		curr = curr->next;
		if (curr == nullptr) {
			std::cerr << "Using operator[] error! Reason: Argument index exceeded the list's length." << std::endl;
			exit(1);
		}
	}
	return curr->data;
}						

template<typename T>
void List<T>::clear() {
	// erase all nodes but head node
	Node<T>* curr = head->next, *del;
	head->next = nullptr;
	while (curr != nullptr) {
		del = curr;
		curr = curr->next;
		delete del;
	}
}

template<typename T>
int List<T>::length()const {
	// get the list's length 
	Node<T>* curr = head->next;
	int count = 0;
	while (curr != nullptr) {
		curr = curr->next;
		++count;
	}
	return count;
}

template<typename T>
Node<T>* List<T>::locate(int i)const {
	/* locate the i-th node & return its pointer, 0<=i<=length()
	 * In particular,	 i=0,	  return head(i.e. the pointer to head node)
	 *				  i>length(), return nullptr
	**/
	if (i < 0) { std::cerr << "Locating error! Reason: Invalid argument i, i must be no less than 0." << std::endl; exit(1); }
	Node<T>* curr = head;
	while (i-- && curr != nullptr)
		curr = curr->next;
	return curr;
}

template<typename T>
Node<T>* List<T>::find(const T& x)const {
	// find val x in the list & return its pointer. If cannot find return nullptr
	Node<T>* curr = head->next;
	while (curr != nullptr) {
		if (curr->data == x)
			break;
		else
			curr = curr->next;
	}
	return curr;
}

template<typename T>
int List<T>::search(const T& x)const {
	// search val x in the list & return its index. If cannot find return 0
	Node<T>* curr = head->next;
	int index = 1;
	while (curr != nullptr) {
		if (curr->data == x) return index;			
		else { curr = curr->next; ++index; }
	}
	return 0;										// not found
}

template<typename T>
T& List<T>::getVal(int i)const {
	// return the value of i-th item, i>=0, i=0 return head node info(may be null)
	Node<T>* curr = locate(i);
	if (curr != nullptr) { return curr->data; }
	else { std::cerr << "Getting value error! Reason: Argument i exceeded the list's length." << std::endl; exit(1); }
}

template<typename T>
bool List<T>::getVal(int i, T& x)const {
	// assign the value of i-th item to x, i>=0, i=0 return head node info(may be null)
	Node<T>* curr = locate(i);
	if (curr != nullptr) {							// locate successfully
		x = curr->data;
		return true;
	}
	else return false;								// i is too large, exceeding the list's length
}

template<typename T>
void List<T>::setVal(int i, const T& x) {	
	Node<T>* curr = locate(i);
	if (curr != nullptr) 							// locate successfully
		curr->data = x;
	else { std::cerr << "Setting value error! Reason: Argument i exceeded the list's length." << std::endl; exit(1); }
}

template<typename T>
bool List<T>::insert(int i, const T& x) {
	/* Insert a new element x after the i-th node, i>=0
	 * In particular, i=0, insert x after head node. 
	**/
	Node<T>* curr = locate(i);
	if (curr == nullptr) { std::cerr << "Insertion error! Reason: Argument i exceeded the list's length." << std::endl; return false; }

	Node<T>* newNode = new Node<T>(x);
	if (newNode == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	newNode->next = curr->next;
	curr->next = newNode;
	return true;
}

template<typename T>
bool List<T>::append(const T& x) {
	// add a new element x at the end of the list
	Node<T>* curr = head;
	// make pointer curr point to last node
	while (curr->next != nullptr)
		curr = curr->next;

	Node<T>* newNode = new Node<T>(x);
	if (newNode == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	newNode->next = curr->next;
	curr->next = newNode;
	return true;
}

template<typename T>
bool List<T>::remove(int i, T& x) {
	// remove the i-th node & store the value to be removed
	if (i < 1) { std::cout << "Deletion error! Reason: Invalid argument i, i must be no less than 1." << std::endl; exit(1); }
	Node<T>* delpre = locate(i - 1), *del = delpre->next;
	if (del == nullptr) {
		std::cout << "Deletion error! Reason: Argument i exceeded the list's length." << std::endl;
		return false;
	}

	delpre->next = del->next;
	x = del->data;
	delete del;
	return true;
}

template<typename T>
bool List<T>::remove(int i) {
	// remove the i-th node without storing it
	if (i < 1) { std::cout << "Deletion error! Reason: Invalid argument i, i must be no less than 1." << std::endl; exit(1); }
	Node<T>* delpre = locate(i - 1), *del = delpre->next;
	if (del == nullptr) {
		std::cout << "Deletion error! Reason: Argument i exceeded the list's length." << std::endl;
		return false;
	}

	delpre->next = del->next;
	delete del;
	return true;
}

template<typename T>
void List<T>::Union(List<T>& L2) {
	// union of two lists & store the result in *this
	Node<T>* curr = L2.head->next;
	T x;
	while (curr != nullptr) {
		x = curr->data;
		if (!search(x))								// not found x in this list
			append(x);
		curr = curr->next;
	}		
}

template<typename T>
void List<T>::Intersection(List<T>& L2) {
	// intersection of two lists & store the result in *this
	Node<T>* curr = head->next, *prev = head;
	T x;
	while (curr != nullptr) {
		x = curr->data;
		if (!L2.search(x)) {						// not found x in list L2			
			// erase x in this list
			prev->next = curr->next;
			Node<T>* del = curr;
			curr = curr->next;
			delete del;			
		}
		else {
			curr = curr->next;
			prev = prev->next;
		}			
	}
}

template<typename T>
void List<T>::input() {
	if (head->next != nullptr) {					// this list has at least one node
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
	Node<T>* curr = head;
	T tmp;
	std::cout << "Please input data. (end up with Ctrl+Z)" << std::endl;
	while (std::cin >> tmp) {
		curr->next = new Node<T>(tmp);
		if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		curr = curr->next;
	}
	std::cin.clear();								// reset the iostate of cin to good	
}

template<typename T>
void List<T>::output()const {
	Node<T>* curr = head->next;
	int i = 1;
	while (curr != nullptr) {
		// modify here for aesthetic according to practical lenth of the list
		printf("#%4d", i++);
		std::cout << ": " << curr->data << '\n';
		curr = curr->next;
	}
}

template<typename T>
void List<T>::Import(const std::string& filename, const std::string& mode_selection_text_or_binary) {
	if (head->next != nullptr) {					// this list has at least one node
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
		// read in ASCII text form

		// open file
		std::ifstream ifs(filename, ios_base::in);
		if (!ifs) {
			std::cerr << "Error in opening file for reading! Can't find file \"" << filename << "\".\n"
					  << "Please check the validity of its directory or filename." << std::endl;
			exit(1);
		}

		// read data		
		T tmp; Node<T>* curr = head;		
		while (ifs >> tmp) {
			curr->next = new Node<T>(tmp);
			if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
			curr = curr->next;
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
			T tmp; Node<T>* curr = head;
			while (ifs.read((char*)&tmp, sizeof(tmp))) {
				curr->next = new Node<T>(tmp);
				if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
				curr = curr->next;
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
}

template<typename T>
void List<T>::Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const {
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
		Node<T>* curr = head->next;
		while (curr != nullptr) {
			ofs << curr->data << '\n';
			curr = curr->next;
		}

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
			Node<T>* curr = head->next;
			while (curr != nullptr) {
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

#endif // !LIST_H

