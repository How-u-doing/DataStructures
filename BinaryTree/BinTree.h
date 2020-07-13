// binary tree header
#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stack>
#include <queue>
#include <iostream>
#include <cassert>

// Binary Tree Node
template <typename T>
struct BinNode {
	T _data;
	BinNode<T>* _lchild, * _rchild;
	BinNode(const T& value = static_cast<T>(0), 
		BinNode<T>* lchild = nullptr, BinNode<T>* rchild = nullptr)
		:_data(value), _lchild(lchild), _rchild(rchild) {}
};

template <typename T>
class BinTree final {
public:
	using node_ptr = BinNode<T>*;
	using node_ref = BinNode<T>&;

	BinTree() :	_root(nullptr) {}
	BinTree(const BinTree<T>& tree);
	~BinTree() { destroy_tree(_root); }

	void create_tree(std::istream& is) { create_tree(is, _root); }
	void print_tree(std::ostream& os) const { print_tree(os, _root); }
	void create_tree(std::istream& is, node_ptr& subtree);
	node_ptr create_tree(const T* preorder, const T* inorder, size_t n);
	void print_tree(std::ostream& os, node_ptr subtree) const;
	node_ptr duplicate(node_ptr subtree);
	void destroy_tree(node_ptr subtree);
	BinTree& operator=(const BinTree<T>& tree);
	template<typename U>
	friend std::istream& operator>>(std::istream& is, BinTree<U>& tree);
	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, const BinTree<U>& tree);

	node_ptr get_root() const noexcept { return _root; }
	void set_root(node_ptr new_root);
	node_ptr parent(node_ptr curr) const { return parent(curr, _root); }
	node_ptr left_child(node_ptr curr) const { return curr->_lchild; }
	node_ptr right_child(node_ptr curr) const { return curr->_rchild; }
	node_ptr parent(node_ptr curr, node_ptr subtree) const;

	bool empty() const noexcept { return _root == nullptr; }
	size_t depth() const { return depth(_root); }
	size_t size() const { return node_count(_root); }
	size_t leaves_count() const { return leaves_count(_root); }
	size_t depth(node_ptr subtree) const;
	size_t node_count(node_ptr subtree) const;
	size_t leaves_count(node_ptr subtree) const;

	void preorder() { preorder(_root, visit); }
	void inorder() { inorder(_root, visit); }
	void postorder() { postorder(_root, visit); }
	void level_order() { level_order(_root, visit); }
	void preorder(node_ptr subtree, void (*visit)(node_ptr) = visit);
	void inorder(node_ptr subtree, void (*visit)(node_ptr) = visit);
	void postorder(node_ptr subtree, void (*visit)(node_ptr) = visit);
	void level_order(node_ptr subtree, void (*visit)(node_ptr) = visit);

	static void visit(node_ptr pos);

private:
	node_ptr _root;
};

template<typename T>
BinTree<T>::BinTree(const BinTree<T>& tree)
{
	if (&tree == this) return;
	_root = duplicate(tree._root);
}

template<typename T>
void BinTree<T>::create_tree(std::istream& is, node_ptr& subtree)
{
}

template<typename T>
BinNode<T>* BinTree<T>::create_tree(const T* preorder, const T* inorder, size_t n)
{
	if (n == 0) return nullptr;
	size_t root_pos = 0;
	while (preorder[0] != inorder[root_pos]) ++root_pos;
	node_ptr p = new BinNode<T>(preorder[0]);
	p->_lchild = create_tree(preorder + 1, inorder, root_pos);
	p->_rchild = create_tree(preorder + root_pos + 1, inorder + root_pos + 1, n - root_pos - 1);
	return p;
}

