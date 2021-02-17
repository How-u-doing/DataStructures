/*
 *  TST ordered symbol table with key specialized as string
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/TST.h
 */
#ifndef TST_H
#define TST_H
#include <string>
#include <vector>
#include <utility>   // std::pair, std::make_pair
#include <iterator>  // std::reverse_iterator
#include <algorithm> // std::reverse_copy
#include <iostream>  // std::cerr, std::cout
#include <stdexcept>
#include <cassert>

namespace mySymbolTable {

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

// Ternary Search Tree symbol table: key=string -> T
template<typename T>
class TST {
    class Tst_iter;
    class Tst_const_iter;
    class Tst_reverse_iter;
    class Tst_const_reverse_iter;

    enum class Link : char { LEFT, MID, RIGHT };

    struct Node {
        char ch = '\0';        // put ch and pos together so that they will take
        Link pos = Link::MID;  // only 4 bytes (padding with another 2 bytes)
        T* pval = nullptr;     // here we use a pointer instead of an object entity given that internal
                               // nodes don't need to store objects (and thus saving memory)
        Node* parent = nullptr;
        Node* left{}, * mid{}, * right{};

        Node() {}
        Node(char c) : ch(c) {}
        Node(char c, Link pos, Node* parent) : ch(c), pos(pos), parent(parent) {}
        ~Node() { delete pval; }
    };
    using node_ptr = Node*;

    node_ptr root;   // pointer to root node whose link is MID
    size_t n;        // no. of keys in TST
public:
    using iterator = Tst_iter;
    using const_iterator = Tst_const_iter;
    using reverse_iterator = Tst_reverse_iter;
    using const_reverse_iterator = Tst_const_reverse_iter;

    TST() : root(nullptr), n(0) {}

    ~TST() { clear(); }

    void clear() { clear(root); root = nullptr; }

    T& operator[](const std::string& key) {
        if (key == "") throw std::invalid_argument("key to operator[] cannot be null");
        return *(insert(key, T())->pval);
    }

    const T& at(const std::string& key) const {
        if (key == "") throw std::invalid_argument("key to at() cannot be null");
        node_ptr x = find_aux(key);
        if (x == nullptr || x->pval == nullptr) // e.g. find "shell" in "she", or reverse
            throw std::out_of_range("invalid key to at()");
        return *(x->pval);
    }

    T& at(const std::string& key) {
        if (key == "") throw std::invalid_argument("key to at() cannot be null");
        node_ptr x = find_aux(key);
        if (x == nullptr || x->pval == nullptr) // e.g. find "shell" in "she", or reverse
            throw std::out_of_range("invalid key to at()");
        return *(x->pval);
    }

    iterator begin() noexcept {
        return iterator(leftmost(root), this);
    }

    const_iterator begin() const noexcept {
        return const_iterator(leftmost(root), this);
    }

    iterator end() noexcept {
        return iterator(nullptr, this);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr, this);
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
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    size_t size() const noexcept { return n; }

    bool empty() const noexcept { return n == 0; }

    bool contains(const std::string& key) const {
        if (key == "") throw std::invalid_argument("key to contains() cannot be null");
        if (node_ptr x = find_aux(key))    return x->pval != nullptr;
        return false;
    }

    std::vector<std::string> keys() const {
        std::vector<std::string> vs;
        collect(root, "", vs);
        return vs; // it'll be totally fine to return large object thanks to RVO
    }

    std::vector<std::string> keys_with_prefix(const std::string& prefix) const {
        if (prefix == "") return keys();
        std::vector<std::string> vs;
        node_ptr x = find_aux(prefix);
        if (x == nullptr) return vs; // no matches, return empty vector
        if (x->pval != nullptr)    vs.push_back(prefix);
        collect(x->mid, prefix, vs);
        return vs; // it'll be totally fine to return large object due to RVO
    }

