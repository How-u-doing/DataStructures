/*
 *  ordered symbol tables:
 *  BST set and multiset
 *  see following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/BstSet.h
 */

#ifndef BSTSET_H
#define BSTSET_H 1

#include "BST_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class BstSet : public Tree<Key, Compare, Alloc, /*IsMap=*/false> {
    using _base = Tree<Key, Compare, Alloc, /*IsMap=*/false>;
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
    BstSet() : _base() {}

    // (1) b
    explicit BstSet(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit BstSet(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    BstSet(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    BstSet(InputIt first, InputIt last, const Alloc& alloc)
        : BstSet(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    BstSet(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    BstSet(std::initializer_list<value_type> init, const Alloc& alloc)
        : BstSet(init, Compare(), alloc) {}

    /* IV */

    BstSet(const BstSet& other) : _base(other) {}

    BstSet& operator=(const BstSet& other) {
        _base::operator=(other);
        return *this;
    }

    BstSet& operator=(std::initializer_list<value_type> ilist) {
        BstSet tmp{ ilist };
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

    void swap(BstSet& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class BstSet

template <typename Key, typename Compare, typename Alloc>
void swap( BstSet<Key, Compare, Alloc>& lhs,
           BstSet<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class BstMultiset : public Tree<Key, Compare, Alloc, /*IsMap=*/false> {
    using _base = Tree<Key, Compare, Alloc, /*IsMap=*/false>;
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
    BstMultiset() : _base() {}

    // (1) b
    explicit BstMultiset(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit BstMultiset(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    BstMultiset(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(first, last);
    }

    // (2) b
    template< class InputIt >
    BstMultiset(InputIt first, InputIt last, const Alloc& alloc)
        : BstMultiset(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    BstMultiset(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(init.begin(), init.end());
    }

    // (3) b
    BstMultiset(std::initializer_list<value_type> init, const Alloc& alloc)
        : BstMultiset(init, Compare(), alloc) {}

    /* IV */

    BstMultiset(const BstMultiset& other) : _base(other) {}

    BstMultiset& operator=(const BstMultiset& other) {
        _base::operator=(other);
        return *this;
    }

    BstMultiset& operator=(std::initializer_list<value_type> ilist) {
        BstMultiset tmp{ ilist };
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

    void swap(BstMultiset& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class BstMultiset

template <typename Key, typename Compare, typename Alloc>
void swap(BstMultiset<Key, Compare, Alloc>& lhs,
    BstMultiset<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !BSTSET_H

