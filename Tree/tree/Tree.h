// tree header
#ifndef TREE_H
#define TREE_H
#include "SeqStack.h"
#include <cassert>
#include <string>
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

	Tree() : _root(nullptr) {}
	~Tree() { destroy_tree(); };
	void create_tree(std::istream& is, node_ptr& t);
	void destroy_tree() { destroy_tree(_root); _root = nullptr; }
	template<typename U>
	friend std::istream& operator>>(std::istream& is, Tree<U>& tree);

	node_ptr get_root()const { return _root; }
	bool empty()const { return _root == nullptr; }
	size_t depth(node_ptr t)const;
	size_t count_node(node_ptr t)const { return count_node(t, true); }		
	size_t count_leaves(node_ptr t)const;

	// We do NOT set traversals as const member functions
	// given that we may need to modify those nodes.
	// see also <https://isocpp.org/wiki/faq/pointers-to-members>
	void preorder(node_ptr t, void (*visit)(node_ptr) = visit);
	void postorder(node_ptr t, void (*visit)(node_ptr) = visit);

	// print paths from root node to leaves
	void print_path(node_ptr t)const { SeqStack<T> s; print_path(t, s, true); }
	void print_tree(node_ptr t)const; // print tree in generalized list form

	// print tree by lines, like Linux tree command
	void print(size_t level = 100) const { print(_root, level); }

private:
	node_ptr _root;
	void destroy_tree(node_ptr t);
	static void visit(node_ptr t);
	size_t count_node(node_ptr t, bool is_same_level) const;
	void print_path(node_ptr t, SeqStack<T>& s, bool is_same_level) const;
	void print(node_ptr dir, size_t level) const;
	void dfs_print(node_ptr dir, const std::string& children_prefix,
		size_t& dir_count, size_t& file_count, size_t curr_level, size_t max_level) const;
	void print_val_is_dir(node_ptr x, bool is_dir) const;
	void dfs_print_aux(node_ptr curr, bool is_last_child, const std::string& children_prefix,
		size_t& dir_count, size_t& file_count, size_t curr_level, size_t max_level) const;

	// precondition: t != nullptr
	bool is_last_child(node_ptr t) const { return t->_next_sibling == nullptr; }
	bool has_children(node_ptr t) const { return t->_first_child != nullptr; }
};

template<typename U>
std::istream& operator>>(std::istream& is, Tree<U>& tree)
{
	tree.create_tree(is, tree._root);
	return is;
}

// create tree by generalized list, e.g. A(B(E), C, D(F,G))#
// precondition: t == nullptr
template<>	// specialization for char type
void Tree<char>::create_tree(std::istream& is, node_ptr& t)
{
	const char end_tag = '#';
	int CS{};	// input first Child or next Sibling: 1 or 2
	SeqStack<node_ptr> s;	// stack for parent pointers
	SeqStack<node_ptr> pcs;	// pos of current sibling in each level
	node_ptr curr{};		// pointer to new node
	node_ptr parent{};		// pointer to previous built parent node
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
				  // is non-null and input start with a letter
				parent = s.top();
				if (CS == 1) pcs.push(parent->_first_child = curr);
				else pcs.top() = pcs.top()->_next_sibling = curr;
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
size_t Tree<T>::count_node(node_ptr t, bool is_same_level) const
{
	if (t == nullptr)	return 0;
	if (is_same_level) 	return 1 + count_node(t->_first_child, false);
	else				return 1 + count_node(t->_first_child, false)
								 + count_node(t->_next_sibling, false);

	// count_node(node_ptr t) alternative impl:
	/*
	if (t == nullptr) return 0;
	size_t count = 1;
	for (node_ptr i = t->_first_child; i != nullptr; i = i->_next_sibling) {
		count += count_node(i);
	}
	return count;
	*/
}

template<typename T>
size_t Tree<T>::count_leaves(node_ptr t) const
{
	if (t == nullptr) return 0;
	size_t count = 0;
	for (node_ptr i = t->_first_child; i != nullptr; i = i->_next_sibling) {
		if (!has_children(i))  count += 1;
		else count += count_leaves(i);
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
void Tree<T>::print_path(node_ptr t, SeqStack<T>& s, bool is_same_level) const
{	
	while (t != nullptr) {
		s.push(t->_data);
		if (!has_children(t)) {
			s.bottom_up_traverse();
			std::cout << '\n';
		}
		else print_path(t->_first_child, s, false);
		if (is_same_level) return; //do Not print paths for siblings when first called
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

template<typename T>
void Tree<T>::print(node_ptr dir, size_t max_level)const {
	if (dir == nullptr) return;
	if (max_level == 0) {
		std::cout << "Invalid level, must be greater than 0.\n";
		return;
	}
	std::cout << "\033[0;33m" << dir->_data << "\033[0m\n";
	size_t dir_count = 0, file_count = 0, curr_level = 1;
	dfs_print(dir, "", dir_count, file_count, curr_level, max_level);
	std::cout << '\n' << dir_count << " directories, " << file_count << " files\n";
}

template<typename T>
void Tree<T>::print_val_is_dir(node_ptr x, bool is_dir) const
{
	if (is_dir) { // brown color for directories
		std::cout << "\033[0;33m" << x->_data << "\033[0m\n";
	}             // bold blue for files
	else std::cout << "\033[1;34m" << x->_data << "\033[0m\n";
}

template<typename T>
void Tree<T>::dfs_print_aux(node_ptr curr, bool is_last_child, const std::string& children_prefix,
	size_t& dir_count, size_t& file_count, size_t curr_level, size_t max_level) const
{
	std::string link_shape = !is_last_child ? "├── " : "└── ";
	std::string prefix_shape = !is_last_child ? "|   " : "    ";

	std::cout << children_prefix << link_shape;
	if (has_children(curr)) {
		++dir_count;
		print_val_is_dir(curr, true);
		if (curr_level < max_level)
			dfs_print(curr, children_prefix + prefix_shape, dir_count, file_count, curr_level + 1, max_level);
	}
	else {
		++file_count;
		print_val_is_dir(curr, false);
	}
}

template<typename T>
void Tree<T>::dfs_print(node_ptr dir, const std::string& children_prefix,
	size_t& dir_count, size_t& file_count, size_t curr_level, size_t max_level) const
{
	if (dir == nullptr) return;
	for (node_ptr i = dir->_first_child; i != nullptr; i = i->_next_sibling) {
		bool last_child = is_last_child(i);
		dfs_print_aux(i, last_child, children_prefix, dir_count, file_count, curr_level, max_level);
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
