/*
 *  internal header file for implementing
 *  unordered symbol tables:
 *  Hash (multi) map/set
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/HashMap/alternative/Hashtable2_impl.h
 */

#ifndef HASHTABLE2_IMPL_H
#define HASHTABLE2_IMPL_H 1

#include <utility>  // std::pair, std::swap
#include <iterator> // std::distance
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>    // std::ceil
#include <cassert>
#include "../my_map_traits.h"  // myst::get_map_key_t

namespace mySymbolTable {
namespace alternative {

// Implementation of a canonical hashtable by separate chaining.
// This version has null links at the tail of each bucket instead of linking
// them to the head nodes of next non-empty buckets. This attribute may lead
// to faster insertion and deletion, but it makes iterators become fatter:
// we need an addiontial ownership hashtable pointer in each iterator and
// optionally a bucket index integer which can help us more easily to find
// its successor.
template<typename T, typename Hash, typename KeyEqual, typename Alloc, bool IsMap>
class Hashtable {
    struct Hash_node;
    class Hash_iter;
    class Hash_const_iter;
    class Hash_local_iter;
    class Hash_const_local_iter;
    using _self = Hashtable<T, Hash, KeyEqual, Alloc, IsMap>;
    using node = Hash_node;
    using node_ptr = node*;
    using NodeAl = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;
public:
    using value_type = T;
    using key_type = typename get_map_key_t<T, IsMap>::key_type;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = Hash_iter;
    using const_iterator = Hash_const_iter;
    using local_iterator = Hash_local_iter;
    using const_local_iterator = Hash_const_local_iter;
    using node_type = Hash_node;

private:
    float     _mlf = 1.f; // max load factor
    size_t    _count = 0;
    Hash      _hash;
    KeyEqual  _keyeq;
    NodeAl    _alloc;
    std::vector<node_ptr> _hashtable;

public:

    Hashtable() : _hashtable(2) {}

    Hashtable( size_t bucket_count,
               const Hash& hash,
               const key_equal& equal,
               const Alloc& alloc )
        : _hash(hash), _keyeq(equal), _alloc(alloc), _hashtable(next_prime(bucket_count)) {}

    explicit Hashtable(const Alloc& alloc) : _alloc(alloc), _hashtable(2) {}

    Hashtable(const _self& rhs) : _mlf(rhs._mlf), _hash(rhs._hash), _keyeq(rhs._keyeq),
                                  _alloc(rhs._alloc), _hashtable(rhs._hashtable.size())
    {
        copy_nodes(rhs);
    }

    Hashtable(const _self& rhs, const Alloc& alloc) : _mlf(rhs._mlf), _hash(rhs._hash),
        _keyeq(rhs._keyeq), _alloc(alloc), _hashtable(rhs._hashtable.size())
    {
        copy_nodes(rhs);
    }

    ~Hashtable() { clear_nodes(); }

    _self& operator=(const _self& rhs) {
        if (this == &rhs) return *this;
        _self tmp{ rhs };
        swap(tmp);
        return *this;
    }

    allocator_type get_allocator() const noexcept {
        return _alloc;
    }

    /* iterators */

    iterator begin() noexcept {
        for (size_t k = 0; k < _hashtable.size(); ++k) {
            if (_hashtable[k]) return iterator(_hashtable[k], this, k);
        }
        return end();
    }

    const_iterator begin() const noexcept {
        for (size_t k = 0; k < _hashtable.size(); ++k) {
            if (_hashtable[k]) return const_iterator(_hashtable[k], this, k);
        }
        return end();
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    iterator end() noexcept {
        return iterator(nullptr, this, _hashtable.size()); // the bucket index is meaningless
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr, this, _hashtable.size());
    }

    const_iterator cend() const noexcept {
        return end();
    }

    /* capacity */

    bool empty() const noexcept { return _count == 0; }

    size_t size() const noexcept { return _count; }

    size_t max_size() const noexcept {
        return std::allocator_traits<NodeAl>::max_size(_alloc);
    }

    /* modifiers */

    void clear() noexcept {
        clear_nodes(); _count = 0;
        for (auto& head : _hashtable)
            head = nullptr;
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
        return erase_aux(pos.ptr());
    }

