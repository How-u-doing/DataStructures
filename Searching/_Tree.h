/** 
 *  internal header file for implementing symbol tables
 *  ordered: BST/AVL_tree/RB_tree(map);
 *  unordered: set
 *  see following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/_Tree.h
 */
#ifndef _TREE_H
#define _TREE_H
//#include <utility>
//#include <string>
//#include <iterator>
#include <iostream>

#ifndef NDEBUG
#   define _assert(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define _assert(condition, message) do { } while (false)
#endif

template<typename T>
struct Tree_node {
	using value_type = T;
	using node_ptr = Tree_node<T>*;

	Tree_node(T val, size_t N = 0, node_ptr parent = nullptr,
		node_ptr left = nullptr, node_ptr right = nullptr) :
		_data(val), _N(N), _parent(parent), _left(left), _right(right) {}

	T* val_ptr() {
		return std::addressof(_data);
	}
	const T* val_ptr() const {
		return std::addressof(_data);
	}

	T _data;
	size_t _N; // size of the subtree rooted at this node
	node_ptr _parent, _left, _right;
};

template<typename T, typename Compare>
class Tree {
protected:
	class Tree_iter;
	class Tree_const_iter;
	using _self = Tree<T, Compare>;
	using node = Tree_node<T>;
	using node_ptr = node*;
	node_ptr _root;
	Compare _comp;

public:
	using value_type = T;
	using iterator = Tree_iter;
	using const_iterator = Tree_const_iter;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	Tree() : _root{ nullptr }, _comp{ Compare() } {}

	Tree(const _self& rhs) : _root{ copy(rhs._root, nullptr) } {}

	_self& operator=(const _self& rhs) {
		_root = copy(rhs._root, nullptr);
		return *this;
	}

	iterator begin() noexcept {
		return iterator(leftmost(),this);
	}

	const_iterator begin() const noexcept {
		return const_iterator(leftmost(), this);
	}

	iterator end() noexcept {
		return iterator(nullptr, this);
	}

	const_iterator end() const noexcept {
		return const_iterator(nullptr, this);
	}

	reverse_iterator rbegin() noexcept {
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() noexcept {
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(begin());
	}

	const_iterator cbegin() const noexcept {
		return begin();
	}

	const_iterator cend() const noexcept {
		return end();
	}

	const_reverse_iterator crbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator(begin());
	}

	size_t size() const noexcept { return size(_root); }

	bool empty() const noexcept { return _root == nullptr; }

	bool contains(const T& key) const { return find(_root, key) != nullptr; }

	node_ptr& root() noexcept { return _root; }

	// element access for map
	/*Val& at(const Key& key) {
		node_ptr x = find(_root, key);
		if (x == nullptr) throw std::out_of_range("at() called with invalid argument: key NOT found");
		return x->_data;
	}
	Val& operator[](const Key& key) {
		node_ptr x = find(_root, key);
		if (x == nullptr) { insert(key, Val{}); return find(_root, key)->_data; }
		return x->_data;
	}
	const Val& at(const Key& key) const {
		node_ptr x = find(_root, key);
		if (x == nullptr) throw std::out_of_range("at() called with invalid argument: key NOT found");
		return x->_data;
	}*/

	// return end() if not found
	iterator find(const T& key) {
		return iterator(find(_root, key), this);
	}
	
	const_iterator find(const T& key) const {
		return const_iterator(find(_root, key), this);
	}

	// return end() if out of range
	iterator floor(const T& key) {
		return iterator(floor(_root, key), this);
	}

	const_iterator floor(const T& key) const {
		return const_iterator(floor(_root, key), this);
	}

	// return end() if out of range
	iterator ceiling(const T& key) {
		return iterator(ceiling(_root, key), this);
	}
	
	const_iterator ceiling(const T& key) const {
		return const_iterator(ceiling(_root, key), this);
	}
	
	// select the kth smallest (greatest) element
	// return end() if out of range
	iterator select(size_t k) {
		if (k >= size()) return end();
		return const_iterator(select(_root, k), this);
	}