    // using wildcard ? to represent any single character
    // e.g. file?.h matches first two in {file1.h, file2.h, file3.cc}
    std::vector<std::string> keys_that_match(const std::string& pattern) const {
        if (pattern == "") throw std::invalid_argument("pattern to keys_that_match() cannot be null");
        std::vector<std::string> vs;
        collect(root, "", pattern, 0, vs);
        return vs;
    }

    std::string longest_prefix_of(const std::string& query) {
        if (query == "") throw std::invalid_argument("query to longest_prefix_of() cannot be null");
        size_t len = max_len(root, query, 0, 0); // 0 if no substring of query
        return query.substr(0, len);
    }

    node_ptr insert(const std::string& key, const T& val) {
        if (key == "") throw std::invalid_argument("key to insert() cannot be null");
        return insert(root, key, val, 0);
    }

    node_ptr insert_or_assign(const std::string& key, const T& val) {
        if (key == "") throw std::invalid_argument("key to insert_or_assign() cannot be null");
        return insert_or_assign(root, key, val, 0);
    }

    void erase(const std::string& key) {
        if (key == "") throw std::invalid_argument("key to erase() cannot be null");
        erase(root, key, 0);
    }

    // return end() if not found
    iterator find(const std::string& key) {
        if (key == "") throw std::invalid_argument("key to find() cannot be null");
        return iterator(find_aux(key), this);
    }

    const_iterator find(const std::string& key) const {
        if (key == "") throw std::invalid_argument("key to find() cannot be null");
        return const_iterator(find_aux(key), this);
    }

    static std::string key(iterator iter) {
        return get_key(iter.ptr());
    }

    static std::string key(const_iterator iter) {
        return get_key(iter.ptr());
    }

    /* visualization */

    void print(bool hide_null_links = false, size_t level = 10) const {
        print(root, hide_null_links, level);
    }

    /* tree height interface */

    // tree height (height of root),  i.e. the number of
    // edges on the longest path from root node to a leaf
    int height() const {
        return height(root) - 1; // -1 if empty
    }

    int height(const_iterator pos) const {
        return height(pos.ptr()) - 1;
    }

private:
    static int height(node_ptr x) {
        if (x == nullptr) return 0;
        return 1 + max(max(height(x->left), height(x->right)), height(x->mid));
    }

    static int max(int a, int b) noexcept {
        return a > b ? a : b;
    }

    // print with a max level
    void print(node_ptr dir, bool hide_null_links, size_t max_level) const {
        if (dir == nullptr) return;
        if (max_level == 0) {
            std::cout << "Invalid level, must be greater than 0.\n";
            return;
        }
        print_val(dir, hide_null_links);
        size_t curr_level = 1, nodes = 1, keys = dir->pval ? 1 : 0;
        dfs_print(dir, "", keys, nodes, hide_null_links, curr_level, max_level);
        std::cout << '\n' << keys << " keys, " << nodes << " nodes\n";
    }

#define GREEN   "\033[0;32m"
#define BROWN   "\033[0;33m"
#define BLUE    "\033[1;34m"
#define END     "\033[0m"

    static void dfs_print(node_ptr dir, const std::string& children_prefix,
        size_t& keys, size_t& nodes, bool hide_null_links, size_t curr_level, size_t max_level)
    {
        if (dir == nullptr) return;
        // pseudocode
        /*for (node_ptr curr = first_child(dir); curr != end_child(dir); curr = next_child(dir, curr)) {
            bool is_last_child = curr == last_child(dir);
            dfs_print_aux(curr, is_last_child, children_prefix, keys, nodes, hide_null_links, curr_level, max_level);
        }*/

        bool is_left_last = hide_null_links ? (!dir->mid && !dir->right) : false;
        bool is_mid_last = hide_null_links ? !dir->right : false;

        dfs_print_aux(dir->left, is_left_last, children_prefix, keys, nodes, hide_null_links, curr_level, max_level);
        dfs_print_aux(dir->mid,  is_mid_last,  children_prefix, keys, nodes, hide_null_links, curr_level, max_level);
        dfs_print_aux(dir->right, true, children_prefix, keys, nodes, hide_null_links, curr_level, max_level);
    }

