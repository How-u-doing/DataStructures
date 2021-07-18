/*
 *  internal header file for implementing
 *  ordered symbol tables:
 *  AVL (multi) map/set
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/AVLtree_impl.h
 */

#ifndef AVLTREE_IMPL_H
#define AVLTREE_IMPL_H 1

#include <type_traits> // std::enable_if
#include <memory>   // std::addressof, std::allocator_tarits
#include <utility>  // std::swap, std::pair
#include <iterator> // std::reverse_iterator, std::distance
#ifndef NDEBUG
#include <iostream> // std::cout
#include <string>
#endif
#include <cassert>
#include "my_map_traits.h"  // myst::get_map_key_t

namespace mySymbolTable {

// Adelson-Velsky and Landis' self-balancing binary search tree
template<typename T, typename Compare, typename Alloc, bool IsMap, bool IsMulti>
class AVLtree {
    struct AVLtree_node;
    class AVLtree_iter;
    class AVLtree_const_iter;
    using _self = AVLtree<T, Compare, Alloc, IsMap, IsMulti>;
    using node = AVLtree_node;
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
    using iterator = AVLtree_iter;
    using const_iterator = AVLtree_const_iter;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = AVLtree_node;

private:
    // _header->_parent points to root node
    // _header->_parent->_parent == _header
    // _header->_left  points to the leftmost node
    // _header->_right points to the rightmost node
    node_ptr _header;
    size_t   _count;
    Compare  _comp;
    NodeAl   _alloc;

#define ROOT _header->_parent

    void set_default_header() noexcept {
        _header->_parent = _header;
        _header->_left   = _header;
        _header->_right  = _header;
    }

    // create & initialize the header node
    void create_header() {
        _header = _alloc.allocate(1); // may throw
        set_default_header();
        // leave the value field uninitialized
    }

    // clear old data only when rhs is empty
    void copy(const _self& rhs) {
        if (!rhs.empty()) {
            ROOT = copy_nodes(rhs.ROOT, _header);
            _count = rhs._count;
            _header->_left  = tree_min(ROOT);
            _header->_right = tree_max(ROOT);
        }
        else clear();
    }
public:
    AVLtree() : _count(0), _comp(), _alloc() {
        create_header();
    }

    explicit AVLtree(const Compare& comp, const Alloc& alloc = Alloc())
        : _count(0), _comp(comp), _alloc(alloc) {
        create_header();
    }

    explicit AVLtree(const Alloc& alloc) : _count(0), _comp(), _alloc(alloc) {
        create_header();
    }

    AVLtree(const _self& rhs) : _count(rhs._count), _comp(rhs._comp), _alloc(rhs._alloc) {
        create_header();
        copy(rhs);
    }

    ~AVLtree() { clear(); _alloc.deallocate(_header, 1); }

    _self& operator=(const _self& rhs) {
        if (this == &rhs) return *this;
        clear();
        copy(rhs);
        return *this;
    }

    allocator_type get_allocator() const noexcept {
        return _alloc;
    }

    /* iterators */

    iterator begin() noexcept {
        return iterator(_header->_left);
    }

    const_iterator begin() const noexcept {
        return const_iterator(_header->_left);
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

    /* lookup */

    size_t count(const key_type& key) const {
        if constexpr (!IsMulti) {
            return contains(key) ? 1 : 0;
        }
        else {
            auto r = equal_range(key);
            return std::distance(r.first, r.second);
        }
    }

    iterator find(const key_type& key) {
        return iterator(find(ROOT, key));
    }
    
    const_iterator find(const key_type& key) const {
        return const_iterator(find(ROOT, key));
    }

    bool contains(const key_type& key) const {
        return find(ROOT, key) != _header;
    }

    iterator lower_bound(const key_type& key) {
        return iterator(lower_bound(ROOT, key));
    }

    const_iterator lower_bound(const key_type& key) const {
        return const_iterator(lower_bound(ROOT, key));
    }

    iterator upper_bound(const key_type& key) {
        return iterator(upper_bound(ROOT, key));
    }
    
    const_iterator upper_bound(const key_type& key) const {
        return const_iterator(upper_bound(ROOT, key));
    }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
        return equal_range_aux(key);
    }
    
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        return equal_range_aux(key);
    }

    /* modifiers */

