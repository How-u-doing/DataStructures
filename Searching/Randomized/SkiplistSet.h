/*
 *  ordered symbol tables:
 *  Skip List set and multiset
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/Randomized/SkiplistSet.h
 */

#ifndef SKIPLISTSET_H
#define SKIPLISTSET_H 1

#include "SkipList_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class SkiplistSet : public SkipList<Key, Compare, Alloc, /*IsMap=*/false> {
    using _base = SkipList<Key, Compare, Alloc, /*IsMap=*/false>;
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
    SkiplistSet() : _base() {}

    // (1) b
    explicit SkiplistSet(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit SkiplistSet(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    SkiplistSet(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_unique(first, last);
    }

    // (2) b
    template< class InputIt >
    SkiplistSet(InputIt first, InputIt last, const Alloc& alloc)
        : SkiplistSet(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    SkiplistSet(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_unique(init.begin(), init.end());
    }

    // (3) b
    SkiplistSet(std::initializer_list<value_type> init, const Alloc& alloc)
        : SkiplistSet(init, Compare(), alloc) {}

    /* IV */

    SkiplistSet(const SkiplistSet& other) : _base(other) {}

    SkiplistSet& operator=(const SkiplistSet& other) {
        _base::operator=(other);
        return *this;
    }

    SkiplistSet& operator=(std::initializer_list<value_type> ilist) {
        SkiplistSet tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* unique insertion for set */

    std::pair<iterator, bool> insert(const value_type& val) {
        return _base::insert_unique(val);
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        return _base::insert_unique(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert_unique(ilist.begin(), ilist.end());
    }

    size_t erase(const key_type& key) {
        return _base::erase_one_top_down(key);
    }

    void swap(SkiplistSet& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class SkiplistSet

template <typename Key, typename Compare, typename Alloc>
void swap(SkiplistSet<Key, Compare, Alloc>& lhs,
          SkiplistSet<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename Compare = std::less<Key>, typename Alloc = std::allocator<Key>>
class SkiplistMultiset : public SkipList<Key, Compare, Alloc, /*IsMap=*/false> {
    using _base = SkipList<Key, Compare, Alloc, /*IsMap=*/false>;
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
    SkiplistMultiset() : _base() {}

    // (1) b
    explicit SkiplistMultiset(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit SkiplistMultiset(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    SkiplistMultiset(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(first, last);
    }

    // (2) b
    template< class InputIt >
    SkiplistMultiset(InputIt first, InputIt last, const Alloc& alloc)
        : SkiplistMultiset(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    SkiplistMultiset(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(init.begin(), init.end());
    }

    // (3) b
    SkiplistMultiset(std::initializer_list<value_type> init, const Alloc& alloc)
        : SkiplistMultiset(init, Compare(), alloc) {}

    /* IV */

    SkiplistMultiset(const SkiplistMultiset& other) : _base(other) {}

    SkiplistMultiset& operator=(const SkiplistMultiset& other) {
        _base::operator=(other);
        return *this;
    }

    SkiplistMultiset& operator=(std::initializer_list<value_type> ilist) {
        SkiplistMultiset tmp{ ilist };
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

    bool erase_one(const key_type& key) {
        return _base::erase_one_top_down(key) == 1;
    }

    void swap(SkiplistMultiset& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class SkiplistMultiset

template <typename Key, typename Compare, typename Alloc>
void swap(SkiplistMultiset<Key, Compare, Alloc>& lhs,
          SkiplistMultiset<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !SKIPLISTSET_H