// create a tree: input via generalized list or preorder 
// e.g.  A(B(D,E(,G)), C(F,))#  or  ABD##E#G##CF###  
template<>
void BinTree<char>::create_tree(std::istream& is, node_ptr& subtree)
{
	const char end_tag = '#';
#ifdef preorder_create_tree
	char ch;
	is >> ch;
	if (ch != end_tag) {
		subtree = new BinNode<char>(ch); // assert(subtree);
		create_tree(is, subtree->_lchild);
		create_tree(is, subtree->_rchild);
	}
	else {
		subtree = nullptr;
	}
#else	// by generalized list
	int LR{};	// input left or right node: 1 or 2
	std::stack<node_ptr> s; // one pointer for each level (except farthest level)
	node_ptr curr{};		// pointer to new node
	node_ptr parent{};		// pointer to previous built parent node
	char ch;
	is >> ch;
	while (ch != end_tag) {
		switch (ch)
		{
		case '(': LR = 1; s.push(curr); break; // enters next level & pushes pointer to parent
		case ',': LR = 2;  break;
		case ')': s.pop(); break; // return to previous upper level
		default:
			curr = new BinNode<char>(ch);
			if (subtree == nullptr)
				subtree = curr;
			else {// a stack underflow bug might occur if subtree
				  // is non-null and input start with letter
				parent = s.top();
				if (LR == 1)
					parent->_lchild = curr;
				else
					parent->_rchild = curr;
			}
		}
		is >> ch;
	}
#endif // preorder_create_tree	
}

// output tree via generalized list
// e.g.  A(B(D,E(,G)), C(F,))
template<typename T>
void BinTree<T>::print_tree(std::ostream& os, node_ptr subtree) const
{
	if (subtree != nullptr) {
		os << subtree->_data;
		if (subtree->_lchild != nullptr || subtree->_rchild != nullptr) {
			os << '(';
			print_tree(os, subtree->_lchild);
			os << ',';
			print_tree(os, subtree->_rchild);
			os << ')';
		}
	}
}

// return a pointer to a node that is the
// root of a duplicate of the subtree
template<typename T>
BinNode<T>* BinTree<T>::duplicate(node_ptr subtree)
{
	if (subtree == nullptr) return nullptr;
	node_ptr root = new BinNode<T>(subtree->_data);	// assert(root);
	root->_lchild = duplicate(subtree->_lchild);
	root->_rchild = duplicate(subtree->_rchild);
	return root;
}

template<typename T>
void BinTree<T>::destroy_tree(node_ptr subtree)
{
	if (subtree == nullptr) return;
	destroy_tree(subtree->_lchild);
	destroy_tree(subtree->_rchild);
	std::cout << "destroying node " << subtree->_data << " with address ";
#if defined _WIN32 || _WIN64
	std::cout << "0x";	// VS2019 in Windows do NOT print prefix
#endif // defined _WIN32 || _WIN64
	std::cout << subtree << '\n';
	delete subtree;
}

template<typename T>
BinTree<T>& BinTree<T>::operator=(const BinTree<T>& tree)
{
	if (&tree != this) {
		destroy_tree(_root);
		_root = duplicate(tree._root);
	}
	return *this;
}

template<typename T>
void BinTree<T>::set_root(node_ptr new_root)
{
	if (new_root == _root) return;
	destroy_tree(_root);
	_root = duplicate(new_root);
}

template<typename T>
BinNode<T>* BinTree<T>::parent(node_ptr curr, node_ptr subtree) const
{
	if (subtree == nullptr || curr == nullptr || curr == _root)
		return nullptr;

	if (subtree->_lchild == curr || subtree->_rchild == curr)
		return subtree;
	else {
		node_ptr p = parent(curr, subtree->_lchild);
		if (p != nullptr)	// found in left-child subtree
			return p;
		else
			return parent(curr, subtree->_rchild);
	}
}

template<typename T>
size_t BinTree<T>::depth(node_ptr subtree) const
{
	if (subtree == nullptr)
		return 0;
	else {
		size_t lc_depth = depth(subtree->_lchild);
		size_t rc_depth = depth(subtree->_rchild);
		return 1 + (lc_depth > rc_depth ? lc_depth : rc_depth);
	}
}