	const_iterator select(size_t k) const {
		if (k >= size()) return end();
		return const_iterator(select(_root, k), this);
	}

	// number of keys in the subtree less (greater) than key
	size_t rank(const T& key) const {
		return rank(_root, key);
	}

	// modifiers
	void clear() { clear(_root); _root = nullptr; }

	void insert(const T& key) {
		insert(_root, nullptr, key);
	}

	void insert_or_assign(const T& key) {
		insert_or_assign(_root, key);
	}

	void erase(const T& key) {
		_root = erase(_root, key);
	}

	void print(size_t level = 100) const noexcept { print(_root, level); }

protected:
	// When using greater (operator>) for comp, the
	// leftmost node is on the contrary the largest.
	// precondition: x != nullptr
	static node_ptr _tree_min(node_ptr x) noexcept {
		while (x->_left != nullptr)
			x = x->_left;
		return x;
	}

	// precondition: x != nullptr
	static node_ptr _tree_max(node_ptr x) noexcept {
		while (x->_right != nullptr)
			x = x->_right;
		return x;
	}

	// precondition: x != nullptr
	static node_ptr _tree_next(node_ptr x) noexcept {
		if (x->_right != nullptr)
			return _tree_min(x->_right);
		node_ptr parent{}; // root->_parent = null
		while ((parent = x->_parent) != nullptr && x == parent->_right)
			x = parent;
		return parent; // nullptr if x is the last (largest) node
	}

	// precondition: x != nullptr
	static node_ptr _tree_prev(node_ptr x) noexcept {
		if (x->_left != nullptr)
			return _tree_max(x->_left);
		node_ptr parent{}; // root->_parent = null
		while ((parent = x->_parent) != nullptr && x == parent->_left)
			x = parent;
		return parent; // nullptr if x is the first (smallest) node
	}

	node_ptr leftmost() const noexcept {
		if (empty()) return nullptr;
		return _tree_min(_root);
	}

	node_ptr copy(node_ptr x, node_ptr parent) {
		if (x != nullptr) {
			node_ptr t{ new node(x->_data, x->_N, parent) };
			t->_left = copy(x->_left, t);
			t->_right = copy(x->_right, t);
			return t;
		}
		return nullptr;
	}

	node_ptr find(node_ptr x, const T& key) const {
		if (x == nullptr)   return nullptr;
		if (_comp(key, x->_data)) return find(x->_left, key);
		if (_comp(x->_data, key)) return find(x->_right, key);
		return x;
	}

	size_t size(node_ptr x) const { return x == nullptr ? 0 : x->_N; }
	
	// precondition: x != nullptr
	void update_size(node_ptr x) noexcept {
		x->_N = 1 + size(x->_left) + size(x->_right);
	}
	
	node_ptr floor(node_ptr x, const T& key) const {
		if (x == nullptr) return nullptr;
		if		(_comp(key, x->_data))  return floor(x->_left, key);
		else if (!_comp(x->_data, key)) return x; // x.key == key 
		node_ptr t{ floor(x->_right, key) };	  // x.key <  key 
		if (t == nullptr) return x;
		else return t;
	}

	node_ptr ceiling(node_ptr x, const T& key) const {
		if (x == nullptr) return nullptr;
		if		(_comp(x->_data, key))  return ceiling(x->_right, key);
		else if (!_comp(key, x->_data)) return x; // key == x.key 
		node_ptr t{ ceiling(x->_left, key) };	  // key <  x.key
		if (t == nullptr) return x;
		else return t;
	}

	// precondition: x != nullptr
	node_ptr select(node_ptr x, size_t k) const {
		size_t t{ size(x->_left) };
		if (k < t) return select(x->_left, k);
		else if (k == t) return x;
		else return select(x->_right, k - t - 1);
	}

