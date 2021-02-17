/*
 *  ordered symbol tables:
 *  AVL set and multiset
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/AvlSet.h
 */

#ifndef AVLSET_H
#define AVLSET_H 1

#include "AVLtree_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class AvlSet : public AVLtree<Key, Compare, Alloc, /*IsMap=*/false> {
    using _base = AVLtree<Key, Compare, Alloc, /*IsMap=*/false>;
public:
    using key_type = Key;
    using value_type = Key;
    using key_compare = Compare;
    using value_compare = Compare;
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

    /* I */

    // (1) a
    AvlSet() : _base() {}

    // (1) b
    explicit AvlSet(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit AvlSet(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    AvlSet(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    AvlSet(InputIt first, InputIt last, const Alloc& alloc)
        : AvlSet(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    AvlSet(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    AvlSet(std::initializer_list<value_type> init, const Alloc& alloc)
        : AvlSet(init, Compare(), alloc) {}

    /* IV */

    AvlSet(const AvlSet& other) : _base(other) {}

    AvlSet& operator=(const AvlSet& other) {
        _base::operator=(other);
        return *this;
    }

    AvlSet& operator=(std::initializer_list<value_type> ilist) {
        AvlSet tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* unique insertion for set */

    std::pair<iterator, bool> insert(const value_type& val) {
        return _base::insert(val);
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        return _base::insert(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert(ilist.begin(), ilist.end());
    }

    void swap(AvlSet& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class AvlSet

template <typename Key, typename Compare, typename Alloc>
void swap(AvlSet<Key, Compare, Alloc>& lhs,
          AvlSet<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class AvlMultiset : public AVLtree<Key, Compare, Alloc, /*IsMap=*/false> {
    using _base = AVLtree<Key, Compare, Alloc, /*IsMap=*/false>;
public:
    using key_type = Key;
    using value_type = Key;
    using key_compare = Compare;
    using value_compare = Compare;
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

    /* I */

    // (1) a
    AvlMultiset() : _base() {}

    // (1) b
    explicit AvlMultiset(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit AvlMultiset(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    AvlMultiset(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(first, last);
    }

    // (2) b
    template< class InputIt >
    AvlMultiset(InputIt first, InputIt last, const Alloc& alloc)
        : AvlMultiset(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    AvlMultiset(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(init.begin(), init.end());
    }

    // (3) b
    AvlMultiset(std::initializer_list<value_type> init, const Alloc& alloc)
        : AvlMultiset(init, Compare(), alloc) {}

    /* IV */

    AvlMultiset(const AvlMultiset& other) : _base(other) {}

    AvlMultiset& operator=(const AvlMultiset& other) {
        _base::operator=(other);
        return *this;
    }

    AvlMultiset& operator=(std::initializer_list<value_type> ilist) {
        AvlMultiset tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* duplicable insertion for multiset */

    iterator insert(const value_type& val) {
        return _base::insert_multi(val);
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        return _base::insert_multi(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert_multi(ilist.begin(), ilist.end());
    }

    void swap(AvlMultiset& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class AvlMultiset

template <typename Key, typename Compare, typename Alloc>
void swap(AvlMultiset<Key, Compare, Alloc>& lhs,
          AvlMultiset<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !AVLSET_H