/*
 *  internal header file for implementing
 *  ordered symbol tables:
 *  Red-black BST (multi) map/set
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/RBtree_impl.h
 */

#ifndef RBTREE_IMPL_H
#define RBTREE_IMPL_H 1

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

enum class RBtree_color { red, black };

// Red-black binary search trees, an isometry of 2-3-4 trees
template<typename T, typename Compare, typename Alloc, bool IsMap, bool IsMulti>
class RBtree {
    struct RBtree_node;
    class RBtree_iter;
    class RBtree_const_iter;
    using _self = RBtree<T, Compare, Alloc, IsMap, IsMulti>;
    using node = RBtree_node;
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
    using iterator = RBtree_iter;
    using const_iterator = RBtree_const_iter;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = RBtree_node;

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
        _header->_color = RBtree_color::red;
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
    RBtree() : _count(0), _comp(), _alloc() {
        create_header();
    }

    explicit RBtree(const Compare& comp, const Alloc& alloc = Alloc())
        : _count(0), _comp(comp), _alloc(alloc) {
        create_header();
    }

    explicit RBtree(const Alloc& alloc) : _count(0), _comp(), _alloc(alloc) {
        create_header();
    }

    RBtree(const _self& rhs) : _count(rhs._count), _comp(rhs._comp), _alloc(rhs._alloc) {
        create_header();
        copy(rhs);
    }

    ~RBtree() { clear(); _alloc.deallocate(_header, 1); }

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

    void swap(RBtree& rhs) noexcept(std::allocator_traits<Alloc>::is_always_equal::value
                              &&    std::is_nothrow_swappable<Compare>::value)
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

    static bool is_red(node_ptr x) noexcept {
        return x->_color == RBtree_color::red;
    }

#ifndef NDEBUG
public:
    /* visualization */

    // print tree by lines
    void print(size_t level = 10) const { print(ROOT, level); }

    /* debug */

    bool is_rb_tree() const {
        if (_count < 3) return true;
        return are_colors_ok(ROOT) && is_balanced(ROOT);
    }

private:
    static bool are_colors_ok(node_ptr x) {
        if (x == nullptr) return true;
        if (is_red(x) && ((x->_left && is_red(x->_left)) || (x->_right && is_red(x->_right))))
            return false;
        return are_colors_ok(x->_left) && are_colors_ok(x->_right);
    }

    static bool is_balanced(node_ptr x) {
        int black = 0;
        for (node_ptr y = x; y; y = y->_left) {
            if (!is_red(y)) ++black;
        }
        return has_same_blacks(x, black);
    }

