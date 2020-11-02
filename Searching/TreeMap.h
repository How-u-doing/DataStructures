/**
 * ordered symbol table: TreeMap
 * see following link for the latest version
 * https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap.h
 */
#ifndef TREEMAP_H
#define TREEMAP_H
#define _map_impl_ 1  // to use alloc_and_replace
#include "_Tree.h"
#include <utility>    // std::pair
#include <functional> // std::less
#include <stdexcept>

namespace mySymbolTable {

template<typename Key, typename Val>
class MyPair : public std::pair<const Key, Val> {
public:
	MyPair() : std::pair<const Key, Val>{ Key(), Val() } {}
	MyPair(const Key& key, const Val& val) : std::pair<const Key, Val>{ key, val } {}
	//MyPair(const MyPair<Key, Val>& other) : std::pair<const Key, Val>{ other.first, other.second } {}

	MyPair& operator=(const MyPair<Key, Val>& other) {
		std::pair<const Key, Val>::second = other.second;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const MyPair& mypair) {
		os << '{' << mypair.first << ", " << mypair.second << '}';
		return os;
	}
};

template<typename Key, typename T, typename Compare = std::less<Key>>
class TreeMap
{	
	class value_compare;
public:
	using _self = TreeMap<Key, T, Compare>;
	using value_type = MyPair<Key, T>;
	using key_type = const Key;
	using mapped_type = T;

	using iterator = typename Tree<value_type, value_compare>::iterator;
	using const_iterator = typename Tree<value_type, value_compare>::const_iterator;
	using reverse_iterator = typename Tree<value_type, value_compare>::reverse_iterator;
	using const_reverse_iterator = typename Tree<value_type, value_compare>::const_reverse_iterator;

	TreeMap() : _comp(Compare()), _tree() {}

	TreeMap(const _self& rhs) : _comp(rhs._comp), _tree(rhs._tree) {}

	_self& operator=(const _self& rhs) {
		_tree = rhs._tree();
		return *this;
	}

	// element access
	// or simply: return insert(value_type{ key, T() })->second;
	T& operator[](const Key& key) {
		iterator it = find(key);
		if (it == end()) { return insert(value_type{ key, T() })->second; }
		return it->second;
	}

	T& at(const Key& key) {
		iterator it = find(key);
		if (it == end()) throw std::out_of_range("at() called with invalid key");
		return it->second;
	}

	const T& at(const Key& key) const {
		iterator it = find(key);
		if (it == end()) throw std::out_of_range("at() called with invalid key");
		return it->second;
	}

	iterator begin() noexcept {
		return _tree.begin();
	}

	const_iterator begin() const noexcept {
		return _tree.begin();
	}

	iterator end() noexcept {
		return _tree.end();
	}

	const_iterator end() const noexcept {
		return _tree.end();
	}

	reverse_iterator rbegin() noexcept {
		return _tree.rbegin();
	}

	const_reverse_iterator rbegin() const noexcept {
		return _tree.rbegin();
	}

	reverse_iterator rend() noexcept {
		return _tree.rend();
	}

	const_reverse_iterator rend() const noexcept {
		return _tree.rend();
	}

	const_iterator cbegin() const noexcept {
		return _tree.cbegin();
	}

	const_iterator cend() const noexcept {
		return _tree.cend();
	}

	const_reverse_iterator crbegin() const noexcept {
		return _tree.crbegin();
	}

	const_reverse_iterator crend() const noexcept {
		return _tree.crend();
	}

	size_t size() const noexcept { return _tree.size(); }

	bool empty() const noexcept { return _tree.empty(); }

	bool contains(const Key& key) const { return _tree.contains(); }

	// return end() if not found
	iterator find(const Key& key) {
		return _tree.find(value_type{ key, T() });
	}

	const_iterator find(const Key& key) const {
		return _tree.find(value_type{ key, T() });
	}

	// return end() if out of range
	iterator floor(const Key& key) {
		return _tree.floor(value_type{ key, T() });
	}

	const_iterator floor(const Key& key) const {
		return _tree.floor(value_type{ key, T() });
	}

	// return end() if out of range
	iterator ceiling(const Key& key) {
		return _tree.ceiling(value_type{ key, T() });
	}

	const_iterator ceiling(const T& key) const {
		return _tree.ceiling(value_type{ key, T() });
	}

	// select the kth smallest (greatest) element
	// return end() if out of range
	iterator select(size_t k) {
		return _tree.select(k);
	}

	const_iterator select(size_t k) const {
		return _tree.select(k);
	}

	// number of keys in the subtree less (greater) than key
	size_t rank(const Key& key) const {
		return _tree.rank(value_type{ key, T() });
	}

	// modifiers
	void clear() { _tree.clear(); }

	iterator insert(const value_type& kvpair) {
		return _tree.insert(kvpair);
	}
	
	iterator insert(const Key& key, const T& val) {
		return _tree.insert(value_type{ key,val });
	}

	iterator insert_or_assign(const value_type& kvpair) {
		return _tree.insert_or_assign(kvpair);
	}

	iterator insert_or_assign(const Key& key, const T& val) {
		return _tree.insert_or_assign(value_type{ key,val });
	}

	void erase(const Key& key) {
		_tree.erase(value_type{ key, T() });
	}

	void print(size_t level = 100) const noexcept { _tree.print(level); }
	
private:
	value_compare _comp;
	Tree<value_type, value_compare> _tree;

	class value_compare {
		Compare _key_comp;
	public:
		value_compare() : _key_comp(Compare()) {}
		value_compare(const Compare& comp) : _key_comp(comp) {}

		bool operator()(const value_type& lhs, const value_type& rhs) const {
			return _key_comp(lhs.first, rhs.first);
		}
	};
};

} //namespace mySymbolTable

#endif // !TREEMAP_H