    static void print_val(node_ptr x, bool hide_null_links) {
        char pos = x->pos == Link::LEFT ? 'L' :
                   x->pos == Link::MID  ? 'M' : 'R';

        if (x->pval) {
            std::cout << GREEN << x->ch << END;
            if (hide_null_links) std::cout << BLUE << " (" << pos << ')' << END;
            std::cout << ": " << BROWN << "{ " << get_key(x) << ", " << *x->pval << " }" << END;
        }
        else {
            std::cout << x->ch;
            if (hide_null_links) std::cout << BLUE << " (" << pos << ')' << END;
        }
        std::cout << '\n';
    }

    static void dfs_print_aux(node_ptr curr, bool is_last_child, const std::string& children_prefix,
        size_t& keys, size_t& nodes, bool hide_null_links, size_t curr_level, size_t max_level)
    {
        if (hide_null_links && curr == nullptr)  return;
        std::string link_shape = !is_last_child ? "├── " : "└── ";
        std::string prefix_shape = !is_last_child ? "|   " : "    ";
        std::cout << children_prefix << link_shape;
        if (!hide_null_links && curr == nullptr) {
            std::cout << BLUE << "null" << END << '\n';
            return;
        }
        ++nodes;
        if (curr->pval) ++keys;
        print_val(curr, hide_null_links);
        if (has_children(curr) && curr_level < max_level) {
            dfs_print(curr, children_prefix + prefix_shape, keys, nodes, hide_null_links, curr_level + 1, max_level);
        }
    }

    // precondition: x != nullptr
    static bool has_children(node_ptr x) noexcept {
        return x->left || x->right || x->mid;
    }

#undef GREEN
#undef BROWN
#undef BLUE
#undef END

private:

    // precondition: key != "" (null string)
    node_ptr find_aux(const std::string& key) const {
        return find_aux(root, key, 0);
    }

    static std::string get_key(node_ptr x) {
        std::vector<char> str;
        for (Link pos; x != nullptr; x = x->parent) {// root->parent == nullptr
            str.push_back(x->ch);
            pos = x->pos;
            while (pos != Link::MID) {
                x = x->parent;
                pos = x->pos;
            }
        }
        std::string key(str.size(), '\0');
        std::reverse_copy(str.begin(), str.end(), key.begin());
        return key;
    }

    // precondition: x != nullptr
    static node_ptr leftmost(node_ptr x) noexcept {
        for (;;) {
            if      (x->left != nullptr) x = x->left;
            else if (x->pval != nullptr) return x;
            else if (x->mid  != nullptr) x = x->mid;
            else x = x->right;
        }
    }

    // precondition: x != nullptr
    static node_ptr rightmost(node_ptr x) noexcept {
        for (;;) {
            if      (x->right != nullptr) x = x->right;
            else if (x->mid   != nullptr) x = x->mid;
            else if (x->pval  != nullptr) return x;
            else x = x->left;
        }
    }

    // precondition: x != nullptr
    static node_ptr tree_next(node_ptr x) noexcept {
        if (x->mid   != nullptr) return leftmost(x->mid);
        if (x->right != nullptr) return leftmost(x->right);
        // go backward
        for (Link pos;  true ; /* empty */) {
            pos = x->pos;
            x = x->parent;
            if (x == nullptr) return nullptr; // argument x points to the last node
            if (pos == Link::LEFT) {
                if (x->pval  != nullptr) return x;
                if (x->mid   != nullptr) return leftmost(x->mid);
                if (x->right != nullptr) return leftmost(x->right);
            }
            else if (pos == Link::MID) {
                if (x->right != nullptr) return leftmost(x->right);
            }
            else  /* pos == Link::RIGHT */ continue;
        }
    }

