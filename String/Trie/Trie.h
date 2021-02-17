/*
 *  symbol table with key specialized as string
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/String/Trie/Trie.h
 */

#ifndef TRIE_H
#define TRIE_H 1

#include <string>
#include <vector>
#include <stdexcept>

namespace mySymbolTable {

// Trie Symbol Table: key=string -> value
template<typename T>
class Trie {
private:
	typedef unsigned char uchar;
	static const size_t R = 256; // extented ASCII
	struct Node {
		T* pval = nullptr;  // here we use a pointer instead of an object entity given that internal
							// nodes doesn't need to store objects (and thus saving memory)
		Node* next[R] = {};

		Node() {}
		Node(const T& val) : pval(new T(val)) {}
		~Node() { delete pval; }
	};
	using node_ptr = Node*;

	node_ptr root;	// pointer to root node
	size_t n;		// no. of keys in trie
public:
	Trie() : root(nullptr), n(0) {}

	~Trie() { clear(); }

	void clear() { clear(root); root = nullptr; }
	
	// or simply: return *(insert(key, T())->pval);
	T& operator[](const std::string& key) {
		if (key == "") throw std::invalid_argument("key to operator[] cannot be null");
		node_ptr x = find(key);
		if (x == nullptr) // e.g. find "shell" in "she"
			return *(insert(key, T())->pval);
		else if (x->pval == nullptr) // e.g. find "she" in "shell"
			x->pval = new T();		 // set it a default value
		return *(x->pval);
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
		if (node_ptr x = find(key))
			return x->pval != nullptr;
		else return false;
	}

	std::vector<std::string> keys() const {
		return keys_with_prefix("");
	}

	std::vector<std::string> keys_with_prefix(const std::string& prefix) const {
		std::vector<std::string> vs;
		collect(find(prefix), prefix, vs);
		return vs; // it'll be totally fine to return large object due to RVO
	}

	// using wildcard ? to represent any single character
	// e.g. file?.h matches first two in {file1.h, file2.h, file3.cc}
	std::vector<std::string> keys_that_match(const std::string& pattern) const {
		if (pattern == "") throw std::invalid_argument("pattern to keys_that_match() cannot be null");
		std::vector<std::string> vs;
		collect(root, "", pattern, vs);
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
		for (size_t c = 0; c < R; ++c) {
			if (x->next[c] != nullptr) return false;
		}
		return true;
	}

	node_ptr find(const std::string& key) const {
		return find(root, key, 0);
	}

	// return pointer to key node, null if not found
	node_ptr find(node_ptr x, const std::string& key, size_t d) const {
		if (x == nullptr) return nullptr;
		if (d == key.length()) return x;
		return find(x->next[(uchar)key[d]], key, d + 1);
	}

	// non-recursive version
	/*node_ptr find(node_ptr x, const std::string& key, size_t d) const {
		for (; x != nullptr; x = x->next[(uchar)key[d++]])
			if (d == key.length())	return x;
		return nullptr;
	}*/

	// return the length of the longest key in subtrie that is a substring of query
	size_t max_len(node_ptr x, const std::string& query, size_t d, size_t len) const {
		if (x == nullptr) return len;
		if (x->pval != nullptr) len = d;
		if (d == query.length()) return len;
		return max_len(x->next[(uchar)query[d]], query, d + 1, len);
	}

	void clear(node_ptr x) {
		if (x == nullptr) return;
		for (size_t c = 0; c < R; ++c)
			clear(x->next[c]);
		delete x;
	}

	void collect(node_ptr x, const std::string& prefix, std::vector<std::string>& vs) const {
		// e.g. collect "shell" and "shore" starting at "sh"
		if (x == nullptr) return;
		if (x->pval != nullptr) vs.push_back(prefix);
		for (size_t c = 0; c < R; ++c)
			collect(x->next[c], prefix + (char)c, vs);
	}

	void collect(node_ptr x, const std::string& prefix, const std::string& pattern,
														std::vector<std::string>& vs) const {
		// e.g. collect  file?.h  in {file1.h, file2.h, file3.cc}
		if (x == nullptr) return;
		size_t d = prefix.length();
		if (d == pattern.length() && x->pval != nullptr) vs.push_back(prefix);
		if (d == pattern.length()) return;

		uchar ch = pattern[d];
		if (ch == '?') { // wildcard that matches any single character
			for (size_t c = 0; c < R; ++c)
				collect(x->next[c],  prefix + (char)c,  pattern, vs);
		}
		else	collect(x->next[ch], prefix + (char)ch, pattern, vs);
	}

	// no overwriting if key already exists
	// return pointer to new inserted node or key node
	node_ptr insert(node_ptr& x, const std::string& key, const T& val, size_t d) {
		// e.g. insert "shell" in "she"
		if (x == nullptr) x = new Node();
		if (d == key.length()) {
			if (x->pval == nullptr) {
				x->pval = new T(val); ++n;
			} // else do nothing
			return x;
		}
		return insert(x->next[(uchar)key[d]], key, val, d + 1);
	}

	// overwrite if key already exists
	// return pointer to new inserted or overwritten node
	node_ptr insert_or_assign(node_ptr& x, const std::string& key, const T& val, size_t d) {
		// e.g. insert "shell" in "she"
		if (x == nullptr) x = new Node();
		if (d == key.length()) {
			if (x->pval == nullptr) {
				x->pval = new T(val); ++n;
			}
			else *(x->pval) = val; // overwrite
			return x;
		}
		return insert_or_assign(x->next[(uchar)key[d]], key, val, d + 1);
	}

	// remove key and its associated value from trie, if any
	void erase(node_ptr& x, const std::string& key, size_t d) {
		// e.g. remove "shells" or "she" in  {"shells", "she"}
		if (x == nullptr) return;
		if (d == key.length()) {
			if (x->pval != nullptr) { // found
				delete x->pval;	--n;
				x->pval = nullptr;
			}
		}
		else erase(x->next[(uchar)key[d]], key, d + 1);

		if (is_leaf(x) && x->pval == nullptr) {
			delete x; x = nullptr;
		}
	}
};

} // mySymbolTable

#endif // !TRIE_H
