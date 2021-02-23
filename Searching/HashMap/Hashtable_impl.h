/*
 *  internal header file for implementing
 *  unordered symbol tables:
 *  Hash (multi) map/set
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/HashMap/Hashtable_impl.h
 */

#ifndef HASHTABLE_IMPL_H
#define HASHTABLE_IMPL_H 1

#include <utility>  // std::pair, std::swap
#include <iterator> // std::distance
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>    // std::ceil
#include <cassert>
#include "my_map_traits.h"  // myst::get_map_key_t

namespace mySymbolTable {

// Separate Chaining
template<typename T, typename Hash, typename KeyEqual, typename Alloc, bool IsMap>
class Hashtable {
    struct Hash_node;
    class Hash_iter;
    class Hash_const_iter;
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
    using local_iterator = Hash_iter;
    using const_local_iterator = Hash_const_iter;
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
        for (auto head : _hashtable) {
            if (head) return iterator(head);
        }
        return end();
    }

    const_iterator begin() const noexcept {
        for (auto head : _hashtable) {
            if (head) return const_iterator(head);
        }
        return end();
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    iterator end() noexcept {
        return iterator(nullptr);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr);
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
        return iterator(erase_aux(pos.ptr()));
    }

    iterator erase(const_iterator pos) {
        return iterator(erase_aux(pos.ptr()));
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
        return iterator(find_aux(key));
    }

    const_iterator find(const key_type& key) const {
        return const_iterator(find_aux(key));
    }

    bool contains(const key_type& key) const {
        return find_aux(key) != nullptr;
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
        return local_iterator(_hashtable[n]);
    }

    const_local_iterator begin(size_t n) const {
        assert(n < _hashtable.size());
        return const_local_iterator(_hashtable[n]);
    }

    const_local_iterator cbegin(size_t n) const {
        return begin(n);
    }

    local_iterator end(size_t n) {
        assert(n < _hashtable.size());
        return local_iterator(end_ptr_of_bucket(n));
    }