    iterator erase(const_iterator pos) {
        return erase_aux(pos.ptr());
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
        return iterator(first.ptr(), this, first.bucket());
    }

    size_t erase(const key_type& key) {
        auto r = equal_range(key);
        size_t n = std::distance(r.first, r.second);
        erase(r.first, r.second);
        return n;
    }

    void swap(Hashtable& rhs) noexcept(std::allocator_traits<Alloc>::is_always_equal::value
                                &&     std::is_nothrow_swappable<Hash>::value
                                &&     std::is_nothrow_swappable<key_equal>::value)
    {
        assert(_alloc == rhs._alloc && "allocator must be the same");
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_alloc, rhs._alloc);
        } // otherwise the behavior is undefined

        std::swap(_mlf,       rhs._mlf);
        std::swap(_count,     rhs._count);
        std::swap(_hash,      rhs._hash);
        std::swap(_keyeq,     rhs._keyeq);
        std::swap(_hashtable, rhs._hashtable);
    }

    /* lookup */

    size_t count(const key_type& key) const {
        auto r = equal_range(key);
        return std::distance(r.first, r.second);
    }

    iterator find(const key_type& key) {
        return find_aux(key);
    }

    const_iterator find(const key_type& key) const {
        return find_aux(key);
    }

    bool contains(const key_type& key) const {
        return find_aux(key).ptr() != nullptr;
    }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
        return equal_range_aux(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        return equal_range_aux(key);
    }

    /* bucket interface */

    local_iterator begin(size_t n) {
        assert(n < _hashtable.size());
        return local_iterator(_hashtable[n], this, n);
    }

    const_local_iterator begin(size_t n) const {
        assert(n < _hashtable.size());
        return const_local_iterator(_hashtable[n], this, n);
    }

    const_local_iterator cbegin(size_t n) const {
        return begin(n);
    }

    local_iterator end(size_t n) {
        assert(n < _hashtable.size());
        return local_iterator(nullptr, this, n);
    }

    const_local_iterator end(size_t n) const {
        assert(n < _hashtable.size());
        return const_local_iterator(nullptr, this, n);
    }

    const_local_iterator cend(size_t n) const {
        return end(n);
    }

    size_t bucket_count() const {
        return _hashtable.size();
    }

    size_t max_bucket_count() const {
        return _hashtable.max_size();
    }

    size_t bucket_size(size_t n) const {
        return std::distance(begin(n), end(n));
    }

    size_t bucket(const key_type& key) const {
        return _hash(key) % bucket_count();
    }

    /* hash policy */

    float load_factor() const {
        return static_cast<float>(size()) / bucket_count();
    }
    
    float max_load_factor() const {
        return _mlf;
    }

    void max_load_factor(float mlf) {
        if (mlf <= 0) return;
        _mlf = mlf;
    }

    // Iterators remain valid, as opposed to invalidation in standard spec.
    void rehash(size_t count) {
        size_t n = static_cast<size_t>(size() / max_load_factor());
        if (count < n) count = n;
        const size_t new_bucket = next_prime(count);
        std::vector<node_ptr> newtable(new_bucket, nullptr);
        iterator pos = begin(), next = pos;
        for (; pos.ptr(); pos = next) {
            ++next;
            // Inserting while preserving relative orders if them go into the same
            // bucket again. It won't allocate new nodes, but relink the old ones.
            rehash_insert_tail(_hash(get_key(pos.ptr())) % new_bucket, pos.ptr(), newtable);
            // if relative orders are not required, we can do faster by inserting at head
            //rehash_insert_head(_hash(get_key(pos.ptr())) % new_bucket, pos.ptr(), newtable);
        }
        _hashtable.swap(newtable);
    }

    void reserve(size_t count) {
        rehash(std::ceil(count / max_load_factor()));
    }

    /* observers */

    hasher hash_function() const {
        return _hash;
    }

    key_equal key_eq() const {
        return _keyeq;
    }

    /* visualization */

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define BROWN   "\033[0;33m"
#define END     "\033[0m"