	size_t rank(node_ptr x, const T& key) const {
		if (x == nullptr) return 0;
		if		(_comp(key, x->_data))  return rank(x->_left, key);
		else if (!_comp(x->_data, key)) return size(x->_left); // x.key == key
		else return 1 + size(x->_left) + rank(x->_right, key);
	}

	void clear(node_ptr x) {
		if (x != nullptr) {
			clear(x->_left);
			clear(x->_right);
			delete x;
		}
	}

	// do nothing if key already exists
	void insert(node_ptr& x, node_ptr x_parent, const T& key) {
		if (x == nullptr) { x = new node(key, 1, x_parent); return; }
		if		(_comp(key, x->_data))	insert(x->_left, x, key);
		else if (_comp(x->_data, key))  insert(x->_right, x, key);
		else	/*    x.key == key  */  return;
		update_size(x);
	}

	void insert_or_assign(node_ptr& x, node_ptr x_parent, const T& key) {
		if (x == nullptr) { x = new node(key, 1, x_parent); return; }
		if		(_comp(key, x->_data))	insert(x->_left, x, key);
		else if (_comp(x->_data, key))  insert(x->_right, x, key);
		else							{ x->_data = key;  return; }
		update_size(x);
	}

	node_ptr erase(node_ptr x, const T& key) {
		if (x == nullptr) return nullptr; // not found
		if		(_comp(key, x->_data)) x->_left = erase(x->_left, key);
		else if (_comp(x->_data, key)) x->_right = erase(x->_right, key);
		else {// found: x.key == key
			if (x->_left == nullptr) {
				node_ptr ret{ x->_right };
				if (ret != nullptr) ret->_parent = x->_parent;
				delete x; return ret;
			}
			if (x->_right == nullptr) {
				node_ptr ret{ x->_left };
				if (ret != nullptr) ret->_parent = x->_parent;
				delete x; return ret;
			}
			node_ptr t{ _tree_min(x->_right) };
			x->_data = t->_data;
			x->_right = erase_min(x->_right);
		}
		update_size(x);
		return x;
	}

	// preconditions: x != nullptr
	node_ptr erase_min(node_ptr x) {
		if (x->_left == nullptr) {
			node_ptr ret{ x->_right };
			if (ret != nullptr) ret->_parent = x->_parent;
			delete x; return ret;
		}
		x->_left = erase_min(x->_left);
		update_size(x);
		return x;
	}

	// print with level
	void print(node_ptr dir, size_t max_level) const noexcept {
		if (dir == nullptr) return;
		std::cout << dir->_data << '\n';
		size_t dir_count = 0, file_count = 0, curr_level = 1;
		dfs_print(dir, "", dir_count, file_count, curr_level, max_level);
		std::cout << '\n' << dir_count << " directories, " << file_count << " files\n";
	}

	void dfs_print(node_ptr dir, const std::string& children_prefix, size_t& dir_count,
		size_t& file_count, size_t& curr_level, size_t max_level) const noexcept
	{
		if (dir == nullptr) return;
		node_ptr curr{};
		if ((curr = dir->_left) != nullptr) {
			std::cout << children_prefix << "├── " << curr->_data << '\n';
			if (has_children(curr) && curr_level < max_level)
				dfs_print(curr, children_prefix + "|   ", ++dir_count, file_count, ++curr_level, max_level);
			else ++file_count;
		}
		else if (dir->_right != nullptr) {// only have right child, print left child as null
			std::cout << children_prefix << "├── " << "null\n";
		}

		if ((curr = dir->_right) != nullptr) {
			std::cout << children_prefix << "└── " << curr->_data << '\n';
			if (has_children(curr) && curr_level < max_level)
				dfs_print(curr, children_prefix + "    ", ++dir_count, file_count, ++curr_level, max_level);
			else ++file_count;
		}
		else if (dir->_left != nullptr) {// only have left child, print right child as null
			std::cout << children_prefix << "└── " << "null\n";
		}
	}

	// precondition: dir != nullptr
	bool has_children(node_ptr dir) const noexcept {
		return dir->_left != nullptr || dir->_right != nullptr;
	}

