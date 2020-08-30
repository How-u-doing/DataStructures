#ifndef BST_H
#define BST_H
#include <queue>
#include <vector>
#include <stdexcept>

namespace myBST {

// ordered symbol table implemented by BST
template<typename Key, typename Val>
class BST {
	class Node;
	Node* _root;
public:
	BST() : _root{ nullptr } {}
	BST(const BST<Key, Val>& bst) : _root{ duplicate(bst._root) } {}
	BST<Key, Val>& operator=(const BST<Key, Val>& bst) {
		_root = duplicate(bst._root);
		return *this;
	}
	Node* duplicate() const { return duplicate(_root); }

	size_t size() const noexcept { return size(_root); }
	bool empty() const noexcept { return _root == nullptr; }
	bool contains(const Key& key) const { return find(_root, key) != nullptr; }
	Node*& root() noexcept { return _root; }

	// element access
	Val& at(const Key& key) {
		Node* x = find(_root, key);
		if (x == nullptr) throw std::invalid_argument("at() called with invalid argument: key NOT found");
		return x->_val;
	}
	Val& operator[](const Key& key) {
		Node* x = find(_root, key);
		if (x == nullptr) { insert(key, Val{}); return find(_root, key)->_val; }
		return x->_val;
	}
	const Val& at(const Key& key) const {
		Node* x = find(_root, key);
		if (x == nullptr) throw std::invalid_argument("at() called with invalid argument: key NOT found");
		return x->_val;
	}

	Key min() const {
		if (empty()) throw std::runtime_error("min() called on empty BST");
		return min(_root)->_key;
	}
	Key max() const {
		if (empty()) throw std::runtime_error("max() called on empty BST");
		return max(_root)->_key;
	}
	Key floor(const Key& key) const {
		if (empty()) throw std::runtime_error("floor() called on empty BST");
		Node* x{ floor(_root, key) };
		if (x == nullptr) throw std::invalid_argument("floor() called with invalid argument: key < min()");
		return x->_key;
	}
	Key ceiling(const Key& key) const {
		if (empty()) throw std::runtime_error("ceiling() called on empty BST");
		Node* x{ ceiling(_root, key) };
		if (x == nullptr) throw std::invalid_argument("ceiling() called with invalid argument: max() < key");
		return x->_key;
	}
	Key select(size_t k) const {
		if (k >= size()) throw std::invalid_argument("select() called with invalid argument: k >= size()");
		return select(_root, k)->_key;
	}
	size_t rank(const Key& key) const {
		if (empty()) throw std::runtime_error("rank() called on empty BST");
		return rank(_root, key);
	}

	// modifiers
	void clear() { clear(_root); }
	void insert(const Key& key, const Val& val) { _root = insert(_root, key, val); }
	void insert_or_assign(const Key& key, const Val& val) { _root = insert_or_assign(_root, key, val); }
	void erase(const Key& key) { _root = erase(_root, key); }
	void erase_min() { 
		if (empty()) throw std::underflow_error("erase_min() called on empty BST");
		_root = erase_min(_root);
	}
	void erase_max() { 
		if (empty()) throw std::underflow_error("erase_max() called on empty BST");
		_root = erase_max(_root);
	}

	void level_order(std::vector<Key>& keys) const {
		std::queue<Node*> que{};
		que.push(_root);
		while (!que.empty()) {
			Node* t{ que.front() }; que.pop();
			if (t == nullptr) continue;
			keys.push_back(t->_key);
			que.push(t->_left);
			que.push(t->_right);
		}
	}

protected:
	Node* duplicate(Node* x) {
		if (x != nullptr) {
			Node* t{ new Node(x->_key, x->_val, x->_N) };
			t->_left = duplicate(x->_left);
			t->_right = duplicate(x->_right);
			return t;
		}
		return nullptr;
	}

	Node* find(Node* x, const Key& key) const {
		if (x == nullptr)  return nullptr;
		if (key < x->_key) return find(x->_left, key);
		if (x->_key < key) return find(x->_right, key);
		return x;
	}

	size_t size(Node* x) const { return x == nullptr ? 0 : x->_N; }
	
