// tree header
#ifndef TREE_H
#define TREE_H
#include "SeqStack.h"
#include <memory>
#include <cassert>
#include <iostream>

template<typename T>
struct TreeNode {
	T _data;
	std::shared_ptr<TreeNode<T>> _first_child,  _next_sibling;
	TreeNode(const T value = {}, std::shared_ptr<TreeNode<T>> fc = nullptr, std::shared_ptr<TreeNode<T>> ns = nullptr)
		: _data(value), _first_child(fc), _next_sibling(ns) {}
	~TreeNode() { std::cout << "Destructing tree node with value " << _data << '\n'; }
};

template<typename T>
class Tree {
public:
	Tree() { _root = nullptr; }
	std::shared_ptr<TreeNode<T>> create_tree(std::shared_ptr<TreeNode<T>> t);

	std::shared_ptr<TreeNode<T>> get_root()const { return _root; }
	bool empty()const { return _root == nullptr; }
	size_t depth(std::shared_ptr<TreeNode<T>> t)const;
	size_t count_node(std::shared_ptr<TreeNode<T>> t)const;
	size_t count_leaves(std::shared_ptr<TreeNode<T>> t)const;

	// We do NOT set traverses as const member functions
	// given that we may need to modify those nodes.
	// see also <https://isocpp.org/wiki/faq/pointers-to-members>
	void preorder(std::shared_ptr<TreeNode<T>> t, void (*visit)(std::shared_ptr<TreeNode<T>>) = visit);
	void postorder(std::shared_ptr<TreeNode<T>> t, void (*visit)(std::shared_ptr<TreeNode<T>>) = visit);

	// print paths from root node to leaves
	void print_path(std::shared_ptr<TreeNode<T>> t)const;
	
private:
	std::shared_ptr<TreeNode<T>> _root;
	static void visit(std::shared_ptr<TreeNode<T>> t);
};

//create tree by level
template<>	// specialization for char type
std::shared_ptr<TreeNode<char>> Tree<char>::create_tree(std::shared_ptr<TreeNode<char>> t)
{
	// to be added..
	return std::make_shared<TreeNode<char>>();
}

template<typename T>
std::shared_ptr<TreeNode<T>> Tree<T>::create_tree(std::shared_ptr<TreeNode<T>> t)
{
	return std::make_shared<TreeNode<T>>();
}

template<typename T>
size_t Tree<T>::depth(std::shared_ptr<TreeNode<T>> t) const
{
	if (t == nullptr) return 0;
	size_t fc_depth = depth(t->_first_child) + 1;
	size_t ns_depth = depth(t->_next_sibling);

	return fc_depth > ns_depth ? fc_depth : ns_depth;
}

template<typename T>
size_t Tree<T>::count_node(std::shared_ptr<TreeNode<T>> t) const
{
	if (t == nullptr) return 0;
	size_t count = 1;
	count += count_node(t->_first_child);
	count += count_node(t->_next_sibling);

	return count;
}

template<typename T>
size_t Tree<T>::count_leaves(std::shared_ptr<TreeNode<T>> t) const
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
void Tree<T>::preorder(std::shared_ptr<TreeNode<T>> t, void(*visit)(std::shared_ptr<TreeNode<T>>))
{
	if (t != nullptr) {
		visit(t);
		preorder(t->_first_child, visit);
		preorder(t->_next_sibling, visit);
	}
}

template<typename T>
void Tree<T>::postorder(std::shared_ptr<TreeNode<T>> t, void(*visit)(std::shared_ptr<TreeNode<T>>))
{
	if (t != nullptr) {
		postorder(t->_first_child, visit);
		visit(t);
		postorder(t->_next_sibling, visit);
	}
}

template<typename T>
void Tree<T>::print_path(std::shared_ptr<TreeNode<T>> t) const
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

// operation for node p
template<typename T>
inline void Tree<T>::visit(std::shared_ptr<TreeNode<T>> p)
{
	assert(p != nullptr);
	std::cout << p->_data;
}

#endif // !TREE_H