    // print bucket [i, n)
#define print_range(i, n)                                                           \
        for (size_t k = i; k < n; ++k) {                                            \
            print_bracket("|* ", RED);                                              \
            std::cout << GREEN << std::right << std::setw(digits) << k << END;      \
            print_bracket(" *|", RED);                                              \
            for (auto it = begin(k); it != end(k); ++it) {                          \
                std::cout << " --> "; print_val(it.ptr());                          \
            }                                                                       \
            std::cout << '\n';                                                      \
        }

    void print(size_t buckets = 37) const {
        //if (buckets > 997) buckets = 997; // prevent from printing too much :)
        size_t n = bucket_count();
        size_t digits = no_of_digit(n);
        if (n <= buckets) { // print all buckets
            print_range(0, n);
        }
        else { // print first half and last half buckets only
            size_t half = buckets / 2;
            print_range(0, half);
            print_3dots_bucket(digits + 6); // |**| + 2ws in between
            print_range(n - half, n);
        }
    }

private:
    static void print_3dots_bucket(size_t width) {
        size_t center = width / 2;
        std::cout << RED << "|*" << END;
        // print ws from "|*"(R) to (L)"..."
        for (size_t i = 2; i < center - 1; ++i) {
            std::cout << ' ';
        }
        std::cout << GREEN << "..." << END;
        // print ws from "..."(R) to (L)"*|"
        for (size_t i = center + 2; i < width - 2; ++i) {
            std::cout << ' ';
        }
        std::cout << RED << "*|" << END;
        std::cout << '\n';
    }

    static size_t no_of_digit(size_t x) noexcept {
        size_t n = 1;
        while (x /= 10) ++n;
        return n;
    }

    static void print_bracket(const char* bracket, const char* color) {
        std::cout << color << bracket << END;
    }

    // hash map
    static void print_val_via_ptr(node_ptr x, std::true_type) {
        std::cout << BROWN << '{' << (x->_val).first
            << ", " << (x->_val).second << '}' << END;
    }

    // hash set
    static void print_val_via_ptr(node_ptr x, std::false_type) {
        std::cout << BROWN << x->_val << END;
    }

    static void print_val(node_ptr x) {
        print_val_via_ptr(x, std::bool_constant<IsMap>{});
    }

private:
    // here n >= 5 and is odd number
    static bool is_odd_prime(size_t n) {
        // This is checked so that we can skip middle 5 numbers in below loop
        if (n % 3 == 0/* || n % 2 == 0 */) return false;

        for (size_t i = 5; i * i <= n; i = i + 6)
            if (n % i == 0 || n % (i + 2) == 0)
                return false;

        return true;
    }

    static size_t next_prime(size_t x) {
        if (x == 1 || x == 0) return 2;
        if (x == 2)           return 3;
        if (x % 2 == 0) ++x;    // turn it into an odd number
        else            x += 2; // get next odd number
        for (; true; x += 2)
            if (is_odd_prime(x)) return x;
    }