    // precondition: x != nullptr
    static node_ptr tree_prev(node_ptr x) noexcept {
        if (x->left != nullptr) return rightmost(x->left);
        // go backward
        for (Link pos;  true ; /* empty */) {
            pos = x->pos;
            x = x->parent;
            if (x == nullptr) return nullptr; // argument x points to the first node
            if (pos == Link::RIGHT) {
                if (x->mid  != nullptr) return rightmost(x->mid);
                if (x->pval != nullptr) return x;
                if (x->left != nullptr) return rightmost(x->left);
            }
            else if (pos == Link::MID) {
                if (x->pval != nullptr) return x;
                if (x->left != nullptr) return rightmost(x->left);
            }
            else  /* pos == Link::LEFT */ continue;
        }
    }

    // precondition: x != nullptr
    static bool is_leaf(node_ptr x) noexcept {
        return !(x->left || x->mid || x->right);
    }
        
    // return pointer to key node, null if not found
    static node_ptr find_aux(node_ptr x, const std::string& key, size_t d) {
        while (x != nullptr) {
            if      (key[d] < x->ch)    x = x->left;
            else if (key[d] > x->ch)    x = x->right;
            else if (d < key.length() - 1) { x = x->mid; ++d; }
            else return x;
        }
        return nullptr;
    }

    // no overwriting if key already exists
    // return pointer to new inserted node or key node
    node_ptr insert(node_ptr& x, const std::string& key, const T& val, size_t d) {
        // e.g. insert "shell" in "she", "shore" in "shell"
        Node** curr = &x; // *curr is a reference to pointer
        Link pos = Link::MID; // link value for root if it is null
        node_ptr parent = nullptr;
        for (;;) {
            if (*curr == nullptr) *curr = new Node(key[d], pos, parent);
            parent = *curr;
            if      (key[d] < (*curr)->ch) { curr = & (*curr)->left;  pos = Link::LEFT; }
            else if (key[d] > (*curr)->ch) { curr = & (*curr)->right; pos = Link::RIGHT; }
            else if (d < key.length() - 1) { curr = & (*curr)->mid;   pos = Link::MID; ++d; }
            else { // found
                if ((*curr)->pval == nullptr) {
                    (*curr)->pval = new T(val); ++n;
                } // else do nothing
                return *curr;
            }
        }
    }

    // overwrite if key already exists
    // return pointer to new inserted or overwritten node
    node_ptr insert_or_assign(node_ptr& x, const std::string& key, const T& val, size_t d) {
        // e.g. insert "shell" in "she", "shore" in "shell"
        Node** curr = &x; // *curr is a reference to pointer
        Link pos = Link::MID; // link value for root if it is null
        node_ptr parent = nullptr;
        for (;;) {
            if (*curr == nullptr) *curr = new Node(key[d], pos, parent);
            parent = *curr;
            if      (key[d] < (*curr)->ch) { curr = & (*curr)->left;  pos = Link::LEFT; }
            else if (key[d] > (*curr)->ch) { curr = & (*curr)->right; pos = Link::RIGHT; }
            else if (d < key.length() - 1) { curr = & (*curr)->mid;   pos = Link::MID; ++d; }
            else { // found
                if ((*curr)->pval == nullptr) {
                    (*curr)->pval = new T(val); ++n;
                }
                else *((*curr)->pval) = val; // overwrite
                return *curr;
            }
        }
    }

    // precondition: query != "" (null string)
    // return the length of the longest key in sub-tst that is a substring of query
    size_t max_len(node_ptr x, const std::string& query, size_t d, size_t len) const {
        for (;;) {
            if (x == nullptr) return len;
            if      (query[d] < x->ch) x = x->left;
            else if (query[d] > x->ch) x = x->right;
            else {// query[d] == x->ch
                if (x->pval != nullptr) len = d + 1;
                if (d == query.length() - 1) return len;
                x = x->mid; ++d;
            }
        }
    }

    // remove key and its associated value in TST, if any
    void erase(node_ptr& x, const std::string& key, size_t d) {
        // e.g. remove "shells" or "she" in  {"shells", "she"}
        if (x == nullptr) return;
        if      (key[d] < x->ch) erase(x->left,  key, d);
        else if (key[d] > x->ch) erase(x->right, key, d);
        else if (d < key.length() - 1) erase(x->mid, key, d + 1);
        else {
            if (x->pval != nullptr) { // found
                delete x->pval;    --n;
                x->pval = nullptr;
            }
        }

        if (is_leaf(x) && x->pval == nullptr) {
            delete x; x = nullptr;
        }
    }

