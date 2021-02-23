/*
 *  internal header file for implementing
 *  ordered symbol tables:
 *  Skip List (multi) map/set
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/Randomized/SkipList_impl.h
 */

#ifndef SKIPLIST_IMPL_H
#define SKIPLIST_IMPL_H 1

#include <memory>   // std::addressof, std::allocator_tarits
#include <utility>  // std::swap, std::pair
#include <iterator> // std::reverse_iterator, std::distance
#include <vector>
#include <random>
#include <cassert>
#include "my_map_traits.h"  // myst::get_map_key_t

namespace mySymbolTable {

constexpr int SkipListMaxLevel = 32; // the probability to reach this is 1/(2^32), almost ZERO!

template<typename T, typename Compare, typename Alloc, bool IsMap>
class SkipList {
    struct SkipList_node;
    class SkipList_iter;
    class SkipList_const_iter;
    using _self = SkipList<T, Compare, Alloc, IsMap>;
    using node = SkipList_node;
    using node_ptr = node*;
    using NodeAl = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;
public:
    using value_type = T;
    using key_type = typename get_map_key_t<T, IsMap>::key_type;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = SkipList_iter;
    using const_iterator = SkipList_const_iter;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = SkipList_node;

private:
    node_ptr _header;
    size_t   _count = 0;
    Compare  _comp{};
    NodeAl   _alloc{};

    void set_default_header(bool init) noexcept {
        _header->_level = 0;  // 0 suggests the skip list's empty
        _header->_prev = _header;
        if (init) {
            _header->_next = (node**) ::operator new(SkipListMaxLevel * sizeof(node_ptr));
        }
        for (int i = 0; i < SkipListMaxLevel; ++i) {
            _header->_next[i] = _header;
        }
    }

    // create & initialize the header node
    void create_header() {
        _header = _alloc.allocate(1); // may throw
        set_default_header(/*init=*/true);
        // leave the value field uninitialized
    }

public:
    SkipList() { create_header(); }

    explicit SkipList(const Compare& comp, const Alloc& alloc = Alloc())
        : _comp(comp), _alloc(alloc) { create_header(); }

    explicit SkipList(const Alloc& alloc) : _alloc(alloc) { create_header(); }

    // note that their levels of each node with same value (thus max level) can differ
    SkipList(const _self& rhs) : _comp(rhs._comp), _alloc(rhs._alloc)
    {
        create_header();
        copy_nodes_from(rhs);
    }

    ~SkipList() {
        clear_nodes();
        ::operator delete(_header->_next);
        _alloc.deallocate(_header, 1);
    }

    _self& operator=(const _self& rhs) {
        if (this == &rhs) return *this;
        clear();
        copy_nodes_from(rhs);
        return *this;
    }

    allocator_type get_allocator() const noexcept {
        return _alloc;
    }

    /* iterators */

    iterator begin() noexcept {
        return iterator(_header->_next[0]);
    }

    const_iterator begin() const noexcept {
        return const_iterator(_header->_next[0]);
    }

    iterator end() noexcept {
        return iterator(_header);
    }

    const_iterator end() const noexcept {
        return const_iterator(_header);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    /* capacity */

    bool empty() const noexcept { return _count == 0; }

    size_t size() const noexcept { return _count; }

    size_t max_size() const noexcept {
        return std::allocator_traits<NodeAl>::max_size(_alloc);
    }

    /* level interface */

    int level() const noexcept { return _header->_level; }

    int level(const_iterator pos) const noexcept { return pos.ptr()->_level; }

    /* lookup */

    size_t count(const key_type& key) const {
        auto r = equal_range(key);
        return std::distance(r.first, r.second);
    }

    iterator find(const key_type& key) {
        return iterator(find_aux(key));
    }
    
    const_iterator find(const key_type& key) const {
        return const_iterator(find_aux(key));
    }

    bool contains(const key_type& key) const {
        return find_aux(key) != _header;
    }

    iterator lower_bound(const key_type& key) {
        return iterator(find_aux(key, /*lower_bound=*/true));
    }

    const_iterator lower_bound(const key_type& key) const {
        return const_iterator(find_aux(key, /*lower_bound=*/true));
    }

    iterator upper_bound(const key_type& key) {
        return iterator(upper_bound_aux(key));
    }
    
    const_iterator upper_bound(const key_type& key) const {
        return const_iterator(upper_bound_aux(key));
    }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
        return { lower_bound(key), upper_bound(key) };
    }
    
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        return { lower_bound(key), upper_bound(key) };
    }

