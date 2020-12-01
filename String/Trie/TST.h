/*
 *  symbol table with key specialized as string:
 *  see following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/String/Trie/TST.h
 */
#ifndef TST_H
#define TST_H
#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>

namespace mySymbolTable {

// Ternary Search Tree symbol table: key=string -> value
template<typename T>
class TST {
	struct Node {
		char ch = '\0';
		T* pval = nullptr;	// here we use a pointer instead of an object entity given that internal
							// nodes doesn't need to store objects (and thus saving memory)
		Node* left = nullptr, * mid = nullptr, * right = nullptr;

		Node() {}
		Node(char c) : ch(c) {}
		//Node(const T& val) : pval(new T(val)) {}
		~Node() { delete pval; }
	};
	using node_ptr = Node*;

	node_ptr root;	// pointer to root node
	size_t n;		// no. of keys in TST
public:
	TST() : root(nullptr), n(0) {}

	~TST() { clear(); }

	void clear() { clear(root); root = nullptr; }

	T& operator[](const std::string& key) {
		if (key == "") throw std::invalid_argument("key to operator[] cannot be null");
		return *(insert(key, T())->pval);
	}

	const T& at(const std::string& key) const {
		if (key == "") throw std::invalid_argument("key to at() cannot be null");
		node_ptr x = find(key);
		if (x == nullptr || x->pval == nullptr) // e.g. find "shell" in "she", or reverse
			throw std::out_of_range("invalid key to at()");
		return *(x->pval);
	}

	T& at(const std::string& key) {
		if (key == "") throw std::invalid_argument("key to at() cannot be null");
		node_ptr x = find(key);
		if (x == nullptr || x->pval == nullptr) // e.g. find "shell" in "she", or reverse
			throw std::out_of_range("invalid key to at()");
		return *(x->pval);
	}

	size_t size() const noexcept { return n; }

	bool empty() const noexcept { return n == 0; }

	bool contains(const std::string& key) const {
		if (key == "") throw std::invalid_argument("key to contains() cannot be null");
		if (node_ptr x = find(key))	return x->pval != nullptr;
		return false;
	}

	std::vector<std::string> keys() const {
		std::vector<std::string> vs;
		collect(root, "", vs);
		return vs; // it'll be totally fine to return large object due to RVO
	}

	std::vector<std::string> keys_with_prefix(const std::string& prefix) const {
		if (prefix == "") return keys();
		std::vector<std::string> vs;
		node_ptr x = find(prefix);
		if (x == nullptr) return vs; // no matches, return empty vector
		if (x->pval != nullptr)	vs.push_back(prefix);
		collect(x->mid, prefix, vs);
		return vs; // it'll be totally fine to return large object due to RVO
	}

	// using wildcard ? to represent any single character
	// e.g. file?.h matches first two in {file1.h, file2.h, file3.cc}
	std::vector<std::string> keys_that_match(const std::string& pattern) const {
		if (pattern == "") throw std::invalid_argument("pattern to keys_that_match() cannot be null");
		std::vector<std::string> vs;
		collect(root, "", pattern, 0, vs);
		return vs;
	}

	std::string longest_prefix_of(const std::string& query) {
		if (query == "") throw std::invalid_argument("query to longest_prefix_of() cannot be null");
		size_t len = max_len(root, query, 0, 0); // 0 if no substring of query
		return query.substr(0, len);
	}

	node_ptr insert(const std::string& key, const T& val) {
		if (key == "") throw std::invalid_argument("key to insert() cannot be null");
		return insert(root, key, val, 0);
	}

	node_ptr insert_or_assign(const std::string& key, const T& val) {
		if (key == "") throw std::invalid_argument("key to insert_or_assign() cannot be null");
		return insert_or_assign(root, key, val, 0);
	}

	void erase(const std::string& key) {
		if (key == "") throw std::invalid_argument("key to erase() cannot be null");
		erase(root, key, 0);
	}
protected:
	// precondition: x != nullptr
	bool is_leaf(node_ptr x) const {
		return !(x->left || x->mid || x->right);
	}

	// precondition: key != "" (null string)
	node_ptr find(const std::string& key) const {
		return find(root, key, 0);
	}

#if defined(RECURSIVE_TST)
	// return pointer to key node, null if not found
	node_ptr find(node_ptr x, const std::string& key, size_t d) const {
		if (x == nullptr) return nullptr;
		if		(key[d] < x->ch)	return find(x->left, key, d);
		else if (key[d] > x->ch)	return find(x->right, key, d);
		else if (d < key.length() - 1)	return find(x->mid, key, d + 1);
		else return x;
	}

	// no overwriting if key already exists
	// return pointer to new inserted node or key node
	node_ptr insert(node_ptr& x, const std::string& key, const T& val, size_t d) {
		// e.g. insert "shell" in "she", "shore" in "shell"
		if (x == nullptr) x = new Node(key[d]);
		if		(key[d] < x->ch) return insert(x->left,  key, val, d);
		else if (key[d] > x->ch) return insert(x->right, key, val, d);
		else if (d < key.length() - 1) return insert(x->mid, key, val, d + 1);
		else { // found
			if (x->pval == nullptr) {
				x->pval = new T(val); ++n;
			} // else do nothing
			return x;
		}
	}

	// overwrite if key already exists
	// return pointer to new inserted or overwritten node
	node_ptr insert_or_assign(node_ptr& x, const std::string& key, const T& val, size_t d) {
		// e.g. insert "shell" in "she", "shore" in "shell"
		if (x == nullptr) x = new Node(key[d]);
		if		(key[d] < x->ch) return insert_or_assign(x->left,  key, val, d);
		else if (key[d] > x->ch) return insert_or_assign(x->right, key, val, d);
		else if (d < key.length() - 1) return insert_or_assign(x->mid, key, val, d + 1);
		else { // found
			if (x->pval == nullptr) {
				x->pval = new T(val); ++n;
			}
			else *(x->pval) = val; // overwrite
			return x;
		}
	}