    node_ptr new_node(const T& val, node_ptr next = nullptr)
    {
        node_ptr p = _alloc.allocate(1);
        try {
            ::new ((void*)p) node(val, next);
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

    // before calling it, you MUST set policies (members) first
    void copy_nodes(const _self& rhs) {
        _count = rhs._count;
        node_ptr prev = nullptr;
        for (const_iterator pos = rhs.begin(); pos.ptr(); ++pos) {
            // inserting at tail to retain relative orders in the same bucket
            node_ptr newnode = new_node(pos.ptr()->_val, nullptr);
            if (_hashtable[pos.bucket()] == nullptr) _hashtable[pos.bucket()] = newnode;
            if (prev) prev->_next = newnode;
            if (pos.ptr()->_next) prev = newnode;
            else prev = nullptr;
        }
    }

    void clear_nodes() noexcept {
        iterator pos = begin();
        node_ptr del;
        while (pos.ptr()) {
            del = pos.ptr();
            ++pos;
            delete_node(del);
        }
    }

    // hash map
    static const key_type& get_key_via_ptr(node_ptr x, std::true_type) noexcept {
        return (x->_val).first;
    }

    // hash set
    static const key_type& get_key_via_ptr(node_ptr x, std::false_type) noexcept {
        return x->_val;
    }

    static const key_type& get_key(node_ptr x) noexcept {
        return get_key_via_ptr(x, std::bool_constant<IsMap>{});
    }

    // hash map
    static const key_type& get_key_via_t(const T& val, std::true_type) noexcept {
        return val.first;
    }

    // hash set
    static const key_type& get_key_via_t(const T& val, std::false_type) noexcept {
        return val;
    }

    static const key_type& get_key(const T& val) noexcept {
        return get_key_via_t(val, std::bool_constant<IsMap>{});
    }

    // loop cursor is `pos`
#define in_bucket_for_each_if_equal(key, n)                             \
        for (node_ptr pos = _hashtable[n]; pos; pos = pos->_next)       \
            if (_keyeq(get_key(pos), key))


    // return the first position that compares equivalent to key
    iterator find_aux(const key_type& key) const {
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            return iterator(pos, this, k);
        }
        return iterator(nullptr, this, _hashtable.size());
    }

    std::pair<iterator, iterator> equal_range_aux(const key_type& key) const {
        iterator first = find_aux(key), next = first;
        if (next.ptr()) ++next;
        while (next.ptr() && first.bucket() == next.bucket()
            && _keyeq(get_key(first.ptr()), get_key(next.ptr())))
            ++next;
        return { first, next };
    }

    // insert a new entry `val` at the beginning of bucket n
    node_ptr insert_head(size_t n, const T& val) {
        node_ptr newnode = new_node(val, _hashtable[n]);
        ++_count;
        return _hashtable[n] = newnode;
    }

    /*
     * The following insertion routine is designed for
     * retaining the relative orders in the same bucket,
     * while normal insertion happens at bucket head for
     * the sake of updates (newly inserted value can be
     * accessed before old ones, which works like cache).
     */

    // insert an initialized `newnode` at the end of bucket n
    static void rehash_insert_tail(size_t n, node_ptr newnode, std::vector<node_ptr>& newtable) {
        node_ptr tail = newtable[n];
        if (tail) {
            for (; tail->_next; tail = tail->_next);
            tail->_next = newnode;
        }
        newnode->_next = nullptr;
        if (newtable[n] == nullptr) newtable[n] = newnode;
    }

    // insert at head if orders are not required
    static void rehash_insert_head(size_t n, node_ptr newnode, std::vector<node_ptr>& newtable) {
        newnode->_next = newtable[n];
        newtable[n] = newnode;
    }

    void try_rehash() {
        if (load_factor() > max_load_factor())
            rehash(2 * size());
    }

protected:
    // only for hash map
    std::pair<iterator, bool> insert_or_assign(const T& val) {
        try_rehash();
        const key_type& key = get_key(val);
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            (pos->_val).second = val.second;
            return { iterator(pos, this, k), false };
        }
        return { iterator(insert_head(k, val), this, k), true };
    }

    std::pair<iterator, bool> insert_unique(const T& val) {
        try_rehash();
        const key_type& key = get_key(val);
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            return { iterator(pos, this, k), false };
        }
        return { iterator(insert_head(k, val), this, k), true };
    }

    iterator insert_multi(const T& val) {
        try_rehash();
        const key_type& key = get_key(val);
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            if (pos == _hashtable[k]) break; // go insert at head
            // otherwise insert before `pos`
            node_ptr newnode = new_node(val, pos);
            node_ptr prev = _hashtable[k];
            for (; prev->_next != pos; prev = prev->_next);
            prev->_next = newnode;
            ++_count;
            return iterator(newnode, this, k);
        }
        // no dup or dup at first node
        return iterator(insert_head(k, val), this, k);
    }

