// circular doubly linked list(with head node) header
#pragma once
#ifndef DBLLIST_H
#define DBLLIST_H

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include"LinearList.h"
#include"List.h"
#include"CircList.h"

template<typename T>
struct DLLNode {
	// (Circular) Doubly Linked List Node structure
	T data;
	DLLNode<T>* prev;
	DLLNode<T>* next;
	DLLNode(DLLNode<T>* previous = nullptr, DLLNode<T>* next = nullptr) : prev(previous), next(next) {};
	DLLNode(const T& val, DLLNode<T>* previous = nullptr, DLLNode<T>* next = nullptr) : data(val), prev(previous), next(next) {};
};

template<typename T>
class DblList : public LinearList<T> {
public:
	DblList();										// constructor with no infomation in data zone of head node
	DblList(const T& Info_for_head_node);			// constructor, & store info in head node
	DblList(const DblList<T>& L); 					// copy constructor
	DblList(const List<T>& L);						// copy constructor by class List<T>
	DblList(const CircList<T>& L);					// copy constructor by class CircList<T>
	DblList<T>& operator=(const DblList<T>& L);		// assignment operator= overloading	
	DblList<T>& operator=(const List<T>& L);		// assign via class List<T>
	DblList<T>& operator=(const CircList<T>& L);	// assign via class CircList<T>
	virtual ~DblList() { clear(); delete head; }	// virtual destructor
	virtual int size()const { return length(); }	// get the size of the list
	virtual int length()const;						// get the number of pactical existing items	
	virtual int search(const T& x)const;			// search specified item x and return its logical sequence number 	
	virtual bool insert(int i, const T& x);			// insert value x after the i-th item, 0<=i<=index_of_last_one_in_logical
	virtual bool append(const T& x);			 	// add value x at the end of list
	virtual bool remove(int i, T& x);				// remove the i-th item & store the removed value
	virtual bool remove(int i);						// remove the i-th item without storing the removed value	
	virtual void Union(const DblList<T>& L2);		// union of two lists, and store the result in *this
	virtual void Intersection(const DblList<T>& L2);// intersection of two lists, and store the result in *this
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
	// new functions in derived class DblList
	void clear();									// erase all
	DLLNode<T>* getHead()const { return head; }		// get the pointer to head node
	void setHead(DLLNode<T>* p);					// make head pointer point to some node that is also pointed by p
	DLLNode<T>* locate(int i)const;					// locate the i-th item	and return the pointer to this node	
	DLLNode<T>* find(const T& x)const;				// find specified item x and return the pointer to this node
	T& operator[](int index);						// access list items via []
	void Union(const List<T>& L2);					// union of List L2 and DblList "this", & store the result in *this
	void Union(const CircList<T>& L2);				// union of CircList L2 and DblList "this", & store the result in *this
	void Intersection(const List<T>& L2);			// intersection of List L2 and DblList "this", & store the result in *this
	void Intersection(const CircList<T>& L2);		// intersection of CircList L2 and DblList "this", & store the result in *this
private:
	DLLNode<T>* head;								// pointer to the head node of the list 
};


template<typename T>
DblList<T>::DblList() {
	/* constructor with no infomation in data zone of head node */
	// build a self-circled head node
	head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	head->next = head;
	head->prev = head;
}

template<typename T>
DblList<T>::DblList(const T& Info_for_head_node) {
	/* constructor, & store some info in head node */
	// build a self-circled head node
	head = new DLLNode<T>(Info_for_head_node);
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	head->next = head;
	head->prev = head;
}

template<typename T>
DblList<T>::DblList(const DblList<T>& L) {
	/* copy constructor */
	DLLNode<T>* srcptr = L.getHead();
	DLLNode<T>* desptr = head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != L.head) {
		// copy data from L & construct dynamic linked list
		desptr->next = new DLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

		// build prev link
		desptr->next->prev = desptr;

		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	desptr->next = head;
	head->prev = desptr;
}