	class Tree_iter
	{
		using _self = Tree_iter;
		//using node_ptr = Tree_node<T>*;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		Tree_iter() : _ptr(nullptr), _ptree(nullptr) {}
		Tree_iter(node_ptr ptr, const Tree* ptree) : _ptr(ptr), _ptree(ptree) {}

		node_ptr ptr() const noexcept { return _ptr; }

		const Tree* cont() const noexcept { return _ptree; }

		reference operator*() const {
			_assert(_ptr != nullptr, "cannot dereference end() iterator");
			return *_ptr->val_ptr();
		}

		pointer operator->() const {
			_assert(_ptr != nullptr, "cannot dereference end() iterator");
			return _ptr->val_ptr();
		}

		_self& operator++() {
			_assert(_ptr != nullptr, "cannot increment end() iterator");
			_ptr = _tree_next(_ptr);
			return *this;
		}

		_self operator++(int) {
			_assert(_ptr != nullptr, "cannot increment end() iterator");
			_self tmp{ *this };
			_ptr = _tree_next(_ptr);
			return tmp;
		}

		_self& operator--() {
			if (_ptr == nullptr) _ptr = _tree_max(_ptree->_root);
			else				 _ptr = _tree_prev(_ptr);
			return *this;
		}

		_self operator--(int) {
			_self tmp{ *this };
			--*this;
			return tmp;
		}

		friend bool operator==(const _self& lhs, const _self& rhs) {
			_assert(lhs._ptree == rhs._ptree, "iterators incompatible");
			return lhs._ptr == rhs._ptr;
		}

		friend bool operator!=(const _self& lhs, const _self& rhs) {
			_assert(lhs._ptree == rhs._ptree, "iterators incompatible");
			return lhs._ptr != rhs._ptr;
		}
	protected:
		node_ptr _ptr;
		const Tree* _ptree;
	};

	class Tree_const_iter
	{
		using _self = Tree_const_iter;
		//using node_ptr = Tree_node<T>*;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		Tree_const_iter() : _ptr(nullptr), _ptree(nullptr) {}
		Tree_const_iter(node_ptr ptr, const Tree* ptree) : _ptr(ptr), _ptree(ptree) {}
		Tree_const_iter(const Tree_iter& other) : _ptr(other.ptr()), _ptree(other.cont()) {}

		node_ptr ptr() const noexcept { return _ptr; }

		const Tree* cont() const noexcept { return _ptree; }

		reference operator*() const {
			_assert(_ptr != nullptr, "cannot dereference end() iterator");
			return *_ptr->val_ptr();
		}

		pointer operator->() const {
			_assert(_ptr != nullptr, "cannot dereference end() iterator");
			return _ptr->val_ptr();
		}

		_self& operator++() {
			_assert(_ptr != nullptr, "cannot increment end() iterator");
			_ptr = _tree_next(_ptr);
			return *this;
		}

		_self operator++(int) {
			_assert(_ptr != nullptr, "cannot increment end() iterator");
			_self tmp{ *this };
			_ptr = _tree_next(_ptr);
			return tmp;
		}

		_self& operator--() {
			if (_ptr == nullptr) _ptr = _tree_max(_ptree->_root);
			else				 _ptr = _tree_prev(_ptr);
			return *this;
		}

		_self operator--(int) {
			_self tmp{ *this };
			--*this;
			return tmp;
		}

		friend bool operator==(const _self& lhs, const _self& rhs) {
			_assert(lhs._ptree == rhs._ptree, "iterators incompatible");
			return lhs._ptr == rhs._ptr;
		}

		friend bool operator!=(const _self& lhs, const _self& rhs) {
			_assert(lhs._ptree == rhs._ptree, "iterators incompatible");
			return lhs._ptr != rhs._ptr;
		}
	protected:
		node_ptr _ptr;
		const Tree* _ptree;
	};
};

#endif // !_TREE_H