private:
    std::pair<node_ptr, size_t> next_entry(node_ptr x, size_t bucket_index) const {
        assert(x != nullptr && "cannot increment end() iterator");
        if (x->_next) return { x->_next, bucket_index };
        size_t i = bucket_index + 1;
        for (; i < _hashtable.size(); ++i) {
            if (_hashtable[i]) return { _hashtable[i], i };
        }
        return { nullptr, i };
    }

    iterator erase_aux(node_ptr x) {
        assert(x != nullptr && "cannot erase end() iterator");
        const size_t k = bucket(get_key(x));
        auto next = next_entry(x, k);
        if (_hashtable[k] == x)  _hashtable[k] = x->_next;
        else {
            node_ptr prev = _hashtable[k];
            for (; prev->_next != x; prev = prev->_next);
            prev->_next = x->_next;
        }
        delete_node(x);
        --_count;
        return iterator(next.first, this, next.second);
    }

    struct Hash_node {
        T _val;
        node_ptr _next;

        Hash_node(const T& val, node_ptr next) : _val(val), _next(next) {}

        T* val_ptr() {
            return &_val;
        }
        const T* val_ptr() const {
            return &_val;
        }
    };

    class Hash_iter {
        using _self = Hash_iter;
    protected:
        node_ptr _ptr = nullptr;
        const Hashtable* _table = nullptr;
        size_t _bucket_index = 0;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Hash_iter() noexcept {}

        Hash_iter(node_ptr ptr, const Hashtable* table, size_t bucket_index) noexcept
            : _ptr(ptr), _table(table), _bucket_index(bucket_index) {}

        node_ptr ptr() const noexcept { return _ptr; }
        const Hashtable* container() const noexcept { return _table; }
        size_t bucket() const noexcept { return _bucket_index; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            auto next = _table->next_entry(_ptr, _bucket_index);
            _ptr = next.first;
            _bucket_index = next.second;
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            operator++();
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            assert(lhs._table == rhs._table && "iterators incompatible");
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            assert(lhs._table == rhs._table && "iterators incompatible");
            return lhs._ptr != rhs._ptr;
        }
    };

    class Hash_const_iter {
        using _self = Hash_const_iter;
    protected:
        node_ptr _ptr = nullptr;
        const Hashtable* _table = nullptr;
        size_t _bucket_index = 0;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        Hash_const_iter() noexcept {}

        Hash_const_iter(node_ptr ptr, const Hashtable* table, size_t bucket_index) noexcept
            : _ptr(ptr), _table(table), _bucket_index(bucket_index) {}

        Hash_const_iter(const Hash_iter& other) noexcept : _ptr(other.ptr()),
            _table(other.container()), _bucket_index(other.bucket()) {}

        node_ptr ptr() const noexcept { return _ptr; }
        const Hashtable* container() const noexcept { return _table; }
        size_t bucket() const noexcept { return _bucket_index; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            auto next = _table->next_entry(_ptr, _bucket_index);
            _ptr = next.first;
            _bucket_index = next.second;
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            operator++();
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            assert(lhs._table == rhs._table && "iterators incompatible");
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            assert(lhs._table == rhs._table && "iterators incompatible");
            return lhs._ptr != rhs._ptr;
        }        
    };

    class Hash_local_iter : public Hash_iter {
        using _base = Hash_iter;
        using _self = Hash_local_iter;
    public:
        Hash_local_iter() noexcept : _base() {}

        Hash_local_iter(node_ptr ptr, const Hashtable* table, size_t bucket_index) noexcept
            : _base(ptr, table, bucket_index) {}

        _self& operator++() {
            _base::_ptr = _base::_ptr->_next;
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _base::_ptr = _base::_ptr->_next;
            return tmp;
        }
    };

    class Hash_const_local_iter : public Hash_const_iter {
        using _base = Hash_const_iter;
        using _self = Hash_const_local_iter;
    public:
        Hash_const_local_iter() noexcept : _base() {}

        Hash_const_local_iter(node_ptr ptr, const Hashtable* table, size_t bucket_index) noexcept
            : _base(ptr, table, bucket_index) {}

        Hash_const_local_iter(const Hash_local_iter& other) noexcept : _base::_ptr(other.ptr()),
            _base::_table(other.container()), _base::_bucket_index(other.bucket()) {}

        _self& operator++() {
            _base::_ptr = _base::_ptr->_next;
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _base::_ptr = _base::_ptr->_next;
            return tmp;
        }
    };
}; // class Hashtable

} // namespace alternative
} // namespace mySymbolTable

#undef RED
#undef GREEN
#undef BROWN
#undef END
#undef print_range
#undef in_bucket_for_each_if_equal

#endif // !HASHTABLE2_IMPL_H