template<typename T>
DblList<T>::DblList(const CircList<T>& L) {
	/* copy constructor by class CircList<T> */
	CLLNode<T>* srcptr = L.getHead(), *L_head = srcptr;
	DLLNode<T>* desptr = head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != L_head) {
		// copy data from L & construct dynamic linked list
		desptr->next = new DLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

		// build prev link
		desptr->next->prev = desptr;

		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	desptr->next = head;
	head->prev = desptr;
}

template<typename T>
DblList<T>::DblList(const List<T>& L) {
	/* copy constructor by class List<T> */ 
	Node<T>* srcptr = L.getHead();
	DLLNode<T>* desptr = head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != nullptr) {
		// copy data from L & construct dynamic linked list
		desptr->next = new DLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

		// build prev link
		desptr->next->prev = desptr;

		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	desptr->next = head;
	head->prev = desptr;
}

template<typename T>
DblList<T>& DblList<T>::operator=(const DblList<T>& L) {
	/* assignment operator= overloading */
	if (head->next != head) clear();				// erase all nodes but head node if list is not null
	DLLNode<T>* srcptr = L.getHead();
	DLLNode<T>* desptr = head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != L.head) {
		// copy data from L & construct dynamic linked list
		desptr->next = new DLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

		// build prev link
		desptr->next->prev = desptr;

		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	desptr->next = head;
	head->prev = desptr;
	return *this;
}

template<typename T>
DblList<T>& DblList<T>::operator=(const CircList<T>& L) {
	/* assign via class CircList<T> */
	if (head->next != head) clear();				// erase all nodes but head node if list is not null
	CLLNode<T>* srcptr = L.getHead(), *L_head = srcptr;
	DLLNode<T>* desptr = head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != L_head) {
		// copy data from L & construct dynamic linked list
		desptr->next = new DLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

		// build prev link
		desptr->next->prev = desptr;

		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	desptr->next = head;
	head->prev = desptr;
	return *this;
}

