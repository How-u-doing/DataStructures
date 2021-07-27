/*
 *  ordered symbol tables:
 *  Red-black BST map and multimap
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/RbMap.h
 */

#ifndef RBMAP_H
#define RBMAP_H 1

#include "RBtree_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <stdexcept>  // std::out_of_range
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T>>>
class RbMap : public RBtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/false> {
    using _base = RBtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/false>;
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
    RbMap() : _base() {}

    // (1) b
    explicit RbMap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit RbMap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    RbMap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    RbMap(InputIt first, InputIt last, const Alloc& alloc)
        : RbMap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    RbMap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    RbMap(std::initializer_list<value_type> init, const Alloc& alloc)
        : RbMap(init, Compare(), alloc) {}

    /* IV */

    RbMap(const RbMap& other) : _base(other) {}

    RbMap& operator=(const RbMap& other) {
        _base::operator=(other);
        return *this;
    }

    RbMap& operator=(std::initializer_list<value_type> ilist) {
        RbMap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* element access */

    T& at(const Key& key) {
        iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("RbMap<K, T> key does not exist");
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("RbMap<K, T> key does not exist");
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

    void swap(RbMap& rhs) {
        _base::swap(rhs);
    }

    /* observers */

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class RbMap

template <typename Key, typename T ,typename Compare, typename Alloc>
void swap(RbMap<Key, T, Compare, Alloc>& lhs,
          RbMap<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T>>>
class RbMultimap : public RBtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/true> {
    using _base = RBtree<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true, /*IsMulti=*/true>;
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
    RbMultimap() : _base() {}

    // (1) b
    explicit RbMultimap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit RbMultimap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    RbMultimap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    RbMultimap(InputIt first, InputIt last, const Alloc& alloc)
        : RbMultimap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    RbMultimap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    RbMultimap(std::initializer_list<value_type> init, const Alloc& alloc)
        : RbMultimap(init, Compare(), alloc) {}

    /* IV */

    RbMultimap(const RbMultimap& other) : _base(other) {}

    RbMultimap& operator=(const RbMultimap& other) {
        _base::operator=(other);
        return *this;
    }

    RbMultimap& operator=(std::initializer_list<value_type> ilist) {
        RbMultimap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* modifiers */

    using _base::insert;

    iterator insert(const Key& key, const T& val) {
        return _base::insert({ key, val });
    }

    void swap(RbMultimap& rhs) {
        _base::swap(rhs);
    }

    /* observers */

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class RbMultimap

template <typename Key, typename T, typename Compare, typename Alloc>
void swap(RbMultimap<Key, T, Compare, Alloc>& lhs,
          RbMultimap<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !RBMAP_H