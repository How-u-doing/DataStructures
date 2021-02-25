/*
 *  ordered symbol tables:
 *  AVL map and multimap
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/AvlMap.h
 */

#ifndef AVLMAP_H
#define AVLMAP_H 1

#include "AVLtree_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <stdexcept>  // std::out_of_range
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T>>>
class AvlMap : public AVLtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/false> {
    using _base = AVLtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/false>;
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
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
    AvlMap() : _base() {}

    // (1) b
    explicit AvlMap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit AvlMap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    AvlMap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    AvlMap(InputIt first, InputIt last, const Alloc& alloc)
        : AvlMap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    AvlMap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    AvlMap(std::initializer_list<value_type> init, const Alloc& alloc)
        : AvlMap(init, Compare(), alloc) {}

    /* IV */

    AvlMap(const AvlMap& other) : _base(other) {}

    AvlMap& operator=(const AvlMap& other) {
        _base::operator=(other);
        return *this;
    }

    AvlMap& operator=(std::initializer_list<value_type> ilist) {
        AvlMap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* element access */

    T& at(const Key& key) {
        iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("AvlMap<K, T> key does not exist");
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("AvlMap<K, T> key does not exist");
        return it->second;
    }

    T& operator[](const Key& key) {
#if 0
        // if T() is cheap, we can just simply do this
        return _base::insert({ key, T() }).first->second;
#elif 0
        // otherwise we can do somthing like this
        iterator it = this->lower_bound(key); // it->first >= key
        if (it == this->end() || key_comp()(key, it->first))
            it = _base::insert(it, { key, T() });
        return it->second;
#else
        // but we have an even cooler one
        auto p = this->cool_lower_bound(key); // p.it->first >= key
        iterator it = p.it;
        if (it == this->end() || key_comp()(key, it->first))
            it = _base::insert_leaf_at(p.x, { key, T() }, p.x_parent);
        return it->second;
#endif
    }

    /* modifiers */

    using _base::insert;

    std::pair<iterator, bool> insert(const Key& key, const T& val) {
        return _base::insert({ key, val });
    }

    std::pair<iterator, bool> insert_or_assign(const value_type& val) {
        return _base::insert_or_assign(val);
    }

    std::pair<iterator, bool> insert_or_assign(const Key& key, const T& val) {       
        return _base::insert_or_assign({ key, val });
    }

    void swap(AvlMap& rhs) {
        _base::swap(rhs);
    }

    /* observers */

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class AvlMap

template <typename Key, typename T ,typename Compare, typename Alloc>
void swap(AvlMap<Key, T, Compare, Alloc>& lhs,
          AvlMap<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T>>>
class AvlMultimap : public AVLtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/true> {
    using _base = AVLtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/true>;
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
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
    AvlMultimap() : _base() {}

    // (1) b
    explicit AvlMultimap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit AvlMultimap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    AvlMultimap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    AvlMultimap(InputIt first, InputIt last, const Alloc& alloc)
        : AvlMultimap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    AvlMultimap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    AvlMultimap(std::initializer_list<value_type> init, const Alloc& alloc)
        : AvlMultimap(init, Compare(), alloc) {}

    /* IV */

    AvlMultimap(const AvlMultimap& other) : _base(other) {}

    AvlMultimap& operator=(const AvlMultimap& other) {
        _base::operator=(other);
        return *this;
    }

    AvlMultimap& operator=(std::initializer_list<value_type> ilist) {
        AvlMultimap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* modifiers */

    using _base::insert;

    iterator insert(const Key& key, const T& val) {
        return _base::insert({ key, val });
    }

    void swap(AvlMultimap& rhs) {
        _base::swap(rhs);
    }

    /* observers */

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class AvlMultimap

template <typename Key, typename T, typename Compare, typename Alloc>
void swap(AvlMultimap<Key, T, Compare, Alloc>& lhs,
          AvlMultimap<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !AVLMAP_H