    void clear(node_ptr x) {
        if (x == nullptr) return;
        clear(x->left);
        clear(x->mid);
        clear(x->right);
        delete x;
    }

    // collect keys in TST rooted at x with given prefix
    // note that x points to the mid link node of prefix node (if any)
    void collect(node_ptr x, const std::string& prefix, std::vector<std::string>& vs) const {
        if (x == nullptr) return;
        collect(x->left, prefix, vs);
        if (x->pval != nullptr) vs.push_back(prefix + x->ch);
        collect(x->mid, prefix + x->ch, vs);
        collect(x->right, prefix, vs);
    }

    // precondition: pattern != "" (null string)
    // note that x points to the mid link node of prefix node (if any)
    void collect(node_ptr x, const std::string& prefix, const std::string& pattern,
        size_t i, std::vector<std::string>& vs) const {
        // e.g. collect  file?.h  in {file1.h, file2.h, file3.cc}
        if (x == nullptr) return;
        assert(i < pattern.length());

        char curr = pattern[i];
        if (curr < x->ch || curr == '?') collect(x->left, prefix, pattern, i, vs);
        if (curr == x->ch || curr == '?') {
            if (i == pattern.length() - 1 && x->pval != nullptr) vs.push_back(prefix + x->ch);
            if (i < pattern.length() - 1) collect(x->mid, prefix + x->ch, pattern, i + 1, vs);
        }
        if (curr > x->ch || curr == '?') collect(x->right, prefix, pattern, i, vs);
    }

    class Tst_iter
    {
        using _self = Tst_iter;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const std::string, T>;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        //using reference = value_type&; // g++ uses `reference` to determine the type of operator*()
        using reference = value_type;    // but we can fool it :)

        Tst_iter() : _ptr(nullptr), _ptree(nullptr) {}
        Tst_iter(node_ptr ptr, const TST* ptree) : _ptr(ptr), _ptree(ptree) {}

        value_type operator*() const {
            _assert(_ptr != nullptr, "cannot dereference end() iterator");
            return std::make_pair(get_key(_ptr), *(_ptr->pval));
        }

        // pointer operator->() const = delete;

        _self& operator++() {
            _assert(_ptr != nullptr, "cannot increment end() iterator");
            _ptr = tree_next(_ptr);
            return *this;
        }

        _self operator++(int) {
            _assert(_ptr != nullptr, "cannot increment end() iterator");
            _self tmp{ *this };
            _ptr = tree_next(_ptr);
            return tmp;
        }

        // --begin() returns end() (its pointer becomes nullptr)
        _self& operator--() {
            if (_ptr == nullptr) _ptr = rightmost(_ptree->root);
            else                 _ptr = tree_prev(_ptr);
            return *this;
        }

        // begin()-- returns a copy of begin(), then itself becomes end()
        _self operator--(int) {
            _self tmp{ *this };
            --*this;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            _assert(lhs._ptree == rhs._ptree, "iterators incompatible");
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            _assert(lhs._ptree == rhs._ptree, "iterators incompatible");
            return lhs._ptr != rhs._ptr;
        }

        // auxiliary functions
        node_ptr ptr() const noexcept { return _ptr; }
        const TST* cont() const noexcept { return _ptree; }    // get container

        std::string key() const {
            _assert(_ptr != nullptr, "cannot get the key of end() iterator");
            return get_key(_ptr);
        }

        T& val() {
            _assert(_ptr != nullptr, "cannot get/set the value of end() iterator");
            return *(_ptr->pval);
        }

        const T& val() const {
            _assert(_ptr != nullptr, "cannot get the value of end() iterator");
            return *(_ptr->pval);
        }
    private:
        node_ptr _ptr;
        const TST* _ptree;        
    };

    class Tst_const_iter
    {
        using _self = Tst_const_iter;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const std::string, T>;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type;

