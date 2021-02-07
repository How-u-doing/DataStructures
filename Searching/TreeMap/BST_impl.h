/*
 *  internal header file for implementing
 *  ordered symbol tables:
 *  BST (multi) map/set
 *  see following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/BST_impl.h
 */

#ifndef BST_IMPL_H
#define BST_IMPL_H 1

#include <memory>   // std::addressof, std::allocator_tarits
#include <utility>  // std::swap, std::pair
#include <string>
#include <iterator> // std::reverse_iterator, std::distance
#include <iostream> // std::cout

namespace mySymbolTable{

#ifndef NDEBUG
#   define _assert(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define _assert(condition, message) do { } while (false)
#endif


template<typename T, bool IsMap>
struct get_map_key_t {
    using key_type = typename T::first_type;
};

template<typename T>
struct get_map_key_t<T, false> {
    using key_type = T;
};

// Binary Search Tree
template<typename T, typename Compare, typename Alloc, bool IsMap>
class Tree {
    struct Tree_node;
    class Tree_iter;
    class Tree_const_iter;
    using _self = Tree<T, Compare, Alloc, IsMap>;
    using node = Tree_node;
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
    using iterator = Tree_iter;
    using const_iterator = Tree_const_iter;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = Tree_node;

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
        try {
            _header = _alloc.allocate(1);
            set_default_header();
            // leave the value field uninitialized
        }
        catch (...) {
            _alloc.deallocate(_header, 1);
            throw;
        }
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
    Tree() : _count(0), _comp(), _alloc() {
        create_header();
    }

    explicit Tree(const Compare& comp, const Alloc& alloc = Alloc()) :
        _count(0), _comp(comp), _alloc(alloc) {
        create_header();
    }

    explicit Tree(const Alloc& alloc) : _count(0), _comp(), _alloc(alloc) {}

    Tree(const _self& rhs) : _count(rhs._count), _comp(rhs._comp), _alloc(rhs._alloc) {
        create_header();
        copy(rhs);
    }

    ~Tree() { clear(); _alloc.deallocate(_header, 1); }

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
        auto r = equal_range(key);
        return std::distance(r.first, r.second);
    }

    // Returns iterator pointing to the first (highest) element (in the order of
    // operator++) that compares equivalent to val, end() is returned if not found.
    iterator find(const key_type& val) {
        if (empty()) return end();
        return iterator(find(ROOT, val));
    }
    
    const_iterator find(const key_type& val) const {
        if (empty()) return end();
        return const_iterator(find(ROOT, val));
    }

    bool contains(const key_type& val) const {
        return empty() ? false : find(ROOT, val) != _header;
    }

    // Returns iterator pointing to the first element that is not less than val.
    // Returns end() if not found.
    iterator lower_bound(const key_type& val) {
        if (empty()) return end();
        return iterator(lower_bound(ROOT, val));
    }

    const_iterator lower_bound(const key_type& val) const {
        if (empty()) return end();
        return const_iterator(lower_bound(ROOT, val));
    }

    // Returns iterator pointing to the first element that is greater than val.
    // Returns end() if not found.
    iterator upper_bound(const key_type& val) {
        if (empty()) return end();
        return iterator(upper_bound(ROOT, val));
    }
    
    const_iterator upper_bound(const key_type& val) const {
        if (empty()) return end();
        return const_iterator(upper_bound(ROOT, val));
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
            clear_nodes(ROOT); _count = 0; set_default_header();
        }
    }

protected:
    // for non-duplicate map (set)
    std::pair<iterator, bool> insert(const T& val) {
        const auto& [p, inserted] = insert_unique(&ROOT, val);
        return { iterator(p), inserted };
    }

    template< typename InputIt >
    void insert(InputIt first, InputIt last) {
        while (first != last) {
            insert(*first++);
        }
    }

    // only for map
    std::pair<iterator, bool> insert_or_assign(const T& val) {
        const auto& [p, inserted] = insert_or_assign(&ROOT, val);
        return { iterator(p), inserted };
    }

    // for multi map (set)
    iterator insert_multi(const T& val) {
        return iterator(insert_multi(&ROOT, val));
    }

    template< typename InputIt >
    void insert_multi(InputIt first, InputIt last) {
        while (first != last) {
            insert_multi(*first++);
        }
    }

