/*
 *  ordered symbol tables:
 *  BST map and multimap
 *  see following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/BstMap.h
 */

#ifndef BSTMAP_H
#define BSTMAP_H 1

#include "BST_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <stdexcept>  // std::out_of_range
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename T>
struct MyPair {
    using first_type = Key;
    using second_type = T;
    Key first;
    T second;
    friend std::ostream& operator<<(std::ostream& os, const MyPair& mypair) {
        os << '{' << mypair.first << ", " << mypair.second << '}';
        return os;
    }
};

template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<MyPair<const Key, T>>>
class BstMap : public Tree<MyPair<Key, T>, Compare, Alloc, true> {
    using _base = Tree<MyPair<Key, T>, Compare, Alloc, true>;
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = MyPair<const Key, T>;
    using key_compare = Compare;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = typename _base::iterator;
    using const_iterator = typename _base::const_iterator;
    using reverse_iterator = typename _base::reverse_iterator;
    using const_reverse_iterator = typename _base::const_reverse_iterator;
    using node_type = typename _base::node_type;

    class value_compare {
        Compare _key_comp;
    public:
        value_compare() : _key_comp(Compare()) {}
        value_compare(const Compare& comp) : _key_comp(comp) {}

        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return _key_comp(lhs.first, rhs.first);
        }
    };

    /* I */

    // (1) a
    BstMap() : _base() {}

    // (1) b
    explicit BstMap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit BstMap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    BstMap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    BstMap(InputIt first, InputIt last, const Alloc& alloc)
        : BstMap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    BstMap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    BstMap(std::initializer_list<value_type> init, const Alloc& alloc = Alloc())
        : BstMap(init, Compare(), alloc) {}

    /* IV */

    BstMap(const BstMap& other) : _base(other) {}

    BstMap& operator=(const BstMap& other) {
        _base::operator=(other);
        return *this;
    }

    BstMap& operator=(std::initializer_list<value_type> ilist) {
        BstMap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* element access */

    T& at(const Key& key) {
        iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("BstMap<K, T> key does not exist");
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("BstMap<K, T> key does not exist");
        return it->second;
    }

    T& operator[](const Key& key) {
        return _base::insert({ key, T() }).first->second;
    }

    /* unique insertion for map */

    std::pair<iterator, bool> insert(const value_type& val) {
        return _base::insert(val);
    }

    std::pair<iterator, bool> insert(const Key& key, const T& val) {
        return _base::insert({ key, val });
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        return _base::insert(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert(ilist.begin(), ilist.end());
    }

    std::pair<iterator, bool> insert_or_assign(const value_type& val) {
        return _base::insert_or_assign(val);
    }

    std::pair<iterator, bool> insert_or_assign(const Key& key, const T& val) {       
        return _base::insert_or_assign({ key, val });
    }

    void swap(BstMap& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class BstMap

template <typename Key, typename T ,typename Compare, typename Alloc>
void swap( BstMap<MyPair<Key, T>, Compare, Alloc>& lhs,
           BstMap<MyPair<Key, T>, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<MyPair<const Key, T>>>
class BstMultimap : public Tree<MyPair<Key, T>, Compare, Alloc, true> {
    using _base = Tree<MyPair<Key, T>, Compare, Alloc, true>;
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = MyPair<const Key, T>;
    using key_compare = Compare;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = typename _base::iterator;
    using const_iterator = typename _base::const_iterator;
    using reverse_iterator = typename _base::reverse_iterator;
    using const_reverse_iterator = typename _base::const_reverse_iterator;
    using node_type = typename _base::node_type;

    class value_compare {
        Compare _key_comp;
    public:
        value_compare() : _key_comp(Compare()) {}
        value_compare(const Compare& comp) : _key_comp(comp) {}

        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return _key_comp(lhs.first, rhs.first);
        }
    };

    /* I */

    // (1) a
    BstMultimap() : _base() {}

    // (1) b
    explicit BstMultimap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit BstMultimap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    BstMultimap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_equal(first, last);
    }

    // (2) b
    template< class InputIt >
    BstMultimap(InputIt first, InputIt last, const Alloc& alloc)
        : BstMultimap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    BstMultimap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_equal(init.begin(), init.end());
    }

    // (3) b
    BstMultimap(std::initializer_list<value_type> init, const Alloc& alloc = Alloc())
        : BstMultimap(init, Compare(), alloc) {}

    /* IV */

    BstMultimap(const BstMultimap& other) : _base(other) {}

    BstMultimap& operator=(const BstMultimap& other) {
        _base::operator=(other);
        return *this;
    }

    BstMultimap& operator=(std::initializer_list<value_type> ilist) {
        BstMultimap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* duplicable insertion for multimap */

    iterator insert(const value_type& val) {
        return _base::insert_equal(val);
    }

    iterator insert(const Key& key, const T& val) {
        return _base::insert_equal({ key, val });
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        return _base::insert_equal(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert_equal(ilist.begin(), ilist.end());
    }

    void swap(BstMultimap& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class BstMultimap

template <typename Key, typename T, typename Compare, typename Alloc>
void swap(BstMultimap<MyPair<Key, T>, Compare, Alloc>& lhs,
          BstMultimap<MyPair<Key, T>, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !BSTMAP_H