        Tst_const_iter() : _ptr(nullptr), _ptree(nullptr) {}
        Tst_const_iter(node_ptr ptr, const TST* ptree) : _ptr(ptr), _ptree(ptree) {}
        Tst_const_iter(const Tst_iter& other) : _ptr(other.ptr()), _ptree(other.cont()) {}

        // we cannot return a const reference since it's a temporary
        // so return type cannot be reference (const value_type&)
        const value_type operator*() const {
            _assert(_ptr != nullptr, "cannot dereference end() iterator");
            return std::make_pair(get_key(_ptr), *(_ptr->pval));
        }

        // pointer operator->() const = delete;

        _self& operator++() {
            _assert(_ptr != nullptr, "cannot increment end() iterator");
            _ptr = tree_next(_ptr);
            return *this;
        }

        _self operator++(int) {
            _assert(_ptr != nullptr, "cannot increment end() iterator");
            _self tmp{ *this };
            _ptr = tree_next(_ptr);
            return tmp;
        }

        // --begin() returns end() (its pointer becomes nullptr)
        _self& operator--() {
            if (_ptr == nullptr) _ptr = rightmost(_ptree->root);
            else                 _ptr = tree_prev(_ptr);
            return *this;
        }

        // begin()-- returns a copy of begin(), then itself becomes end()
        _self operator--(int) {
            _self tmp{ *this };
            --*this;
            return tmp;
        }

        friend bool operator==(const _self& lhs, const _self& rhs) {
            _assert(lhs._ptree == rhs._ptree, "iterators incompatible");
            return lhs._ptr == rhs._ptr;
        }

        friend bool operator!=(const _self& lhs, const _self& rhs) {
            _assert(lhs._ptree == rhs._ptree, "iterators incompatible");
            return lhs._ptr != rhs._ptr;
        }

        // auxiliary functions
        node_ptr ptr() const noexcept { return _ptr; }
        const TST* cont() const noexcept { return _ptree; }

        std::string key() const {
            _assert(_ptr != nullptr, "cannot get the key of end() iterator");
            return get_key(_ptr);
        }

        const T& val() const {
            _assert(_ptr != nullptr, "cannot get the value of end() iterator");
            return *(_ptr->pval);
        }
    private:
        node_ptr _ptr;
        const TST* _ptree;
    };

    class Tst_reverse_iter : public std::reverse_iterator<iterator> {
        using _base = std::reverse_iterator<iterator>;
    public:
        Tst_reverse_iter() : _base() {}
        Tst_reverse_iter(node_ptr ptr, const TST* ptree) : _base(ptr, ptree) {}
        Tst_reverse_iter(const iterator& it) : _base(it) {}

        // auxiliary functions
        node_ptr ptr() const noexcept { return _base::base().ptr(); }
        const TST* cont() const noexcept { return _base::base().cont(); }

        std::string key() const {
            return (--_base::base()).key();
        }

        T& val() {
            return (--_base::base()).val();
        }

        const T& val() const {
            return (--_base::base()).val();
        }
    };

    class Tst_const_reverse_iter : public std::reverse_iterator<const_iterator> {
        using _base = std::reverse_iterator<const_iterator>;
    public:
        Tst_const_reverse_iter() : _base() {}
        Tst_const_reverse_iter(node_ptr ptr, const TST* ptree) : _base(ptr, ptree) {}
        //Tst_const_reverse_iter(const iterator& it) : _base(it) {} // implicit
        Tst_const_reverse_iter(const const_iterator& it) : _base(it) {} // crucial
        Tst_const_reverse_iter(const reverse_iterator& it) : _base(it) {}

        // auxiliary functions
        node_ptr ptr() const noexcept { return _base::base().ptr(); }
        const TST* cont() const noexcept { return _base::base().cont(); }

        std::string key() const {
            return (--_base::base()).key();
        }

        const T& val() const {
            return (--_base::base()).val();
        }
    };
};

} // namespace mySymbolTable

#endif // !TST_H