    /* modifiers */

    void clear() noexcept {
        if (!empty()) {
            clear_nodes(); _count = 0;
            set_default_header(/*init=*/false);
        }
    }

protected:
    // for non-duplicate hash map (set)
    template< typename InputIt >
    void insert_unique(InputIt first, InputIt last) {
        while (first != last) {
            insert_unique(*first++);
        }
    }

    // for hash multi map (set)
    template< typename InputIt >
    void insert_multi(InputIt first, InputIt last) {
        while (first != last) {
            insert_multi(*first++);
        }
    }

public:
    iterator erase(iterator pos) {
        return iterator(erase_bottom_up(pos.ptr()));
    }

    iterator erase(const_iterator pos) {
        return iterator(erase_bottom_up(pos.ptr()));
    }

    iterator erase(const_iterator first, const_iterator last) {
        // quick erasing
        if (first == begin() && first == end()) {
            clear();
            return end();
        }
        while (first != last) {
            first = erase(first);
        }
        return iterator(first.ptr());
    }

    size_t erase(const key_type& key) {
        auto r = equal_range(key);
        size_t n = std::distance(r.first, r.second);
        erase(r.first, r.second);
        return n;
    }

    void swap(SkipList& rhs) noexcept(std::allocator_traits<Alloc>::is_always_equal::value
                               &&     std::is_nothrow_swappable<Compare>::value)
    {
        assert(_alloc == rhs._alloc && "allocator must be the same");
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_alloc, rhs._alloc);
        } // otherwise the behavior is undefined

        std::swap(_header, rhs._header);
        std::swap(_count, rhs._count);
        std::swap(_comp, rhs._comp);
    }
    
private:
    node_ptr new_node(const T& val, int level, node_ptr prev, node_ptr update[])
    {
        node_ptr p = _alloc.allocate(1);
        try {
            ::new ((void*)p) node(val, level, prev, update);
        }
        catch (...) {
            _alloc.deallocate(p, 1);
            throw;
        }
        return p;
    }

    void delete_node(node_ptr x) noexcept {
        x->~node(); // don't forget
        _alloc.deallocate(x, 1);
    }

    void copy_nodes_from(const _self& rhs) {
        for (node_ptr x = rhs._header->_next[0]; x != rhs._header; x = x->_next[0]) {
            insert_multi(x->_val);
        }
    }

    void clear_nodes() noexcept {
        for (node_ptr x = _header->_next[0], del; x != _header;) {
            del = x; x = x->_next[0];
            delete_node(del);
        }
    }

    // find the first key that compares equivalent to `key`
    node_ptr find_aux(const key_type& key, bool lower_bound = false) const {
        if (empty()) return _header;
        node_ptr curr = _header, next;
        for (int level = _header->_level - 1; level >= 0; --level) {
            while ((next = curr->_next[level]) != _header && _comp(get_key(next), key))
            {
                curr = next;
            }
        }
        curr = curr->_next[0];
        if (curr == _header) return _header; // not found, key too large
        if (!_comp(key, get_key(curr))/* && !_comp(get_key(curr), key)*/)
            return curr; // found
        else // not found: curr->_prev < key < curr
            return lower_bound ? curr : _header;
    }

#if 0
    // lazy version
    node_ptr upper_bound_aux(const key_type& key) const {
        node_ptr curr = find_aux(key, /*lower_bound=*/true);
        while (curr != _header && !_comp(key, get_key(curr)))
            curr = curr->_next[0];
        return curr;
    }
#endif
    // eager version
    node_ptr upper_bound_aux(const key_type& key) const {
        if (empty()) return _header;
        node_ptr curr = _header, next = _header;
        for (int level = _header->_level - 1; level >= 0; --level) {
            while ((next = curr->_next[level]) != _header && !_comp(key, get_key(next)))
            {
                curr = next;
            }
        }
        // now curr <= key < next or next == _header
        return next;
    }

    // map
    static const key_type& get_key_via_ptr(node_ptr x, std::true_type) {
        return (x->_val).first;
    }

    // set
    static const key_type& get_key_via_ptr(node_ptr x, std::false_type) {
        return x->_val;
    }

    static const key_type& get_key(node_ptr x) {
        return get_key_via_ptr(x, std::bool_constant<IsMap>{});
    }

    // map
    static const key_type& get_key_via_t(const T& val, std::true_type) {
        return val.first;
    }

    // set
    static const key_type& get_key_via_t(const T& val, std::false_type) {
        return val;
    }

    static const key_type& get_key(const T& val) {
        return get_key_via_t(val, std::bool_constant<IsMap>{});
    }

    static bool coin_flip_is_heads() {
        // c++ version
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 1);
        return distrib(gen) == 1;