public:
    // References and iterators to the erased elements are invalidated.
    // Other references and iterators are not affected.
    // Retrurns iterator following the last removed element.
    iterator erase(iterator pos) {
        return iterator(erase(pos.ptr()));
    }

    iterator erase(const_iterator pos) {
        return iterator(erase(pos.ptr()));
    }

    // Returns iterator following the last removed element.
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

    // Returns the number of elements removed.
    size_t erase(const key_type& val) {
        auto r = equal_range(val);
        size_t n = std::distance(r.first, r.second);
        erase(r.first, r.second);
        return n;
    }

    void swap(Tree& rhs) noexcept(std::allocator_traits<Alloc>::is_always_equal::value
                           &&     std::is_nothrow_swappable<Compare>::value)
    {
        _assert(_alloc == rhs._alloc, "allocator must be the same");
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_alloc, rhs._alloc);
        } // otherwise the behavior is undefined

        std::swap(_header, rhs._header);
        std::swap(_count, rhs._count);
        std::swap(_comp, rhs._comp);
    }

    /* visualization */

    // print in a tree structure
    void print(size_t level = 10) const { print(ROOT, level); }

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

    // equivalent to `new node(val, parent, left = 0, right = 0)`
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

    // precondition: ROOT is dereferencable, i.e. size() > 0
    node_ptr find(node_ptr x, const key_type& val) const {
        while (x != nullptr) {
            if      (_comp(val, get_key(x))) x = x->_left;
            else if (_comp(get_key(x), val)) x = x->_right;
            else return x;
        }
        return _header; // end(), not found
    }

    // precondition: x != NULL && size() > 0
    node_ptr lower_bound(node_ptr x, const key_type& val) const {
        node_ptr parent = x->_parent;
        while (x != nullptr) {
            if (_comp(get_key(x), val)) x = x->_right;
            else {// x.val >= val
                parent = x;
                x = x->_left;
            }
        }
        return parent; // _header if not found
    }
        
    node_ptr upper_bound(node_ptr x, const key_type& val) const {
        node_ptr parent = x->_parent;
        while (x != nullptr) {
            if (!_comp(val, get_key(x))) x = x->_right;
            else {// x.val > val
                parent = x;
                x = x->_left;
            }
        }
        return parent; // _header if not found
    }

    void insert_uniquely_at(node** x, const T& val, node_ptr parent) {
        *x = new_node(val, parent);
        ++_count;
        if (_count == 1) {
            _header->_parent = _header->_left = _header->_right = *x;
        }
        else {
            if (*x == _header->_left->_left  ) _header->_left  = *x;
            if (*x == _header->_right->_right) _header->_right = *x;
        }
    }
    
    // only for map
    std::pair<node_ptr, bool> insert_or_assign(node** x, const T& val) {
        node_ptr parent = (*x)->_parent;
        if (_count > 0) {
            while (*x != nullptr) {
                parent = *x;
                if      (_comp(get_key(val), get_key(*x))) x = &(*x)->_left;
                else if (_comp(get_key(*x), get_key(val))) x = &(*x)->_right;
                else {
                    ((*x)->_val).second = val.second;
                    return { *x, false };
                }
            }
        }
        insert_uniquely_at(x, val, parent);        
        return { *x, true };
    }

    std::pair<node_ptr, bool> insert_unique(node** x, const T& val) {
        node_ptr parent = (*x)->_parent;
        if (_count > 0) {
            while (*x != nullptr) {
                parent = *x;
                if      (_comp(get_key(val), get_key(*x))) x = &(*x)->_left;
                else if (_comp(get_key(*x), get_key(val))) x = &(*x)->_right;
                else return { *x, false };
            }
        }
        insert_uniquely_at(x, val, parent);
        return { *x, true };
    }

    node_ptr insert_multi(node** x, const T& val) {
        node_ptr parent = (*x)->_parent;
        if (_count > 0) {
            while (*x != nullptr) {
                parent = *x;
                if      (_comp(get_key(val), get_key(*x))) x = &(*x)->_left;
                else if (_comp(get_key(*x), get_key(val))) x = &(*x)->_right;
                else break;
            }
        }
        if (_count == 0 || *x == nullptr) {
            insert_uniquely_at(x, val, parent);
        }
        else { // *x points to first node that compares equally to `val`
            // duplicates are maintained in this way: a->b->c->d->e->...
            /* 
                     20
                   /     \
                  10     (20)a
                         /  \
                    (20)b   (20)c
                           /  \
                        (20)d  (20)e
                                 \
                                  30
            */
            bool equal = true;
            while (equal) {
                parent = *x;
                x = &(*x)->_right;
                if (*x == nullptr) {// insert duplicate at leaf
                    *x = new_node(val, parent);
                    ++_count;
                    if (parent == _header->_right) _header->_right = *x;
                    return *x;
                }
                equal = !_comp(get_key(val), get_key(*x)) && !_comp(get_key(*x), get_key(val));
                if (equal && (*x)->_left == nullptr) {
                    parent = *x;
                    x = &(*x)->_left;
                    *x = new_node(val, parent);
                    ++_count;
                    return *x;
                }
            }
            // Now *x points to a different val, say 30 in above example,
            // and we want to insert (20)e in between 30 and (20)c.
            // We need also update the child's parent to inserted node.
            node** y = &(*x)->_parent;
            *x = new_node(val, parent, nullptr, *x); ++_count;
            *y = *x;
        }
        return *x;
    }

    void set_parent_child(node_ptr x, node_ptr val) noexcept {
        if (x == x->_parent->_left)
            x->_parent->_left = val;
        else if (x == x->_parent->_right)
            x->_parent->_right = val;
        else // x == ROOT
            ROOT = val == nullptr ? _header : val;
    }

    void set_left_child_parent(node_ptr x, node_ptr val) noexcept {
        if (x->_left != nullptr) {
            x->_left->_parent = val;
        }
    }

    void copy_node_link(node_ptr dest, node_ptr src) noexcept {
        dest->_parent = src->_parent;
        dest->_left = src->_left;
        dest->_right = src->_right;
    }

    // precondition: x != nulllptr
    node_ptr erase(node_ptr x) {
        _assert(x != _header, "cannot erase end() iterator");
        if (x == _header->_left ) _header->_left  = tree_next(x);
        if (x == _header->_right) _header->_right = tree_prev(x);
        node_ptr next = tree_next(x); // for return

        node_ptr r_min = x->_right == nullptr ? nullptr : tree_min(x->_right);
        if (r_min == nullptr) {// x->_right == nullptr
            set_parent_child(x, x->_left);
            set_left_child_parent(x, x->_parent);
        }
        else if (r_min == x->_right) {// x->_right->_left == nullptr
            r_min->_parent = x->_parent;
            r_min->_left = x->_left; // copy_node_link(r_min, x) will result in a circle
            set_parent_child(x, r_min);
            set_left_child_parent(x, r_min);
        }
        else { // r_min->_left == nullptr
            if (r_min->_right == nullptr) {// r_min is leaf
                r_min->_parent->_left = nullptr;
            }
            else {// hast right child
                r_min->_parent->_left = r_min->_right;
                r_min->_right->_parent = r_min->_parent;
            }
            copy_node_link(r_min, x);
            set_parent_child(x, r_min);
            set_left_child_parent(x, r_min);
            x->_right->_parent = r_min;
        }

        delete_node(x);
        --_count;
        return next;
    }

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

    // precondition: dir != nullptr
    static bool has_children(node_ptr dir) noexcept {
        return dir->_left != nullptr || dir->_right != nullptr;
    }

#undef BLUE
#undef BROWN
#undef END

    struct Tree_node {
        T _val;
        node_ptr _parent, _left, _right;

        Tree_node(T val, node_ptr parent, node_ptr left = nullptr, node_ptr right = nullptr) :
            _val(val), _parent(parent), _left(left), _right(right) {}

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

    class Tree_iter
    {
        using _self = Tree_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Tree_iter() noexcept : _ptr(nullptr) {}
        Tree_iter(node_ptr ptr) noexcept : _ptr(ptr) {}

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

    class Tree_const_iter
    {
        using _self = Tree_const_iter;
        node_ptr _ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        Tree_const_iter() : _ptr(nullptr) {}
        Tree_const_iter(node_ptr ptr) : _ptr(ptr) {}
        Tree_const_iter(const Tree_iter& other) : _ptr(other.ptr()) {}

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
}; // class Tree

} // namespace mySymbolTable

#endif // !BST_IMPL_H