	// precondition: x != nullptr
	void update_size(Node* x) {
		x->_N = 1 + size(x->_left) + size(x->_right);
	}
	// precondition: x != nullptr
	Node* min(Node* x) const {//return x->_left == nullptr ? x : min(x->_left);
		while (x->_left != nullptr)
			x = x->_left;
		return x;
	}
	// precondition: x != nullptr
	Node* max(Node* x) const {
		while (x->_right != nullptr)
			x = x->_right;
		return x;
	}
	Node* floor(Node* x, const Key& key) const {
		if (x == nullptr) return nullptr;
		if (key < x->_key) return floor(x->_left, key);
		else if (!(x->_key < key)) return x; // x.key == key 
		Node* t{ floor(x->_right, key) };	 // x.key <  key 
		if (t == nullptr) return x;
		return t;
	}
	Node* ceiling(Node* x, const Key& key) const {
		if (x == nullptr) return nullptr;
		if (x->_key < key) return ceiling(x->_right, key);
		else if (!(key < x->_key)) return x; // key == x.key 
		Node* t{ ceiling(x->_left, key) };	 // key <  x.key
		if (t == nullptr) return x;
		return t;
	}
	// precondition: x != nullptr
	Node* select(Node* x, size_t k) const {
		size_t t{ size(x->_left) };
		if (k < t) return select(x->_left, k);
		else if (k == t) return x;
		return select(x->_right, k - t - 1);
	}
	// precondition: x != nullptr
	size_t rank(Node* x, const Key& key) const {
		if (key < x->_key) return rank(x->_left, key);
		else if (!(x->_key < key)) return size(x->_left); // x.key == key
		return 1 + size(x->_left) + rank(x->_right, key);
	}

	void clear(Node* x) {
		if (x != nullptr) {
			clear(x->_left);
			clear(x->_right);
			delete x;
		}
	}

	// if (!contains(key)) insert_or_assign(key, val);
	Node* insert(Node*& x, const Key& key, const Val& val) {
		if (x == nullptr) return new Node(key, val, 1);
		if		(key < x->_key) x->_left = insert(x->_left, key, val);
		else if (x->_key < key) x->_right = insert(x->_right, key, val);
		else return x;
		update_size(x);
		return x;
	}
	Node* insert_or_assign(Node*& x, const Key& key, const Val& val) {
		if (x == nullptr) return new Node(key, val, 1);
		if		(key < x->_key) x->_left  = insert_or_assign(x->_left, key, val);
		else if (x->_key < key) x->_right = insert_or_assign(x->_right, key, val);
		else x->_val = val;
		update_size(x);
		return x;
	}
	// remove the node associated with key if it exists
	// return null when key not found or in a leaf node
	Node* erase(Node* x, const Key& key) {
		if (x == nullptr) return nullptr; // not found
		if		(key < x->_key) x->_left  = erase(x->_left, key);
		else if (x->_key < key) x->_right = erase(x->_right, key);
		else {// found: x.key == key
			if (x->_left == nullptr) { Node* ret{ x->_right }; delete x; return ret; }
			if (x->_right == nullptr) { Node* ret{ x->_left }; delete x; return ret; }
			Node* t{ min(x->_right) };
			x->_key = t->_key; x->_val = t->_val;
			x->_right = erase_min(x->_right);
		}
		update_size(x);
		return x;
	}
	// precondition: x != nullptr
	Node* erase_min(Node* x) {
		if (x->_left == nullptr) { Node* ret{ x->_right }; delete x; return ret; }
		x->_left = erase_min(x->_left);
		update_size(x);
		return x;
	}
	// precondition: x != nullptr
	Node* erase_max(Node* x) {
		if (x->_right == nullptr) { Node* ret{ x->_left }; delete x; return ret; }
		x->_right = erase_max(x->_right);
		update_size(x);
		return x;
	}

	struct Node {
		Key _key;
		Val _val;
		size_t _N; // size of the subtree rooted at this node
		Node* _left, * _right;
		Node(Key key, Val val, size_t N = 0, Node* left = nullptr, Node* right = nullptr)
			: _key{ key }, _val{ val }, _N{ N }, _left{ left }, _right{ right } {}
	};
};

} // namespace myBST

#endif // !BST_H
