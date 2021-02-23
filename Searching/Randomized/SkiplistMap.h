/*
 *  ordered symbol tables:
 *  Skip List map and multimap
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/Randomized/SkiplistMap.h
 */

#ifndef SKIPLISTMAP_H
#define SKIPLISTMAP_H 1

#include "SkipList_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::less
#include <stdexcept>  // std::out_of_range
#include <initializer_list>

namespace mySymbolTable {

template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T>>>
class SkiplistMap : public SkipList<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true> {
    using _base = SkipList<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true>;
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
    SkiplistMap() : _base() {}

    // (1) b
    explicit SkiplistMap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit SkiplistMap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    SkiplistMap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_unique(first, last);
    }

    // (2) b
    template< class InputIt >
    SkiplistMap(InputIt first, InputIt last, const Alloc& alloc)
        : SkiplistMap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    SkiplistMap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_unique(init.begin(), init.end());
    }

    // (3) b
    SkiplistMap(std::initializer_list<value_type> init, const Alloc& alloc)
        : SkiplistMap(init, Compare(), alloc) {}

    /* IV */

    SkiplistMap(const SkiplistMap& other) : _base(other) {}

    SkiplistMap& operator=(const SkiplistMap& other) {
        _base::operator=(other);
        return *this;
    }

    SkiplistMap& operator=(std::initializer_list<value_type> ilist) {
        SkiplistMap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* element access */

    T& at(const Key& key) {
        iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("SkiplistMap<K, T> key does not exist");
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("SkiplistMap<K, T> key does not exist");
        return it->second;
    }

    T& operator[](const Key& key) {
        iterator it = this->lower_bound(key); // it->first >= key
        if (it == this->end() || key_comp()(key, it->first))
            it = _base::insert_before_bottom_up(it.ptr(), { key, T() });
        return it->second;
    }

    /* unique insertion for map */

    std::pair<iterator, bool> insert(const value_type& val) {
        return _base::insert_unique(val);
    }

    std::pair<iterator, bool> insert(const Key& key, const T& val) {
        return _base::insert_unique({ key, val });
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        return _base::insert_unique(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert_unique(ilist.begin(), ilist.end());
    }

    std::pair<iterator, bool> insert_or_assign(const value_type& val) {
        return _base::insert_or_assign(val);
    }

    std::pair<iterator, bool> insert_or_assign(const Key& key, const T& val) {       
        return _base::insert_or_assign({ key, val });
    }

    size_t erase(const key_type& key) {
        return _base::erase_one_top_down(key);
    }

    void swap(SkiplistMap& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class SkiplistMap

template <typename Key, typename T ,typename Compare, typename Alloc>
void swap(SkiplistMap<Key, T, Compare, Alloc>& lhs,
          SkiplistMap<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T>>>
class SkiplistMultimap : public SkipList<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true> {
    using _base = SkipList<std::pair<const Key, T>, Compare, Alloc, /*IsMap=*/true>;
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
    SkiplistMultimap() : _base() {}

    // (1) b
    explicit SkiplistMultimap(const Compare& comp, const Alloc& alloc = Alloc())
        : _base(comp, alloc) {}

    // (1) c
    explicit SkiplistMultimap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< class InputIt >
    SkiplistMultimap(InputIt first, InputIt last, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(first, last);
    }

    // (2) b
    template< class InputIt >
    SkiplistMultimap(InputIt first, InputIt last, const Alloc& alloc)
        : SkiplistMultimap(first, last, Compare(), alloc) {}

    /* III */

    // (3) a
    SkiplistMultimap(std::initializer_list<value_type> init, const Compare& comp = Compare(),
        const Alloc& alloc = Alloc()) : _base(comp, alloc)
    {
        _base::insert_multi(init.begin(), init.end());
    }

    // (3) b
    SkiplistMultimap(std::initializer_list<value_type> init, const Alloc& alloc)
        : SkiplistMultimap(init, Compare(), alloc) {}

    /* IV */

    SkiplistMultimap(const SkiplistMultimap& other) : _base(other) {}

    SkiplistMultimap& operator=(const SkiplistMultimap& other) {
        _base::operator=(other);
        return *this;
    }

    SkiplistMultimap& operator=(std::initializer_list<value_type> ilist) {
        SkiplistMultimap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* duplicable insertion for multimap */

    iterator insert(const value_type& val) {
        return _base::insert_multi(val);
    }

    iterator insert(const Key& key, const T& val) {
        return _base::insert_multi({ key, val });
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

    void swap(SkiplistMultimap& rhs) {
        _base::swap(rhs);
    }

    key_compare key_comp() const {
        return key_compare{};
    }

    value_compare value_comp() const {
        return value_compare{};
    }

}; // class SkiplistMultimap

template <typename Key, typename T, typename Compare, typename Alloc>
void swap(SkiplistMultimap<Key, T, Compare, Alloc>& lhs,
          SkiplistMultimap<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !SKIPLISTMAP_H