#if 0
        // In c, we can do in this way (see below), but here's the problem:
        // every time we start the program we always get the same random
        // numbers so all class instances (can be of different types as we
        // change the template arguments) have the same sequence of random
        // numbers thus their levels are the same, which might be a little
        // undesirable (but on the other side for one single instance this
        // property makes it very stable, always getting the same levels
        // when inserting same values, which's good)??
        // Note that we cannot add `srand(time(NULL))` here, since it will
        // render the results almost always identical during insertion
        // because the computer's executing the instructions way too fast,
        // and each time this subroutine is getting invoked, current time
        // ( `time(NULL)` ) remains unchanged thus we always getting heads
        // or tails which in turn leds to identical levels (either 1 or 32)
        // for all nodes. A possible workaround is to put srand(time(NULL))
        // in constructor so that the random levels for nodes can be
        // instance specific.

        int n = rand() / (RAND_MAX + 1.0) * 2; // rand() % 2 is BAD, see below
        return n == 1;
        /* generate a random number from range [min, max]:
         * random_num = min + (int) (rand() / (RAND_MAX + 1.0) * (max - min + 1))
         * NOTE that `min+rand()%(max-min+1)` is biased towards small numbers, for example,
         * RAND_MAX == 4, and we want a number from range [0, 2], then
         *    rand()  |  rand() % 3
         * -----------+-------------
         *      0     |     0
         *      1     |     1
         *      2     |     2
         *      3     |     0
         */
#endif
    }

    static int random_level() {
        int level = 1;
        while (coin_flip_is_heads() && level < SkipListMaxLevel)
            ++level;
        return level;
    }

    node_ptr insert_after(node_ptr x, const T& val, node_ptr update[]) {
        int level = random_level();
        if (level > _header->_level) {
            for (int i = _header->_level; i < level; ++i)
                update[i] = _header;
            _header->_level = level;
        }
        node_ptr newnode = new_node(val, level, x, update);
        ++_count;
        return newnode;
    }

protected:
    // only for map
    std::pair<iterator, bool> insert_or_assign(const T& val) {
        const key_type& key = get_key(val);
        node_ptr update[SkipListMaxLevel];
        node_ptr curr = _header, next;
        for (int level = _header->_level - 1; level >= 0; --level) {
            while ((next = curr->_next[level]) != _header && _comp(get_key(next), key))
                curr = next;
            update[level] = curr;
        }
        next = curr->_next[0];
        if (next != _header && !_comp(key, get_key(next))/* && !_comp(get_key(next), key)*/) {
            next->_val.second = val.second;
            return { next, false };
        }
        else return { insert_after(curr, val, update), true };
    }

    std::pair<iterator, bool> insert_unique(const T& val) {
        const key_type& key = get_key(val);
        node_ptr update[SkipListMaxLevel];
        node_ptr curr = _header, next;
        for (int level = _header->_level - 1; level >= 0; --level) {
            while ((next = curr->_next[level]) != _header && _comp(get_key(next), key))
                curr = next;
            update[level] = curr;
        }
        next = curr->_next[0];
        if (next != _header && !_comp(key, get_key(next))/* && !_comp(get_key(next), key)*/) {
            return { next, false };
        }
        else return { insert_after(curr, val, update), true };
    }

    iterator insert_multi(const T& val) {
        const key_type& key = get_key(val);
        node_ptr update[SkipListMaxLevel];
        node_ptr curr = _header, next;
        for (int level = _header->_level - 1; level >= 0; --level) {
            while ((next = curr->_next[level]) != _header && _comp(get_key(next), key))
                curr = next;
            update[level] = curr;
        }
        return insert_after(curr, val, update);
    }

    iterator insert_before_bottom_up(node_ptr x, const T& val) {
        int level = random_level();
        node_ptr newnode = new_node(val, level, x->_prev, nullptr);
        newnode->_next[0] = x;
        x->_prev->_next[0] = newnode;
        x->_prev = newnode;
        node_ptr curr = newnode->_prev;
        for (int i = 1; i < level; ++i) {
            while (curr != _header && curr->_level <= i)
                curr = curr->_prev;
            newnode->_next[i] = curr->_next[i];
            curr->_next[i] = newnode;
        }
        if (level > _header->_level) _header->_level = level;
        ++_count;
        return newnode;
    }

    // quick erasure for non-duplicate map/set or for
    // multi map/set when only wanting to delete one key
    size_t erase_one_top_down(const key_type& key) {
        node_ptr update[SkipListMaxLevel];
        node_ptr curr = _header, next;
        for (int level = _header->_level - 1; level >= 0; --level) {
            while ((next = curr->_next[level]) != _header && _comp(get_key(next), key))
                curr = next;
            update[level] = curr;
        }
        curr = curr->_next[0];
        if (curr == _header || _comp(key, get_key(curr))) {
            return 0;
        }
        curr->_next[0]->_prev = curr->_prev;
        for (int i = 0; i < curr->_level; ++i) {
            update[i]->_next[i] = curr->_next[i];
        }
        delete_node(curr); --_count;
        for (int i = _header->_level - 1; i >= 0 && _header->_next[i] == _header; --i)
            --_header->_level;
        return 1;
    }