template<typename T>
size_t BinTree<T>::node_count(node_ptr subtree) const
{
	if (subtree == nullptr)
		return 0;
	else
		return 1 + node_count(subtree->_lchild)
		         + node_count(subtree->_rchild);
}

template<typename T>
size_t BinTree<T>::leaves_count(node_ptr subtree) const
{
	if (subtree == nullptr)	
		return 0;
	else {
		if (subtree->_lchild == nullptr && subtree->_rchild == nullptr)
			return 1;
		else
			return leaves_count(subtree->_lchild) + leaves_count(subtree->_rchild);
	}
}

template<typename T>
void BinTree<T>::preorder(node_ptr subtree, void(*visit)(node_ptr))
{
	if (subtree != nullptr) {
#ifdef nonrecursive_visit
		std::stack<node_ptr> s;
		node_ptr p = subtree;
		s.push(p);
		while (!s.empty()) {
			p = s.top(); s.pop();
			visit(p);
			if (p->_rchild != nullptr)
				s.push(p->_rchild);
			if (p->_lchild != nullptr)// left child pushed after
				s.push(p->_lchild);	  // right to visit left first
		}
#else	// recursive visit
		visit(subtree);
		preorder(subtree->_lchild, visit);
		preorder(subtree->_rchild, visit);
#endif // nonrecursive_visit
	}
}

template<typename T>
void BinTree<T>::inorder(node_ptr subtree, void(*visit)(node_ptr))
{
	if (subtree != nullptr) {
#ifdef nonrecursive_visit
		std::stack<node_ptr> s;
		node_ptr p = subtree;
		while (p != nullptr || !s.empty()) {
			if (p != nullptr) {
				s.push(p);
				p = p->_lchild;
			}
			else {
				p = s.top(); s.pop();
				visit(p);
				p = p->_rchild;
			}
		}
#else	// recursive visit
		inorder(subtree->_lchild, visit);
		visit(subtree);
		inorder(subtree->_rchild, visit);
#endif // nonrecursive_visit
	}
}

template<typename T>
void BinTree<T>::postorder(node_ptr subtree, void(*visit)(node_ptr))
{	
	if (subtree != nullptr) {
#ifdef nonrecursive_visit
		std::stack<node_ptr> s;
		node_ptr p = subtree, q = subtree;
		while (p != nullptr) {
			while (p->_lchild != nullptr) {
				s.push(p);
				p = p->_lchild;
			}
			while (p->_rchild == nullptr || p->_rchild == q) {
				visit(p);
				q = p;
				if (s.empty()) return;
				p = s.top(); s.pop();
			}
			s.push(p);
			p = p->_rchild;
		}
#else	// recursive visit
		postorder(subtree->_lchild, visit);
		postorder(subtree->_rchild, visit);
		visit(subtree);
#endif // nonrecursive_visit
	}
}

template<typename T>
void BinTree<T>::level_order(node_ptr subtree, void(*visit)(node_ptr))
{
	if (subtree == nullptr) return;
	std::queue<node_ptr> q;
	node_ptr p = subtree;
	q.push(p);
	while (!q.empty()) {
		p = q.front(); q.pop();
		visit(p);
		if (p->_lchild != nullptr)
			q.push(p->_lchild);
		if (p->_rchild != nullptr)
			q.push(p->_rchild);
	}
}

// operations for a specific tree node
template<typename T>
inline void BinTree<T>::visit(node_ptr pos)
{
	assert(pos != nullptr);
	std::cout << pos->_data;
}

template<typename U>
inline std::istream& operator>>(std::istream& is, BinTree<U>& tree)
{
	tree.create_tree(is, tree._root);
	return is;
}

template<typename U>
inline std::ostream& operator<<(std::ostream& os, const BinTree<U>& tree)
{
	tree.print_tree(os, tree._root);
	return os;
}

#endif // !BINARYTREE_H

