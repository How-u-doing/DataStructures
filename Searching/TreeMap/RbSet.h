/*
 *  ordered symbol tables:
 *  Red-black BST set and multiset
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/RbSet.h
 */

#ifndef RBSET_H
#define RBSET_H 1

#include "RBtree_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class RbSet : public RBtree<Key, Compare, Alloc, /*IsMap=*/false, /*IsMulti=*/false> {
    using _base = RBtree<Key, Compare, Alloc, /*IsMap=*/false, /*IsMulti=*/false>;
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
    RbSet() : _base() {}

    // (1) b
    explicit RbSet(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit RbSet(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    RbSet(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    RbSet(InputIt first, InputIt last, const Alloc& alloc)
        : RbSet(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    RbSet(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    RbSet(std::initializer_list<value_type> init, const Alloc& alloc)
        : RbSet(init, Compare(), alloc) {}

    /* IV */

    RbSet(const RbSet& other) : _base(other) {}

    RbSet& operator=(const RbSet& other) {
        _base::operator=(other);
        return *this;
    }

    RbSet& operator=(std::initializer_list<value_type> ilist) {
        RbSet tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* modifiers */

    void swap(RbSet& rhs) {
        _base::swap(rhs);
    }

    /* observers */

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class RbSet

template <typename Key, typename Compare, typename Alloc>
void swap(RbSet<Key, Compare, Alloc>& lhs,
          RbSet<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class RbMultiset : public RBtree<Key, Compare, Alloc, /*IsMap=*/false, /*IsMulti=*/true> {
    using _base = RBtree<Key, Compare, Alloc, /*IsMap=*/false, /*IsMulti=*/true>;
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
    RbMultiset() : _base() {}

    // (1) b
    explicit RbMultiset(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit RbMultiset(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    RbMultiset(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< class InputIt >
    RbMultiset(InputIt first, InputIt last, const Alloc& alloc)
        : RbMultiset(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    RbMultiset(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (3) b
    RbMultiset(std::initializer_list<value_type> init, const Alloc& alloc)
        : RbMultiset(init, Compare(), alloc) {}

    /* IV */

    RbMultiset(const RbMultiset& other) : _base(other) {}

    RbMultiset& operator=(const RbMultiset& other) {
        _base::operator=(other);
        return *this;
    }

    RbMultiset& operator=(std::initializer_list<value_type> ilist) {
        RbMultiset tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* modifiers */

    void swap(RbMultiset& rhs) {
        _base::swap(rhs);
    }

    /* observers */

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class RbMultiset

template <typename Key, typename Compare, typename Alloc>
void swap(RbMultiset<Key, Compare, Alloc>& lhs,
          RbMultiset<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !RBSET_H