private:
    node_ptr erase_bottom_up(node_ptr x) {
        assert(x != _header && "cannot erase end() iterator");
        node_ptr x_next = x->_next[0];
        x->_prev->_next[0] = x_next;
        x_next->_prev = x->_prev;
        // move left then up, left up, ...
        // until reach the first node with the same level
        int level = x->_level;
        node_ptr curr = x->_prev;
        for (int i = 1; i < level; ++i) {
            while (curr->_level <= i)
                curr = curr->_prev;
            curr->_next[i] = x->_next[i];
        }
        delete_node(x); --_count;
        for (int i = _header->_level - 1; i >= 0 && _header->_next[i] == _header; --i)
            --_header->_level;
        return x_next;
    }

    struct SkipList_node {
        T _val;
        int      _level;
        node_ptr _prev;
        node**   _next;

        SkipList_node(const T& val, int level, node_ptr prev, node_ptr update[])
            : _val(val), _level(level), _prev(prev)
        {
            _next = (node**) ::operator new(level * sizeof(node_ptr));
            if (update) {
                for (int i = 0; i < level; ++i) {
                    _next[i] = update[i]->_next[i];
                    update[i]->_next[i] = this;
                }
                _next[0]->_prev = this;
            }
        }

        ~SkipList_node() { ::operator delete(_next); }

        // in case operator& is overloaded
        T* val_ptr() {
            return std::addressof(_val); // return &_val;
        }
        const T* val_ptr() const {
            return std::addressof(_val);
        }
    };

    class SkipList_iter
    {
        using _self = SkipList_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        SkipList_iter() noexcept : _ptr(nullptr) {}
        SkipList_iter(node_ptr ptr) noexcept : _ptr(ptr) {}

        node_ptr ptr() const noexcept { return _ptr; }
        int level() const noexcept { return _ptr->_level; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            _ptr = _ptr->_next[0];
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _ptr = _ptr->_next[0];
            return tmp;
        }

        _self& operator--() {
            _ptr = _ptr->_prev;
            return *this;
        }

        _self operator--(int) {
            _self tmp{ *this };
            _ptr = _ptr->_prev;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };

    class SkipList_const_iter
    {
        using _self = SkipList_const_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        SkipList_const_iter() : _ptr(nullptr) {}
        SkipList_const_iter(node_ptr ptr) : _ptr(ptr) {}
        SkipList_const_iter(const SkipList_iter& other) : _ptr(other.ptr()) {}

        node_ptr ptr() const noexcept { return _ptr; }
        int level() const noexcept { return _ptr->_level; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            _ptr = _ptr->_next[0];
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _ptr = _ptr->_next[0];
            return tmp;
        }

        _self& operator--() {
            _ptr = _ptr->_prev;
            return *this;
        }

        _self operator--(int) {
            _self tmp{ *this };
            _ptr = _ptr->_prev;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };
}; // class SkipList

} // namespace mySymbolTable

#endif // !SKIPLIST_IMPL_H