    void clear() noexcept {
        if (!empty()) {
            clear_nodes(ROOT); _count = 0; set_default_header();
        }
    }

    std::conditional_t<!IsMulti, std::pair<iterator, bool>, iterator>
    insert(const T& val) {
        if constexpr (!IsMulti) return insert_aux(&ROOT, val);
        else return insert_aux(&ROOT, val).first;
    }

private:
    iterator insert_hint_unique(node_ptr hint, const T& val) {
        const key_type& key = get_key(val);
        if (hint == _header) { // end()
            if (_comp(get_key(_header->_right), key)) // max < key
                return insert_leaf_at(&_header->_right->_right, val, _header->_right);
            else return insert_aux(&ROOT, val).first;
        }
        else if (_comp(key, get_key(hint))) { // key < hint
            if (hint == _header->_left) // key < min
                return insert_leaf_at(&_header->_left->_left, val, _header->_left);
            node_ptr prev = tree_prev(hint);
            if (_comp(get_key(prev), key)) { // prev < key < hint
                if (prev->_right == nullptr) // prev is the rightmost of the left subtree of hint
                    return insert_leaf_at(&prev->_right, val, prev);
                else // otherwise hint->_left is null
                    return insert_leaf_at(&hint->_left, val, hint);
            }
            return insert_aux(&ROOT, val).first;
        }
        else if (_comp(get_key(hint), key)) { // hint < key
            if (hint == _header->_right) // max < key
                return insert_leaf_at(&_header->_right->_right, val, _header->_right);
            node_ptr next = tree_next(hint);
            if (_comp(key, get_key(next))) { // hint < key < next
                if (next->_left == nullptr)  // next is the leftmost of the right subtree of hint
                    return insert_leaf_at(&next->_left, val, next);
                else // otherwise hint->_right is null
                    return insert_leaf_at(&hint->_right, val, hint);
            }
            return insert_aux(&ROOT, val).first;
        }
        else // equivalent keys
            return iterator(hint);
    }

    iterator insert_hint_multi(node_ptr hint, const T& val) {
        const key_type& key = get_key(val);
        if (hint == _header) { // end()
            if (!_comp(key, get_key(_header->_right))) // key >= max
                return insert_leaf_at(&_header->_right->_right, val, _header->_right);
            else return insert_aux(&ROOT, val).first;
        }
        else if (!_comp(get_key(hint), key)) { // key <= hint
            if (hint == _header->_left) // key <= min
                return insert_leaf_at(&_header->_left->_left, val, _header->_left);
            node_ptr prev = tree_prev(hint);
            if (!_comp(key, get_key(prev))) { // prev <= key <= hint
                if (prev->_right == nullptr)  // prev is the rightmost of the left subtree of hint
                    return insert_leaf_at(&prev->_right, val, prev);
                else // otherwise hint->_left is null
                    return insert_leaf_at(&hint->_left, val, hint);
            }
            return insert_aux(&ROOT, val).first;
        }
        else { // hint < key
            if (hint == _header->_right) // max < key
                return insert_leaf_at(&_header->_right->_right, val, _header->_right);
            node_ptr next = tree_next(hint);
            if (!_comp(get_key(next), key)) { // hint < key <= next
                if (next->_left == nullptr)   // next is the leftmost of the right subtree of hint
                    return insert_leaf_at(&next->_left, val, next);
                else // otherwise hint->_right is null
                    return insert_leaf_at(&hint->_right, val, hint);
            }
            return insert_aux(&ROOT, val).first;
        }
    }

public:
    // Inserts value in the position as close as possible, just prior to hint.
    // Complexity: Amortized constant if the insertion happens in the position
    // just before the hint, logarithmic in the size of the container otherwise.
    iterator insert(const_iterator hint, const T& val) {
        if (empty()) return insert_leaf_at(&ROOT, val, _header);
        if constexpr (!IsMulti)
            return insert_hint_unique(hint.ptr(), val);
        else
            return insert_hint_multi(hint.ptr(), val);
    }

    template< typename InputIt >
    void insert(InputIt first, InputIt last) {
        while (first != last) {
            insert(*first++);
        }
    }