    const_local_iterator end(size_t n) const {
        assert(n < _hashtable.size());
        return const_local_iterator(end_ptr_of_bucket(n));
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
        node_ptr pos = begin().ptr();
        std::vector<node_ptr> newtable(next_prime(count), nullptr);
        _hashtable.swap(newtable);
        for (node_ptr next; pos; pos = next) {
            next = pos->_next;
            // Inserting while preserving relative orders if them go into the same
            // bucket again. It won't allocate new nodes, but relink the old ones.
            rehash_insert_tail(bucket(get_key(pos)), pos);
        }
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

    node_ptr new_node(const T& val, node_ptr prev = nullptr, node_ptr next = nullptr)
    {
        node_ptr p = _alloc.allocate(1);
        try {
            ::new ((void*)p) node(val, prev, next);
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
        for (node_ptr pos = rhs.begin().ptr(); pos; pos = pos->_next) {
            // inserting at tail to retain relative orders in the same bucket
            //insert_tail(bucket(get_key(pos)), pos->_val); // a little bit slower
            prev = copying_insert_tail(bucket(get_key(pos)), pos->_val, prev);
        }
    }

    node_ptr copying_insert_tail(size_t n, const T& val, node_ptr prev) {
        node_ptr newnode = new_node(val, prev, nullptr);
        if (prev) prev->_next = newnode;
        return _hashtable[n] ? newnode : _hashtable[n] = newnode;
    }

    void clear_nodes() noexcept {
        node_ptr pos = begin().ptr(), del;
        while (pos) {
            del = pos;
            pos = pos->_next;
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

    // set `next` the first non-empty bucket head after bucket n, if any
#define set_next(next, n, N)                                     \
        for (size_t i = n + 1; i < N; ++i) {                     \
            if (_hashtable[i]) {                                 \
                next = _hashtable[i]; break;                     \
            }                                                    \
        }

    // set `x` the tail of its bucket (n), starting from `x` itself
    // if  `x` is null, the tail is still `x` per se
#define set_tail(x, n)                                           \
        for (node_ptr pos = x;                                   \
             pos && bucket(get_key(pos)) ==                      \
                    static_cast<size_t>(n);                      \
             pos = pos->_next)                                   \
        { x = pos; }

    // set `prev` the first non-empty bucket tail before bucket n, if any
#define set_prev(prev, n)                                        \
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {     \
            if (_hashtable[i]) {                                 \
                prev = _hashtable[i];                            \
                set_tail(prev, i);                               \
                break;                                           \
            }                                                    \
        }

    // loop cursor is `pos`
#define in_bucket_for_each_if_equal(key, n)                      \
        for (node_ptr pos = _hashtable[n],                       \
                      end = end_ptr_of_bucket(n);                \
             pos != end;                                         \
             pos = pos->_next)                                   \
            if (_keyeq(get_key(pos), key))


    // return next non-empty bucket head, if any
    node_ptr end_ptr_of_bucket(size_t n) const {
        if (_hashtable[n] == nullptr) // return itself if it's an empty bucket
            return nullptr;
        const size_t N = _hashtable.size();
        node_ptr next = nullptr;
        set_next(next, n, N);
        return next;
    }

    // return first position that compares equivalent to key
    node_ptr find_aux(const key_type& key) const {
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            return pos;
        }
        return nullptr; // end(), not found
    }

    std::pair<node_ptr, node_ptr> equal_range_aux(const key_type& key) const {
        node_ptr first = find_aux(key);
        node_ptr next = first == nullptr ? nullptr : first->_next;
        for (; next && _keyeq(get_key(first), get_key(next)); next = next->_next);
        return { first, next };
    }

    // insert a new entry `val` at the beginning of bucket n
    node_ptr insert_head(size_t n, const T& val) {
        node_ptr next = _hashtable[n];
        node_ptr prev = next ? next->_prev : nullptr;
        if (!next) { // empty bucket
            const size_t N = _hashtable.size();
            set_next(next, n, N);
            if (next) prev = next->_prev;
            else      set_prev(prev, n);
        }
        node_ptr newnode = new_node(val, prev, next);
        if (prev) prev->_next = newnode;
        if (next) next->_prev = newnode;
        ++_count;
        return _hashtable[n] = newnode;
    }

    /*
     * The following two insert at tail are designed for 
     * retaining the relative orders in the same bucket,
     * while normal insertion happens at bucket head for
     * the sake of updates (newly inserted value can be
     * accessed before old ones, which works like cache).
     */

    // insert a new entry `val` at the end of bucket n
    node_ptr insert_tail(size_t n, const T& val) {
        node_ptr prev = _hashtable[n];
        set_tail(prev, n);
        node_ptr next = prev ? prev->_next : nullptr;
        if (!prev) { // empty bucket
            const size_t N = _hashtable.size();
            // This version is geared torwards hashtable copying, since
            // when doing copying buckets after the current one are all
            // empty, which means traversing downward will get nothing!
            set_prev(prev, n); // in general, more expensive though
            if (prev) next = prev->_next;
            else      set_next(next, n, N);
        }
        node_ptr newnode = new_node(val, prev, next);
        if (prev) prev->_next = newnode;
        if (next) next->_prev = newnode;
        ++_count;
        return _hashtable[n] ? newnode : _hashtable[n] = newnode;
    }

    // insert an initialized `newnode` at the end of bucket n
    node_ptr rehash_insert_tail(size_t n, node_ptr newnode) {
        node_ptr prev = _hashtable[n];
        set_tail(prev, n);
        node_ptr next = prev ? prev->_next : nullptr;
        if (!prev) { // empty bucket
            const size_t N = _hashtable.size();
            set_next(next, n, N);
            if (next) prev = next->_prev;
            else      set_prev(prev, n);
        }
        newnode->_prev = prev; newnode->_next = next;
        if (prev) prev->_next = newnode;
        if (next) next->_prev = newnode;
        return _hashtable[n] ? newnode : _hashtable[n] = newnode;
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
            return { pos, false };
        }
        return { insert_head(k, val), true };
    }

    std::pair<iterator, bool> insert_unique(const T& val) {
        try_rehash();
        const key_type& key = get_key(val);
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            return { pos, false };
        }
        return { insert_head(k, val), true };
    }

    iterator insert_multi(const T& val) {
        try_rehash();
        const key_type& key = get_key(val);
        const size_t k = bucket(key);
        in_bucket_for_each_if_equal(key, k) {
            if (pos == _hashtable[k]) break; // return insert_head(k, val)
            return insert_before(pos, val);
        }
        // no dup or dup at first node
        return insert_head(k, val);
    }

private:
    // precondition: x is NOT the first node in its bucket, otherwise
    // we will have to update the head pointer in the hashtable.
    node_ptr insert_before(node_ptr x, const T& val) {
        node_ptr newnode = new_node(val, x->_prev, x);
        x->_prev->_next = newnode;
        x->_prev = newnode;
        ++_count;
        return newnode;
    }

    node_ptr erase_aux(node_ptr x) {
        assert(x != nullptr && "cannot erase end() iterator");
        node_ptr prev = x->_prev;
        node_ptr next = x->_next;
        if (next) next->_prev = prev;
        if (prev) prev->_next = next;
        const size_t k = bucket(get_key(x));
        if (x == _hashtable[k]) { // x is the head of this bucket
            if (next && bucket(get_key(next)) == k)
                _hashtable[k] = next;
            else _hashtable[k] = nullptr;
        }
        delete_node(x);
        --_count;
        return next;
    }

    struct Hash_node {
        T _val;
        node_ptr _prev, _next;

        Hash_node(const T& val, node_ptr prev, node_ptr next)
            : _val(val), _prev(prev), _next(next) {}

        T* val_ptr() {
            return &_val;
        }
        const T* val_ptr() const {
            return &_val;
        }
    };

    class Hash_iter {
        using _self = Hash_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Hash_iter() noexcept : _ptr(nullptr) {}
        Hash_iter(node_ptr ptr) noexcept : _ptr(ptr) {}

        node_ptr ptr() const noexcept { return _ptr; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _ptr = _ptr->_next;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };

    class Hash_const_iter {
        using _self = Hash_const_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        Hash_const_iter() noexcept : _ptr(nullptr) {}
        Hash_const_iter(node_ptr ptr) noexcept : _ptr(ptr) {}
        Hash_const_iter(const Hash_iter& other) noexcept : _ptr(other.ptr()) {}

        node_ptr ptr() const noexcept { return _ptr; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _ptr = _ptr->_next;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };
}; // class Hashtable

} // namespace mySymbolTable

#undef RED
#undef GREEN
#undef BROWN
#undef END
#undef print_range
#undef set_next
#undef set_tail
#undef set_prev
#undef in_bucket_for_each_if_equal

#endif // !HASHTABLE_IMPL_H