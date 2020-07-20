// tree header
#ifndef TREE_H
#define TREE_H
#include "SeqStack.h"
#include <memory>
#include <cassert>
#include <iostream>

template<typename T>
struct TreeNode {
	using node_ptr = TreeNode<T>*;
	T _data;
	node_ptr _first_child,  _next_sibling;
	TreeNode(const T& value = T{}, node_ptr fc = nullptr, node_ptr ns = nullptr)
		: _data(value), _first_child(fc), _next_sibling(ns) {}
	~TreeNode() { std::cout << "Destructing tree node with value " << _data << '\n'; }
};

template<typename T>
class Tree {
public:
	using node_ptr = typename TreeNode<T>::node_ptr;

	Tree() { _root = nullptr; }
	~Tree() { destroy_tree(_root); };
	void create_tree(std::istream& is, node_ptr& t);
	void destroy_tree(node_ptr t);
	template<typename U>
	friend std::istream& operator>>(std::istream& is, Tree<U>& tree);

	node_ptr get_root()const { return _root; }
	bool empty()const { return _root == nullptr; }
	size_t depth(node_ptr t)const;
	size_t count_node(node_ptr t)const;
	size_t count_leaves(node_ptr t)const;

	// We do NOT set traverses as const member functions
	// given that we may need to modify those nodes.
	// see also <https://isocpp.org/wiki/faq/pointers-to-members>
	void preorder(node_ptr t, void (*visit)(node_ptr) = visit);
	void postorder(node_ptr t, void (*visit)(node_ptr) = visit);

	// print paths from root node to leaves
	void print_path(node_ptr t)const;

	void print_tree(node_ptr t)const;
	
private:
	node_ptr _root;
	static void visit(node_ptr t);
};

template<typename U>
std::istream& operator>>(std::istream& is, Tree<U>& tree)
{
	tree.create_tree(is, tree._root);
	return is;
}

// create tree by generalized list
// e.g. A(B(E), C, D(F,G))#
template<>	// specialization for char type
void Tree<char>::create_tree(std::istream& is, node_ptr& t)
{
	const char end_tag = '#';
	int CS{};	// input first Child or next Sibling: 1 or 2
	SeqStack<node_ptr> s;	// stack for parent pointers
	SeqStack<node_ptr> pcs;	// pos of current sibling in each level
	node_ptr curr{};		// pointer to new node
	node_ptr parent{};		// pointer to previous built parent node
	node_ptr sibling_pos{};
	char ch;
	is >> ch;
	while (ch != end_tag) {
		switch (ch)
		{
		case '(': CS = 1; s.push(curr);  break;	// enters next level & pushes pointer to parent
		case ',': CS = 2;  break;
		case ')': s.pop(); pcs.pop(); break;	// return to previous upper level
		default:
			curr = new TreeNode<char>(ch);
			if (t == nullptr)	// alright, actually we need t to be null under a typical input
				t = curr;
			else {// a stack underflow bug might occur if t
				  // is non-null and input start with letter
				parent = s.top();
				if (CS == 1) {
					pcs.push(parent->_first_child = curr);
				}
				else {
					sibling_pos = pcs.top();
					sibling_pos->_next_sibling = curr;
					// update current sibling pos
					pcs.top() = sibling_pos->_next_sibling;
				}
			}
		}
		is >> ch;
	}
}

template<typename T>
void Tree<T>::create_tree(std::istream& is, node_ptr& t)
{
}

template<typename T>
void Tree<T>::destroy_tree(node_ptr t)
{
	if (t != nullptr) {
		destroy_tree(t->_first_child);
		destroy_tree(t->_next_sibling);
		delete t;
	}
}

template<typename T>
size_t Tree<T>::depth(node_ptr t) const
{
	if (t == nullptr) return 0;
	size_t fc_depth = depth(t->_first_child) + 1;
	size_t ns_depth = depth(t->_next_sibling);

	return fc_depth > ns_depth ? fc_depth : ns_depth;
}

template<typename T>
size_t Tree<T>::count_node(node_ptr t) const
{
	if (t == nullptr) return 0;
	size_t count = 1;
	count += count_node(t->_first_child);
	count += count_node(t->_next_sibling);

	return count;
}

template<typename T>
size_t Tree<T>::count_leaves(node_ptr t) const
{
	static size_t count = 0;
	if (t != nullptr) {
		if (t->_first_child == nullptr)
			++count;
		else
			count_leaves(t->_first_child);

		count_leaves(t->_next_sibling);
	}
	return count;
}

template<typename T>
void Tree<T>::preorder(node_ptr t, void(*visit)(node_ptr))
{
	if (t != nullptr) {
		visit(t);
		preorder(t->_first_child, visit);
		preorder(t->_next_sibling, visit);
	}
}

template<typename T>
void Tree<T>::postorder(node_ptr t, void(*visit)(node_ptr))
{
	if (t != nullptr) {
		postorder(t->_first_child, visit);
		visit(t);
		postorder(t->_next_sibling, visit);
	}
}

template<typename T>
void Tree<T>::print_path(node_ptr t) const
{
	static SeqStack<T> s;
	while (t != nullptr) {
		s.push(t->_data);
		if (t->_first_child == nullptr) {
			s.bottom_up_traverse();
			std::cout << '\n';
		}
		else
			print_path(t->_first_child);
		s.pop();
		t = t->_next_sibling;
	}
}

template<typename T>
void Tree<T>::print_tree(node_ptr t) const
{
	if (t != nullptr) {
		std::cout << t->_data;
		if (t->_first_child != nullptr) {
			std::cout << '(';
			// print subtrees rooted in siblings
			for (auto p = t->_first_child; p != nullptr; p = p->_next_sibling) {
				print_tree(p);
				if (p->_next_sibling != nullptr)
					std::cout << ',';
			}
			std::cout << ')';
		}
	}
}

// operation for node p
template<typename T>
inline void Tree<T>::visit(node_ptr p)
{
	assert(p != nullptr);
	std::cout << p->_data;
}

#endif // !TREE_H