    void insert(std::initializer_list<value_type> ilist) {
        return insert(ilist.begin(), ilist.end());
    }

protected:
    std::pair<iterator, bool> insert_or_assign(const T& val) {
        return insert_aux(&ROOT, val, /*assign=*/true);
    }

public:
    iterator erase(iterator pos) {
        return iterator(erase(pos.ptr()));
    }

    iterator erase(const_iterator pos) {
        return iterator(erase(pos.ptr()));
    }

    iterator erase(const_iterator first, const_iterator last) {
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
        if constexpr (!IsMulti) {
            iterator it = find(key);
            if (it == end()) return 0;
            erase(it);
            return 1;
        }
        else {
            auto r = equal_range(key);
            size_t n = std::distance(r.first, r.second);
            erase(r.first, r.second);
            return n;
        }
    }

    void swap(AVLtree& rhs) noexcept(std::allocator_traits<Alloc>::is_always_equal::value
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

    /* tree height interface */

    // tree height (height of root),  i.e. the number of
    // edges on the longest path from root node to a leaf
    int height() const {
        if (empty()) return -1; // -1 suggests it's empty
        return height(ROOT) - 1;
    }

    int height(const_iterator pos) const {
        if (pos == end()) return -1;
        return height(pos.ptr()) - 1;
    }

private:
    static int height(node_ptr x) {
        if (x == nullptr) return 0;
        return 1 + max(height(x->_left), height(x->_right));
    }

    static int max(int a, int b) noexcept {
        return a > b ? a : b;
    }

#ifndef NDEBUG
public:
    /* visualization */

    // print tree by lines
    void print(size_t level = 10) const { print(ROOT, level); }

    /* debug */

    void check_bf() const {
        if (!empty()) check_bf_aux(ROOT, std::bool_constant<IsMap>{});
    }

    bool is_balanced() const {
        if (empty()) return true;
        return is_balanced_aux(ROOT);
    }

private:
    static bool is_balanced_aux(node_ptr x) {
        if (x == nullptr) return true;
        int bf_x = bf(x);
        if (bf_x > 1 || bf_x < -1) return false;
        return is_balanced_aux(x->_left) && is_balanced_aux(x->_right);
    }

    static int bf(node_ptr x) {
        return height(x->_right) - height(x->_left);
    }

    static void check_bf_aux(node_ptr x, std::false_type) {
        if (x == nullptr) return;
        int bf_x = bf(x);
        if (bf_x != x->_bf)
            std::cout << "\033[0;31mwrong BF at " << x->_val << " : bf = "
                      << x->_bf << ", but should be " << bf_x << "\033[0;m\n";
        check_bf_aux(x->_left,  std::bool_constant<false>{});
        check_bf_aux(x->_right, std::bool_constant<false>{});
    }

    static void check_bf_aux(node_ptr x, std::true_type) {
        if (x == nullptr) return;
        int bf_x = bf(x);
        if (bf_x != x->_bf)
            std::cout << "\033[0;31mwrong BF at {" << (x->_val).first << ", " << (x->_val).second
                      << "} : bf = " << x->_bf << ", but should be " << bf_x << "\033[0;m\n";
        check_bf_aux(x->_left,  std::bool_constant<true>{});
        check_bf_aux(x->_right, std::bool_constant<true>{});
    }
#endif

private:
    // When using greater<T> (operator>) for comp, the
    // leftmost node is on the contrary the largest.
    // precondition: x != nullptr && x != _header
    static node_ptr tree_min(node_ptr x) noexcept {
        while (x->_left != nullptr)
            x = x->_left;
        return x;
    }

    // precondition: x != nullptr && x != _header
    static node_ptr tree_max(node_ptr x) noexcept {
        while (x->_right != nullptr)
            x = x->_right;
        return x;
    }

    // precondition: x != nullptr && x != _header
    static node_ptr tree_next(node_ptr x) noexcept {
        if (x->_right != nullptr)
            return tree_min(x->_right);
        // ROOT being rightmost, go to _header
        if (x == x->_parent->_parent) return x->_parent;
        node_ptr parent = x->_parent;// root->_parent == _header
        while (x == parent->_right) {// root == _header->_parent
            x = parent;
            parent = x->_parent;
        }
        return parent; // _header if x points to the rightmost node
    }

    // precondition: x != nullptr && x != _header
    static node_ptr tree_prev(node_ptr x) noexcept {
        if (x->_left != nullptr)
            return tree_max(x->_left);
        // ROOT being leftmost, go to _header
        if (x == x->_parent->_parent) return x->_parent;
        node_ptr parent = x->_parent;// root->_parent == _header
        while (x == parent->_left) { // root == _header->_parent
            x = parent;
            parent = x->_parent;
        }
        return parent; // _header if x points to the leftmost node
    }

    node_ptr new_node(const T& val, node_ptr parent, node_ptr left  = nullptr,
                                                     node_ptr right = nullptr)
    {
        node_ptr p = _alloc.allocate(1);
        try {
            ::new ((void*)p) node(val, parent, left, right);
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

    node_ptr copy_nodes(node_ptr x, node_ptr parent) {
        if (x != nullptr) {
            node_ptr t = new_node(x->_val, parent);
            t->_bf = x->_bf;
            t->_left  = copy_nodes(x->_left,  t);
            t->_right = copy_nodes(x->_right, t);
            return t;
        }
        return nullptr;
    }

    // precondition: x = ROOT != _header or size() > 0
    void clear_nodes(node_ptr x) noexcept {
        if (x != nullptr) {
            clear_nodes(x->_left);
            clear_nodes(x->_right);
            delete_node(x);
        }
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

    node_ptr find(node_ptr x, const key_type& key) const {
        if (x == _header) return _header;
        while (x != nullptr) {
            if      (_comp(key, get_key(x))) x = x->_left;
            else if (_comp(get_key(x), key)) x = x->_right;
            else return x;
        }
        return _header; // end(), not found
    }

    std::pair<node_ptr, node_ptr> equal_range_aux(const key_type& key) const {
        if constexpr (!IsMulti) {
            const_iterator first = lower_bound(key), second = first;
            if (second != end()) ++second;
            return { first.ptr(), second.ptr() };
        }
        else {
            return { lower_bound(ROOT, key), upper_bound(ROOT, key) };
        }
    }

    node_ptr upper_bound(node_ptr x, const key_type& key) const {
        if (x == _header) return _header;
        node_ptr parent = x->_parent;
        while (x != nullptr) {
            if (!_comp(key, get_key(x))) x = x->_right;
            else {// x > key
                parent = x;
                x = x->_left;
            }
        }
        return parent; // _header if not found
    }

    node_ptr lower_bound(node_ptr x, const key_type& key) const {
        if (x == _header) return _header;
        node_ptr parent = x->_parent;
        while (x != nullptr) {
            if (_comp(get_key(x), key)) x = x->_right;
            else {// x >= key
                parent = x;
                x = x->_left;
            }
        }
        return parent; // _header if not found
    }

protected:
    // I don't want to include the whole <tuple>,
    // plus using a struct is more convenient :)
    struct lower_bound_result {
        iterator it;
        node** x;
        node_ptr x_parent;
    };
    // This subroutine is very helpful in implementing map::operator[], first
    // we try to find it's lower bound (first position no less than key), if key
    // is smaller than this lower bound (suggests the key doesn't exist) then we
    // can use this cool lower bound's 2nd return value and insert exactly there.
    lower_bound_result cool_lower_bound(const key_type& key) {
        node** x = &ROOT;
        if (*x == _header) return { end(), x, _header }; // empty container
        node_ptr parent = _header, x_parent = _header;
        while (*x != nullptr) {
            x_parent = *x;
            if (_comp(get_key(*x), key)) x = &(*x)->_right;
            else {// x >= key
                parent = *x;
                x = &(*x)->_left;
            }
        }
        return { iterator(parent), x, x_parent }; // end() if not found
    }

    node_ptr insert_leaf_at(node** x, const T& val, node_ptr parent) {
        *x = new_node(val, parent);
        ++_count;
        if (_count == 1) {
            _header->_parent = _header->_left = _header->_right = *x;
        }
        else {
            if (*x == _header->_left->_left  ) _header->_left  = *x;
            if (*x == _header->_right->_right) _header->_right = *x;
        }
        node_ptr newnode = *x; // balancing may change the link *x
        rebalance_after_inserting(*x);
        return newnode;
    }

private:
    // precondition: *x != nullptr
    std::pair<node_ptr, bool> insert_aux(node** x, const T& val, bool assign = false) {
        if (empty()) return { insert_leaf_at(&ROOT, val, _header), true };
        const key_type& key = get_key(val);
        node_ptr parent = (*x)->_parent;
        while (*x != nullptr) {
            parent = *x;
            if      (_comp(key, get_key(*x))) x = &(*x)->_left;
            else if (_comp(get_key(*x), key)) x = &(*x)->_right;
            else {
                if constexpr (IsMulti) {
                    x = &(*x)->_right;
                    if (*x != nullptr) {
                        parent = tree_min(*x);
                        x = &parent->_left;
                    }
                    break;
                }
                else {
                    if constexpr (IsMap) {
                        if (assign)  (*x)->_val.second = val.second;
                    }
                    return { *x, false };
                }
            }
        }
        return { insert_leaf_at(x, val, parent), true };
    }

    // H(X)=h, H(b)=h+2, H(Z)=h+1, H(Y)=h (insertion at Z or deletion at X) or H(Y)=h+1 (deletion at X)
    /*
         a  (+2)                                              b  (0 or -1)
        /  \                  rotate left                   /   \
       X    b (+1 or 0)   ------------------->   (0 or +1) a     Z 
           / \             insertion at Z or              / \
          Y   Z            deletion  at X                X   Y

    */
    node_ptr rotate_left(node_ptr a) { // RR shape
        node_ptr b = a->_right;
        a->_right = b->_left;
        if (b->_left) b->_left->_parent = a;
        replace(a, b);
        b->_left = a;
        a->_parent = b;
        if (b->_bf == 1) { // insertion at Z or deletion at X
            a->_bf = b->_bf = 0;
        }
        else { // b->_bf == 0, only happens with deletion at X
            a->_bf = 1;
            b->_bf = -1;
        }
        return b;
    }

    // mirror-image of `rotate_left`
    node_ptr rotate_right(node_ptr a) { // LL shape
        node_ptr b = a->_left;
        a->_left = b->_right;
        if (b->_right) b->_right->_parent = a;
        replace(a, b);
        b->_right = a;
        a->_parent = b;
        if (b->_bf == -1) { // insertion or deletion
            a->_bf = b->_bf = 0;
        }
        else { // b->_bf == 0, only happens with deletion
            a->_bf = -1;
            b->_bf = 1;
        }
        return b;
    }

    // H(X)=h, H(b)=h+2, H(W)=h, H(c)=h+1,
    // H(Y)=H(Z)=h (deletion at X) or
    // H(Y)=h-1, H(Z)=h (insertion at Z or deletion at X) or
    // H(Y)=h, H(Z)=h-1 (insertion at Y or deletion at X)
    /*
              a  (+2)                              a                                       c   (0)
            /  \            rotate right         /   \           rotate left             /   \
           X    b (-1)     ------------->       X     c         ------------>   (-1, 0) a     b (0, +1)
               / \          around b                 / \         around a              / \   / \
    (+/-1, 0) c   W                                 Y   b                             X   Y Z   W
             / \                                       / \
            Y   Z                                     Z   W

    */
    node_ptr rotate_right_left(node_ptr a) { // RL shape
        node_ptr b = a->_right, c = b->_left;
        int bf_c = c->_bf;
        // The setups of balance factors are inaccurate and unnecessary since
        // we're rotating b while BF(b) isn't +/-2, but the cost is trivial.
        rotate_right(b);
        rotate_left(a);
        c->_bf = 0;
        if (bf_c == 0) { // only happens with deletion at X
            a->_bf = b->_bf = 0;
        }
        else {
            if (bf_c == 1) { // insertion at Z or deletion at X
                a->_bf = -1;
                b->_bf = 0;
            }
            else { // bf_c == -1, insertion at Y or deletion at X
                a->_bf = 0;
                b->_bf = 1;
            }
        }
        return c;
    }

    // mirror-image of `rotate_right_left`
    node_ptr rotate_left_right(node_ptr a) { // LR shape
        node_ptr b = a->_left, c = b->_right;
        int bf_c = c->_bf;
        rotate_left(b);
        rotate_right(a);
        c->_bf = 0;
        if (bf_c == 0) { // only happens with deletion
            a->_bf = b->_bf = 0;
        }
        else { // insertion or deletion
            if (bf_c == 1) {
                a->_bf = 0;
                b->_bf = -1;
            }
            else { // bf_c == -1
                a->_bf = 1;
                b->_bf = 0;
            }
        }
        return c;
    }

    // rebalance via retracing ancestors of newly inserted node x
    void rebalance_after_inserting(node_ptr x) {
        for (node_ptr parent = x->_parent; parent != _header;) {
            if (x == parent->_left)
                --parent->_bf;
            else if (x == parent->_right)
                ++parent->_bf;

            if (parent->_bf == 0) return; // H(parent) doesn't change

            if (parent->_bf == 1 || parent->_bf == -1) { // keep retracing & updating ancestors' BFs
                x = parent;
                parent = x->_parent;
                continue;
            }
            else { // parent->_bf == +/-2, do some rotations
                if (parent->_bf == 2) { // right-heavy
                    if (x->_bf == 1) // RR shape
                        rotate_left(parent);
                    else // x->_bf == -1, RL shape
                        rotate_right_left(parent);
                }
                else { // parent->_bf == -2, left-heavy
                    if (x->_bf == -1) // LL shape
                        rotate_right(parent);
                    else // x->_bf == 1, LR shape
                        rotate_left_right(parent);
                }
                return;
            }
        }
    }

    // rebalance from leaf x
    void rebalance_for_deletion_from(node_ptr x) {
        for (node_ptr parent = x->_parent; parent != _header;) {
            if (x == parent->_left)
                ++parent->_bf;
            else if (x == parent->_right)
                --parent->_bf;

            if (parent->_bf == 1 || parent->_bf == -1) return; // deletion doesn't change H(parent)

            if (parent->_bf == 0) { // keep retracing & updating ancestors' BFs
                x = parent;
                parent = x->_parent;
                continue;
            }
            else { // parent->_bf == +/-2, do some rotations
                int sibling_bf = sibling(x)->_bf;
                if (parent->_bf == 2) { // right-heavy
                    if (sibling_bf >= 0) // 0 or 1, RR shape
                        x = rotate_left(parent);
                    else // sibling_bf == -1, RL shape
                        x = rotate_right_left(parent);
                }
                else { // parent->_bf == -2, left-heavy
                    if (sibling_bf <= 0) // -1 or 0, LL shape
                        x = rotate_right(parent);
                    else // sibling_bf == 1, LR shape
                        x = rotate_left_right(parent);
                }

                if (x->_bf == 0) parent = x->_parent; // keep retracing
                else /* x->_bf == +/- 1 */ return;
            }
        }
    }

    bool unlink_leaf(node_ptr x, node_ptr& balance_pos) {
        bool height_decreased = true;
        node_ptr parent = x->_parent;
        node_ptr sibling = x == parent->_left ? parent->_right : parent->_left;
        if (sibling) {
            if (has_children(sibling)) {
                if (x == parent->_left) {
                    parent->_left = nullptr;
                    if (sibling->_bf == -1) { // sibling only has left, RL shape
                        balance_pos = rotate_right_left(x->_parent);
                    }
                    else { // sibling->_bf == 1 or 0, has right or both, RR shape
                        balance_pos = rotate_left(x->_parent);
                    }
                }
                else {
                    parent->_right = nullptr;
                    if (sibling->_bf == 1) { // sibling only has right, LR shape
                        balance_pos = rotate_left_right(x->_parent);
                    }
                    else { // sibling->_bf == -1 or 0, has left or both, LL shape
                        balance_pos = rotate_right(x->_parent);
                    }
                }

                if (balance_pos->_bf != 0) { // -1 or 1, sibling has two children
                    height_decreased = false;
                }
            }
            else { // won't change the height of parent
                if (x == parent->_left) {
                    parent->_bf = 1;
                    parent->_left = nullptr;
                }
                else {
                    parent->_bf = -1;
                    parent->_right = nullptr;
                }
                height_decreased = false;
            }
        }
        else { // decrease of 1 in the height of parent
            parent->_bf = 0;
            balance_pos = parent;
            parent->_left = parent->_right = nullptr;
        }
        return height_decreased;
    }


    node_ptr erase(node_ptr x) {
        assert(x != _header && "cannot erase end() iterator");
        node_ptr next = tree_next(x); // for return
        bool height_decreased = true;
        node_ptr balance_pos = x;

        if (_count == 1 && x == ROOT) {
            set_default_header();
            goto destroy_node;
        }
        if      (x == _header->_left ) _header->_left  = next;
        else if (x == _header->_right) _header->_right = tree_prev(x);

        if (x->_left && !x->_right) {
            replace(x, x->_left);
            balance_pos = x->_left;
        }
        else if (!x->_left && x->_right) {
            replace(x, x->_right);
            balance_pos = x->_right;
        }
        else if (!x->_left && !x->_right) { // x is a leaf
            height_decreased = unlink_leaf(x, balance_pos);
        }
        else { // has both children
            node_ptr r_min = tree_min(x->_right);
            swap_node(x, r_min);
            height_decreased = unlink_leaf(x, balance_pos);
        }

        if (height_decreased) rebalance_for_deletion_from(balance_pos);

    destroy_node:
        delete_node(x);
        --_count;
        return next;
    }

    // x has two children, r_min is the smallest on the right subtree of x.
    // Swap x with r_min first, if r_min is not a leaf (has right child) then
    // swap x with r_min's right child again. This makes sure x becomes a leaf.
    void swap_node(node_ptr x, node_ptr r_min) noexcept {
        node_ptr r_min_parent = r_min->_parent; // r_min is the left child
        node_ptr r_min_right = r_min->_right;
        replace(x, r_min);
        r_min->_bf = x->_bf;
        r_min->_left = x->_left;
        x->_left->_parent = r_min;
        if (r_min == x->_right) {
            if (r_min_right) { // is a leaf since r_min->_left is nil
                r_min_right->_right = x; // left or right are both okay
                x->_parent = r_min_right;
                r_min_right->_bf = 1; // as we chose right, otherwise -1
            }
            else {
                r_min->_right = x;
                x->_parent = r_min;
            }
        }
        else {
            r_min->_right = x->_right;
            x->_right->_parent = r_min;
            if (r_min_right) { // is a leaf since r_min->_left is nil
                r_min_parent->_left = r_min_right;
                r_min_right->_parent = r_min_parent;
                r_min_right->_right = x;
                x->_parent = r_min_right;
                r_min_right->_bf = 1;
            }
            else {
                r_min_parent->_left = x;
                x->_parent = r_min_parent;
            }
        }
    }

    // replace node x with a non-null node y
    void replace(node_ptr x, node_ptr y) noexcept {
        if (x == ROOT) ROOT = y;
        else if (x == x->_parent->_left)    x->_parent->_left = y;
        else /*  x == x->_parent->_right */ x->_parent->_right = y;
        y->_parent = x->_parent;
    }

    // precondition: x != ROOT
    static node_ptr sibling(node_ptr x) noexcept {
        return x == x->_parent->_left ? x->_parent->_right : x->_parent->_left;
    }

    static bool has_children(node_ptr x) noexcept {
        return x->_left || x->_right;
    }

#ifndef NDEBUG
    // print with a max level
    void print(node_ptr dir, size_t max_level) const {
        if (empty()) return; // to prevent infinite loop
        if (max_level == 0) {
            std::cout << "Invalid level, must be greater than 0.\n";
            return;
        }
        print_val_is_dir(dir, true);
        size_t dir_count = 0, file_count = 0, curr_level = 1;
        dfs_print(dir, "", dir_count, file_count, curr_level, max_level);
        std::cout << '\n' << dir_count << " directories, " << file_count << " files\n";
    }

#define BLUE    "\033[1;34m"
#define BROWN   "\033[0;33m"
#define END     "\033[0m"

    // map
    static void print_val_via_ptr(node_ptr x, bool is_dir, std::true_type) {
        if (is_dir) { // brown color for directories
            std::cout << BROWN << '{' << (x->_val).first << ", " << (x->_val).second << '}' << END << '\n';
        }             // default color for files
        else std::cout << '{' << (x->_val).first << ", " << (x->_val).second << "}\n";
    }

    // set
    static void print_val_via_ptr(node_ptr x, bool is_dir, std::false_type) {
        if (is_dir) {
            std::cout << BROWN << x->_val << END << '\n';
        }
        else std::cout << x->_val << '\n';
    }

    static void print_val_is_dir(node_ptr x, bool is_dir) {
        print_val_via_ptr(x, is_dir, std::bool_constant<IsMap>{});
    }

    static void dfs_print_aux(node_ptr curr, bool is_last_child, const std::string& children_prefix,
        size_t& dir_count, size_t& file_count, size_t curr_level, size_t max_level)
    {
        std::string link_shape = !is_last_child ? "├── " : "└── ";
        std::string prefix_shape = !is_last_child ? "|   " : "    ";

        std::cout << children_prefix << link_shape;
        if (curr == nullptr) {
            std::cout << BLUE << "null" << END << '\n';
            return;
        }
        if (has_children(curr)) {
            ++dir_count;
            print_val_is_dir(curr, true);
            if (curr_level < max_level)
                dfs_print(curr, children_prefix + prefix_shape, dir_count, file_count, curr_level + 1, max_level);
        }
        else {
            ++file_count;
            print_val_is_dir(curr, false);
        }
    }

    // precondition: dir = ROOT != _header or size() > 0
    static void dfs_print(node_ptr dir, const std::string& children_prefix, size_t& dir_count,
        size_t& file_count, size_t curr_level, size_t max_level)
    {
        if (dir == nullptr) return;
        // pseudocode
        /*for (node_ptr curr = first_child(dir); curr != end_child(dir); curr = next_child(dir, curr)) {
            bool is_last_child = curr == last_child(dir);
            dfs_print_aux(curr, is_last_child, children_prefix, dir_count, file_count, curr_level, max_level);
        }*/

        dfs_print_aux(dir->_left, false, children_prefix, dir_count, file_count, curr_level, max_level);
        dfs_print_aux(dir->_right, true, children_prefix, dir_count, file_count, curr_level, max_level);
    }
#endif

#undef BLUE
#undef BROWN
#undef END
#undef ROOT

    struct AVLtree_node {
        T _val;
        node_ptr _parent, _left, _right;
        int _bf = 0; // though acctually we will only need 3 bits, [-2, 2]

        AVLtree_node(const T& val, node_ptr parent, node_ptr left, node_ptr right)
            : _val(val), _parent(parent), _left(left), _right(right) {}

        // in case operator& is overloaded
        T* val_ptr() {
            return std::addressof(_val); // return &_val;
        }
        const T* val_ptr() const {
            return std::addressof(_val);
        }
    };

    static bool is_header(node_ptr x) {
        if (x->_left == x) return true; // container's empty
        if (x->_left == nullptr || x->_right == nullptr) return false;
        return x->_left->_parent != x;
    }

    class AVLtree_iter
    {
        using _self = AVLtree_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        AVLtree_iter() noexcept : _ptr(nullptr) {}
        AVLtree_iter(node_ptr ptr) noexcept : _ptr(ptr) {}

        node_ptr ptr() const noexcept { return _ptr; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            _ptr = tree_next(_ptr);
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _ptr = tree_next(_ptr);
            return tmp;
        }

        // if container's empty, _ptr->_right still gets itself (_header)
        // maybe we should assert(_ptr != _ptr->_left), i.e. !empty()
        _self& operator--() {
            if (is_header(_ptr)) _ptr = _ptr->_right;
            else                 _ptr = tree_prev(_ptr);
            return *this;
        }

        _self operator--(int) {
            _self tmp{ *this };
            --*this;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };

    class AVLtree_const_iter
    {
        using _self = AVLtree_const_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        AVLtree_const_iter() : _ptr(nullptr) {}
        AVLtree_const_iter(node_ptr ptr) : _ptr(ptr) {}
        AVLtree_const_iter(const AVLtree_iter& other) : _ptr(other.ptr()) {}

        node_ptr ptr() const noexcept { return _ptr; }

        reference operator*() const {
            return *_ptr->val_ptr();
        }

        pointer operator->() const {
            return _ptr->val_ptr();
        }

        _self& operator++() {
            _ptr = tree_next(_ptr);
            return *this;
        }

        _self operator++(int) {
            _self tmp{ *this };
            _ptr = tree_next(_ptr);
            return tmp;
        }

        _self& operator--() {
            if (is_header(_ptr)) _ptr = _ptr->_right;
            else                 _ptr = tree_prev(_ptr);
            return *this;
        }

        _self operator--(int) {
            _self tmp{ *this };
            --*this;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };
}; // class AVLtree

} // namespace mySymbolTable

#endif // !AVLTREE_IMPL_H