template<typename T>
DblList<T>& DblList<T>::operator=(const List<T>& L) {
	/* assign via class List<T> */
	if (head->next != head) clear();				// erase all nodes but head node if list is not null
	Node<T>* srcptr = L.getHead();
	DLLNode<T>* desptr = head = new DLLNode<T>;
	if (head == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
	while (srcptr->next != nullptr) {
		// copy data from L & construct dynamic linked list
		desptr->next = new DLLNode<T>(srcptr->next->data);
		if (desptr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

		// build prev link
		desptr->next->prev = desptr;

		// move pointers backward
		srcptr = srcptr->next;
		desptr = desptr->next;
	}
	// after the loop, desptr point to last node
	desptr->next = head;
	head->prev = desptr;
	return *this;
}

template<typename T>
T& DblList<T>::operator[](int index) {
	/* access list items via [], 0<=index<=length(), equivalent to getVal(int i)
	 * i=0, return head node info(may be null)
	**/
	if (index < 0) { std::cerr << "Using operator[] error! Reason: Invalid argument index, index must be no less than 0." << std::endl; exit(1); }
	DLLNode<T>* curr = head;
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
void DblList<T>::setHead(DLLNode<T>* p) {
	/* make head->next point to some node that is also pointed by p */
	if (p == head)									// the same, no need to change
		return;
	// else reset head node

	// take out head node
	head->prev->next = head->next;
	head->next->prev = head->prev;	
	
	// insert head node between new tail node and p
	p->prev->next = head;
	head->prev = p->prev;
	head->next = p;
	p->prev = head;
}

template<typename T>
void DblList<T>::clear() {
	/* erase all nodes but head node */
	DLLNode<T>* curr = head->next, *del;
	head->next = head;
	while (curr != head) {
		del = curr;
		curr = curr->next;
		delete del;
	}
}

template<typename T>
int DblList<T>::length()const {
	/* get the list's length */
	DLLNode<T>* curr = head->next;
	int count = 0;
	while (curr != head) {
		curr = curr->next;
		++count;
	}
	return count;
}

template<typename T>
DLLNode<T>* DblList<T>::locate(int i)const {
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
	DLLNode<T>* curr = head;
	while (i--)
		curr = curr->next;
	return curr;
}

template<typename T>
DLLNode<T>* DblList<T>::find(const T& x)const {
	/* find val x in the list & return its pointer. If cannot find return nullptr */
	DLLNode<T>* curr = head->next;
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
int DblList<T>::search(const T& x)const {
	/* search val x in the list & return its index. If cannot find return 0 */
	DLLNode<T>* curr = head->next;
	int index = 1;
	while (curr != head) {
		if (curr->data == x) return index;
		else { curr = curr->next; ++index; }
	}
	return 0;										// not found
}

template<typename T>
T& DblList<T>::getVal(int i)const {
	/* return the value of i-th item, i>=0.  i=0,     return head node info(may be null)
	 *								      i>length(), i:=i-length()-1, go on and on like this
	**/
	return locate(i)->data;
}

template<typename T>
bool DblList<T>::getVal(int i, T& x)const {
	/* assign the value of i-th item to x, i>=0 */
	x = locate(i)->data;
	return true;
}

template<typename T>
void DblList<T>::setVal(int i, const T& x) {
	/* set i-th item the value x. If i>length(), i:=i-length()-1, go on and on like this */
	locate(i)->data = x;
}

template<typename T>
bool DblList<T>::insert(int i, const T& x) {
	/* Insert a new element x after the i-th node, i>=0
	 * In particular, i=0, insert x after head node.
	**/
	DLLNode<T>* curr = locate(i), *newNode = new DLLNode<T>(x);
	if (newNode == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

	// build new links
	newNode->next = curr->next;
	curr->next->prev = newNode;
	curr->next = newNode;
	newNode->prev = curr;

	return true;
}

template<typename T>
bool DblList<T>::append(const T& x) {
	/* add a new element x after the last node(before head node) */ 
	DLLNode<T>* newNode = new DLLNode<T>(x);
	if (newNode == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }

	// insert x between head and tail
	head->prev->next = newNode;
	newNode->prev = head->prev;
	newNode->next = head;
	head->prev = newNode;

	return true;
}

template<typename T>
bool DblList<T>::remove(int i, T& x) {
	/* remove the i-th node & store the value to be removed */
	if (i < 1) { std::cout << "Deletion error! Reason: Invalid argument i, i must be no less than 1." << std::endl; exit(1); }
	DLLNode<T>* del = locate(i);
	if (del == head) {
		std::cout << "Deletion error! Cannot delete head node." << std::endl;
		return false;
	}

	// build new links & delete the i-th node
	del->prev->next = del->next;
	del->next->prev = del->prev;
	x = del->data;
	delete del;

	return true;
}

template<typename T>
bool DblList<T>::remove(int i) {
	/* remove the i-th node without storing it */
	if (i < 1) { std::cout << "Deletion error! Reason: Invalid argument i, i must be no less than 1." << std::endl; exit(1); }
	DLLNode<T>* del = locate(i);
	if (del == head) {
		std::cout << "Deletion error! Cannot delete head node." << std::endl;
		return false;
	}

	// build new links & delete the i-th node
	del->prev->next = del->next;
	del->next->prev = del->prev;
	delete del;

	return true;
}

template<typename T>
void DblList<T>::Union(const DblList<T>& L2) {
	/* union of two lists & store the result in *this */
	DLLNode<T>* curr = L2.head->next;
	T x;
	while (curr != L2.head) {
		x = curr->data;
		if (!search(x))								// not found x in this list
			append(x);
		curr = curr->next;
	}
}

template<typename T>
void DblList<T>::Union(const CircList<T>& L2) {
	/* union of CircList L2 and DblList "this", & store the result in *this */
	CLLNode<T>* curr = L2.getHead()->next, *L2_head = L2.getHead();
	T x;
	while (curr != L2_head) {
		x = curr->data;
		if (!search(x))								// not found x in this list
			append(x);
		curr = curr->next;
	}
}

template<typename T>
void DblList<T>::Union(const List<T>& L2) {
	/* union of List L2 and DblList "this", & store the result in *this */
	Node<T>* curr = L2.getHead()->next;
	T x;
	while (curr != nullptr) {
		x = curr->data;
		if (!search(x))								// not found x in this list
			append(x);
		curr = curr->next;
	}	
}

template<typename T>
void DblList<T>::Intersection(const DblList<T>& L2) {
	/* intersection of two lists & store the result in *this */
	DLLNode<T>* curr = head->next, *del;
	T x;
	while (curr != head) {
		x = curr->data;
		if (!L2.search(x)) {						// not found x in list L2			
			// erase x in this list
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			del = curr;
			curr = curr->next;
			delete del;
		}
		else  										// retain nodes with identical data
			curr = curr->next;
	}
}

template<typename T>
void DblList<T>::Intersection(const CircList<T>& L2) {
	/* intersection of CircList L2 and DblList "this", & store the result in *this */
	DLLNode<T>* curr = head->next, *del;
	T x;
	while (curr != head) {
		x = curr->data;
		if (!L2.search(x)) {						// not found x in list L2			
			// erase x in this list
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			del = curr;
			curr = curr->next;
			delete del;
		}
		else  										// retain nodes with identical data
			curr = curr->next;
	}
}

template<typename T>
void DblList<T>::Intersection(const List<T>& L2) {
	/* intersection of List L2 and DblList "this", & store the result in *this */
	DLLNode<T>* curr = head->next, *del;
	T x;
	while (curr != head) {
		x = curr->data;
		if (!L2.search(x)) {						// not found x in list L2			
			// erase x in this list
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			del = curr;
			curr = curr->next;
			delete del;
		}
		else  										// retain nodes with identical data
			curr = curr->next;
	}
}

template<typename T>
void DblList<T>::input() {
	if (head->next != head) {						// this list has at least one node
		std::cout << "Warning, the list is not null. Input new data will cover the original data\n";
		std::cout << "Are you sure to go on?(y or n)\n";
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
	DLLNode<T>* curr = head;
	T tmp;
	std::cout << "Please input data. (end up with Ctrl+Z)" << std::endl;
	while (std::cin >> tmp) {
		curr->next = new DLLNode<T>(tmp);
		if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
		curr->next->prev = curr;
		curr = curr->next;
	}
	// after the loop, curr points to the last node	
	curr->next = head;
	head->prev = curr;
	std::cin.clear();								// reset the iostate of cin to good
}

template<typename T>
void DblList<T>::output()const {
	DLLNode<T>* curr = head->next;
	int i = 1;
	while (curr != head) {
		// modify here for aesthetic according to practical lenth of the list
		printf("#%4d", i++);
		std::cout << ": " << curr->data << '\n';
		curr = curr->next;
	}
}

template<typename T>
void DblList<T>::Import(const std::string& filename, const std::string& mode_selection_text_or_binary) {
	if (head->next != head) {						// this list has at least one node
		std::cout << "Warning, the list is not null. Input new data will cover the original data\n";
		std::cout << "Are you sure to go on?(y or n)\n";
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
		T tmp; DLLNode<T>* curr = head;
		while (ifs >> tmp) {
			curr->next = new DLLNode<T>(tmp);
			if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
			curr->next->prev = curr;
			curr = curr->next;
		}
		curr->next = head;
		head->prev = curr;

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
			T tmp; DLLNode<T>* curr = head;
			while (ifs.read((char*)&tmp, sizeof(tmp))) {
				curr->next = new DLLNode<T>(tmp);
				if (curr->next == nullptr) { std::cerr << "Memory allocation error!" << std::endl; exit(1); }
				curr->next->prev = curr;
				curr = curr->next;
			}
			curr->next = head;
			head->prev = curr;

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
void DblList<T>::Export(const std::string& filename, const std::string& mode_selection_text_or_binary)const {
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
		DLLNode<T>* curr = head->next;
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
			DLLNode<T>* curr = head->next;
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

#endif // !DBLLIST_H