	// precondition: query != "" (null string)
	// return the length of the longest key in sub-tst that is a substring of query
	size_t max_len(node_ptr x, const std::string& query, size_t d, size_t len) const {
		if (x == nullptr) return len;
		if (query[d] < x->ch) return max_len(x->left, query, d, len);
		else if (query[d] == x->ch) {
			if (x->pval != nullptr) len = d + 1;
			if (d == query.length() - 1) return len;
			return max_len(x->mid, query, d + 1, len);
		}
		else return max_len(x->right, query, d, len);
	}
#else	// non-recursive versions
	// return pointer to key node, null if not found
	node_ptr find(node_ptr x, const std::string& key, size_t d) const {
		while (x != nullptr) {
			if		(key[d] < x->ch)	x = x->left;
			else if (key[d] > x->ch)	x = x->right;
			else if (d < key.length() - 1) { x = x->mid; ++d; }
			else return x;
		}
		return nullptr;
	}

	// no overwriting if key already exists
	// return pointer to new inserted node or key node
	node_ptr insert(node_ptr& x, const std::string& key, const T& val, size_t d) {
		// e.g. insert "shell" in "she", "shore" in "shell"
		Node** curr = &x; // *curr is a reference to pointer
		for (;;) {
			if (*curr == nullptr) *curr = new Node(key[d]);
			if		(key[d] < (*curr)->ch)	 curr = & ( (*curr)->left  );
			else if (key[d] > (*curr)->ch)	 curr = & ( (*curr)->right );
			else if (d < key.length() - 1) { curr = & ( (*curr)->mid   ); ++d; }
			else { // found
				if ((*curr)->pval == nullptr) {					
					(*curr)->pval = new T(val); ++n;
				} // else do nothing
				return *curr;
			}
		}
	}

	// overwrite if key already exists
	// return pointer to new inserted or overwritten node
	node_ptr insert_or_assign(node_ptr& x, const std::string& key, const T& val, size_t d) {
		// e.g. insert "shell" in "she", "shore" in "shell"
		Node** curr = &x; // *curr is a reference to pointer
		for (;;) {
			if (*curr == nullptr) *curr = new Node(key[d]);
			if		(key[d] < (*curr)->ch)	 curr = & ( (*curr)->left  );
			else if (key[d] > (*curr)->ch)	 curr = & ( (*curr)->right );
			else if (d < key.length() - 1) { curr = & ( (*curr)->mid   ); ++d; }
			else { // found
				if ((*curr)->pval == nullptr) {
					(*curr)->pval = new T(val);	++n;
				}
				else *((*curr)->pval) = val; // overwrite
				return *curr;
			}
		}		
	}

	// precondition: query != "" (null string)
	// return the length of the longest key in sub-tst that is a substring of query
	size_t max_len(node_ptr x, const std::string& query, size_t d, size_t len) const {
		for (;;) {
			if (x == nullptr) return len;
			if		(query[d] < x->ch) x = x->left;
			else if (query[d] > x->ch) x = x->right;
			else {// query[d] == x->ch
				if (x->pval != nullptr) len = d + 1;
				if (d == query.length() - 1) return len;
				x = x->mid; ++d;
			}
		}
	}
#endif // defined(RECURSIVE_TST)

	// remove key and its associated value in TST, if any
	void erase(node_ptr& x, const std::string& key, size_t d) {
		// e.g. remove "shells" or "she" in  {"shells", "she"}
		if (x == nullptr) return;
		if		(key[d] < x->ch) erase(x->left,  key, d);
		else if (key[d] > x->ch) erase(x->right, key, d);
		else if (d < key.length() - 1) erase(x->mid, key, d + 1);
		else {
			if (x->pval != nullptr) { // found
				delete x->pval;	--n;
				x->pval = nullptr;
			}
		}

		if (is_leaf(x) && x->pval == nullptr) {
			delete x; x = nullptr;
		}
	}

	void clear(node_ptr x) {
		if (x == nullptr) return;
		clear(x->left);
		clear(x->mid);
		clear(x->right);
		delete x;
	}

	// collect keys in TST rooted at x with given prefix
	// note that x points to the mid link node of prefix node (if any)
	void collect(node_ptr x, const std::string& prefix, std::vector<std::string>& vs) const {
		if (x == nullptr) return;
		collect(x->left, prefix, vs);
		if (x->pval != nullptr) vs.push_back(prefix + x->ch);
		collect(x->mid, prefix + x->ch, vs);
		collect(x->right, prefix, vs);
	}

	// precondition: pattern != "" (null string)
	// note that x points to the mid link node of prefix node (if any)
	void collect(node_ptr x, const std::string& prefix, const std::string& pattern,
		size_t i, std::vector<std::string>& vs) const {
		// e.g. collect  file?.h  in {file1.h, file2.h, file3.cc}
		if (x == nullptr) return;
		assert(i < pattern.length());

		char curr = pattern[i];
		if (curr < x->ch || curr == '?') collect(x->left, prefix, pattern, i, vs);
		if (curr == x->ch || curr == '?') {
			if (i == pattern.length() - 1 && x->pval != nullptr) vs.push_back(prefix + x->ch);
			if (i < pattern.length() - 1) collect(x->mid, prefix + x->ch, pattern, i + 1, vs);
		}
		if (curr > x->ch || curr == '?') collect(x->right, prefix, pattern, i, vs);
	}
};

} // namespace mySymbolTable

#endif // !TST_H 