    static bool has_same_blacks(node_ptr x, int black) {
        if (x == nullptr) return black == 0;
        if (!is_red(x)) --black;
        return has_same_blacks(x->_left,  black) &&
               has_same_blacks(x->_right, black);
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

    // precondition: x != nullptr
    static node_ptr tree_prev(node_ptr x) noexcept {
        if (x == x->_parent->_parent && is_red(x)) // x == _header
            return x->_right;
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

    node_ptr new_node(const T& val, RBtree_color color, node_ptr parent,
                      node_ptr left = nullptr, node_ptr right = nullptr)
    {
        node_ptr p = _alloc.allocate(1);
        try {
            ::new ((void*)p) node(val, color, parent, left, right);
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
            node_ptr t = new_node(x->_val, x->_color, parent);
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
        *x = new_node(val, RBtree_color::red, parent);
        ++_count;
        if (_count == 1) {
            (*x)->_color = RBtree_color::black;
            return _header->_parent = _header->_left = _header->_right = *x;
        }
        if      (*x == _header->_left->_left  ) _header->_left  = *x;
        else if (*x == _header->_right->_right) _header->_right = *x;
        node_ptr newnode = *x; // rebalancing may change the link *x
        if (is_red(parent)) rebalance_after_inserting(*x);
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

    /*
             a                                 b
           /   \         rotate left         /   \
          X     b       ------------>       a     Z
               / \       around  a         / \
              Y   Z                       X   Y
    */
    static void rotate_left(node_ptr a) noexcept {
        node_ptr b = a->_right;
        a->_right = b->_left;
        if (b->_left) b->_left->_parent = a;
        replace(a, b);
        b->_left = a;
        a->_parent = b;
    }

    // mirror image of `rotate_left`
    static void rotate_right(node_ptr a) noexcept {
        node_ptr b = a->_left;
        a->_left = b->_right;
        if (b->_right) b->_right->_parent = a;
        replace(a, b);
        b->_right = a;
        a->_parent = b;
    }

    // x.color == BLACK, x.children.color == RED
    // flip their colors
    static void flip_colors(node_ptr x) noexcept {
        x->_color = RBtree_color::red;
        x->_left->_color = x->_right->_color = RBtree_color::black;
    }

    void rebalance_after_inserting(node_ptr x) noexcept {
        while (x != ROOT && is_red(x->_parent)) {
            // x->_parent can't be ROOT since its color is red while
            // ROOT's is black. Therefore, x's grandparent is valid.
            node_ptr x_pp = x->_parent->_parent;
            if (x->_parent == x_pp->_left) {
                if (x_pp->_right && is_red(x_pp->_right)) {
                    flip_colors(x_pp);
                    x = x_pp;
                }
                else { // uncle's BLACK or is null
                    if (x == x->_parent->_right) {
                        x = x->_parent;
                        rotate_left(x);
                    }
                    rotate_right(x_pp);
                    x->_parent->_color = RBtree_color::black;
                    x_pp->_color = RBtree_color::red;
                }
            }
            else { // mirror image of previous big if-then statement
                if (x_pp->_left && is_red(x_pp->_left)) {
                    flip_colors(x_pp);
                    x = x_pp;
                }
                else {
                    if (x == x->_parent->_left) {
                        x = x->_parent;
                        rotate_right(x);
                    }
                    rotate_left(x_pp);
                    x->_parent->_color = RBtree_color::black;
                    x_pp->_color = RBtree_color::red;
                }
            }
        }
        ROOT->_color = RBtree_color::black;
    }

    // canonical implementation according to CLRS
    node_ptr erase(node_ptr z) noexcept {
        assert(z != _header && "cannot erase end() iterator");
        node_ptr next = tree_next(z); // for return
        node_ptr y = z, x = nullptr, x_parent = nullptr;
        RBtree_color y_original_color = y->_color;

        if (_count == 1 && z == ROOT) {
            set_default_header();
            goto destroy_node;
        }
        if      (z == _header->_left ) _header->_left  = next;
        else if (z == _header->_right) _header->_right = tree_prev(z);

        if (!z->_left) { // z has at most one non-null child
            x = z->_right; // might be null
            x_parent = z->_parent;
            replace(z, x);
        }
        else if (!z->_right) { // z has exactly one non-null child
            x = z->_left; // not null
            x_parent = z->_parent;
            replace(z, x);
        }
        else { // has both children
            y = tree_min(z->_right); // node to be effectively deleted
            y_original_color = y->_color;
            x = y->_right; // might be null, y's left is null
            if (y == z->_right) {
                x_parent = y;
            }
            else {
                x_parent = y->_parent;
                replace(y, x);
                y->_right = z->_right;
                z->_right->_parent = y;
            }
            replace(z, y);
            y->_left = z->_left;
            z->_left->_parent = y;
            y->_color = z->_color;
        }

        // rebalance
        if (y_original_color == RBtree_color::black) {
            while (x != ROOT && (x == nullptr || !is_red(x))) {
                if (x == x_parent->_left) {
                    node_ptr w = x_parent->_right;
                    // case 1: w is red, then convert it to case 2-4 (w is black)
                    if (is_red(w)) {
                        rotate_left(x_parent);
                        x_parent->_color = RBtree_color::red;
                        w->_color = RBtree_color::black;
                        w = x_parent->_right;
                    }
                    // case 2: both children of w are black (or null)
                    if ((!w->_left || !is_red(w->_left)) && (!w->_right || !is_red(w->_right))) {
                        // Push one black off both x & w up to x.p and test x.p's color on the next round.
                        // If x.p's color is red-black (indicated by RED) then the loop terminates and we
                        // assign it's color to BLACK, otherwise (doubly black) keep going from x.p.
                        w->_color = RBtree_color::red;
                        x = x_parent;
                        x_parent = x->_parent;
                    }
                    else {
                        // case 3: only w's right child is black, left child is red (thus non-null)
                        if (!w->_right || !is_red(w->_right)) {
                            // "Move" the left red child of w to the right, then we will arrive at case 4,
                            // which can be balanced by performing a left rotation on x.p and changing the
                            // new root's (w's) color to x.p's color and assigning its two children's colors
                            // (x.p's and w's right child's which's red) to black. In this way, we can think
                            // of it as one black out of x's double blacks is shifted to x.p and thus we are
                            // done. Tada :)
                            w->_left->_color = RBtree_color::black; // should be performed before rotation
                                                                    // since w->_left changes as we rotate
                            w->_color = RBtree_color::red;
                            rotate_right(w);
                            w = x_parent->_right;
                        }
                        // case 4: w's right child is red
                        rotate_left(x_parent);
                        w->_color = x_parent->_color;
                        x_parent->_color = RBtree_color::black;
                        w->_right->_color = RBtree_color::black;
                        break;
                    }
                }
                else { // same as above with _left <-> _right
                    node_ptr w = x_parent->_left;
                    // case 1: w is red, then convert it to case 2-4 (w is black)
                    if (is_red(w)) {
                        rotate_right(x_parent);
                        x_parent->_color = RBtree_color::red;
                        w->_color = RBtree_color::black;
                        w = x_parent->_left;
                    }
                    // case 2: both children of w are black (or null)
                    if ((!w->_right || !is_red(w->_right)) && (!w->_left || !is_red(w->_left))) {
                        w->_color = RBtree_color::red;
                        x = x_parent;
                        x_parent = x->_parent;
                    }
                    else {
                        // case 3: only w's left child is black, right child is red (thus non-null)
                        if (!w->_left || !is_red(w->_left)) {
                            w->_right->_color = RBtree_color::black;
                            w->_color = RBtree_color::red;
                            rotate_left(w);
                            w = x_parent->_left;
                        }
                        // case 4: w's left child is red
                        rotate_right(x_parent);
                        w->_color = x_parent->_color;
                        x_parent->_color = RBtree_color::black;
                        w->_left->_color = RBtree_color::black;
                        break;
                    }
                }
            }
            if (x) x->_color = RBtree_color::black;
        }

    destroy_node:
        delete_node(z);
        --_count;
        return next;
    }

    // replace node x with node y
    static void replace(node_ptr x, node_ptr y) noexcept {
        node_ptr xp = x->_parent;
        if (x == xp->_parent && is_red(xp)) xp->_parent = y;
        else if (x == xp->_left)    xp->_left  = y;
        else /*  x == xp->_right */ xp->_right = y;
        if (y) y->_parent = xp;
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
        print_val(dir);
        size_t dir_count = 0, file_count = 0, curr_level = 1;
        dfs_print(dir, "", dir_count, file_count, curr_level, max_level);
        std::cout << '\n' << dir_count << " directories, " << file_count << " files\n";
    }

#define BLUE  "\033[1;34m"
#define RED   "\033[0;31m"
#define END   "\033[0m"

    // map
    static void print_val_via_ptr(node_ptr x, std::true_type) {
        if (is_red(x)) {
            std::cout << RED << '{' << (x->_val).first << ", " << (x->_val).second << '}' << END << '\n';
        }
        else std::cout << '{' << (x->_val).first << ", " << (x->_val).second << "}\n";
    }

    // set
    static void print_val_via_ptr(node_ptr x, std::false_type) {
        if (is_red(x)) {
            std::cout << RED << x->_val << END << '\n';
        }
        else std::cout << x->_val << '\n';
    }

    static void print_val(node_ptr x) {
        print_val_via_ptr(x, std::bool_constant<IsMap>{});
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
        print_val(curr);
        if (has_children(curr)) {
            ++dir_count;
            if (curr_level < max_level)
                dfs_print(curr, children_prefix + prefix_shape, dir_count, file_count, curr_level + 1, max_level);
        }
        else {
            ++file_count;
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
#undef RED
#undef END
#undef ROOT

    struct RBtree_node {
        T _val;
        RBtree_color _color;
        node_ptr _parent, _left, _right;

        RBtree_node(const T& val, RBtree_color color, node_ptr parent, node_ptr left, node_ptr right)
            : _val(val), _color(color), _parent(parent), _left(left), _right(right) {}

        // in case operator& is overloaded
        T* val_ptr() {
            return std::addressof(_val); // return &_val;
        }
        const T* val_ptr() const {
            return std::addressof(_val);
        }
    };

    class RBtree_iter
    {
        using _self = RBtree_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        RBtree_iter() noexcept : _ptr(nullptr) {}
        RBtree_iter(node_ptr ptr) noexcept : _ptr(ptr) {}

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
            _ptr = tree_prev(_ptr);
            return *this;
        }

        _self operator--(int) {
            _self tmp{ *this };
            _ptr = tree_prev(_ptr);
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };

    class RBtree_const_iter
    {
        using _self = RBtree_const_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        RBtree_const_iter() : _ptr(nullptr) {}
        RBtree_const_iter(node_ptr ptr) : _ptr(ptr) {}
        RBtree_const_iter(const RBtree_iter& other) : _ptr(other.ptr()) {}

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
            _ptr = tree_prev(_ptr);
            return *this;
        }

        _self operator--(int) {
            _self tmp{ *this };
            _ptr = tree_prev(_ptr);
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            return lhs._ptr != rhs._ptr;
        }
    };
}; // class RBtree

} // namespace mySymbolTable

#endif // !RBTREE_IMPL_H
