#pragma once

#include <functional>

namespace myRankingAlgo {

enum class RankMethod { Min, Max, Average };

template <typename T, typename Compare = std::less<T>>
class RankTree {
public:
    RankTree(Compare comp = Compare()) : comp_(comp) {}

    ~RankTree() { clear(); }

    int size() const { return root_ ? root_->count : 0; }

    void clear() {
        clear(root_);
        root_ = nullptr;
    }

    // return the highest rank (see rank_max) of the inserted element.
    int insert(const T &value) { return insert_node(&root_, value); }

    void remove(const T &value) { remove_node(&root_, value); }

    int rank_min(const T &value) const { return rank_min(root_, value); }

    int rank_max(const T &value) const { return rank_max(root_, value); }

private:
    struct Node {
        T value;
        int count;
        Node *left;
        Node *right;
    };

    Node *root_ = nullptr;
    Compare comp_;

    void clear(Node *root) {
        if (root) {
            clear(root->left);
            clear(root->right);
            delete root;
        }
    }

    int insert_node(Node **root, const T &value) {
        int rank_max = 0;
        while (*root) {
            (*root)->count++;
            if (comp_(value, (*root)->value)) {
                root = &(*root)->left;
            } else {
                rank_max += 1 + ((*root)->left ? (*root)->left->count : 0);
                root = &(*root)->right;
            }
        }
        *root = new Node{value, 1, nullptr, nullptr};
        return rank_max + 1;  // +1 for this inserted one
    }

    void remove_node(Node **root, const T &value) {
        if (*root == nullptr) {
            return;
        }

        if (comp_(value, (*root)->value)) {
            remove_node(&(*root)->left, value);
        } else if (comp_((*root)->value, value)) {
            remove_node(&(*root)->right, value);
        } else {
            if ((*root)->left == nullptr) {
                Node *tmp = (*root)->right;
                delete *root;
                *root = tmp;
            } else if ((*root)->right == nullptr) {
                Node *tmp = (*root)->left;
                delete *root;
                *root = tmp;
            } else {
                Node *min_node = find_min((*root)->right);
                (*root)->value = min_node->value;
                remove_node(&(*root)->right, min_node->value);
            }
        }

        if (*root) {
            update_size(*root);
        }
    }

    int rank_min(Node *root, const T &value) const {
        int rank = root ? root->count : 0;
        while (root) {
            if (comp_(root->value, value)) {
                root = root->right;
            } else {
                rank -= 1 + (root->right ? root->right->count : 0);
                root = root->left;
            }
        }
        return rank + 1;
    }

    int rank_max(Node *root, const T &value) const {
// #define DO_RECURSIVE_RANKING
#if defined(DO_RECURSIVE_RANKING)
        if (root == nullptr) {
            return 0;
        }

        if (comp_(value, root->value)) {
            return rank_max(root->left, value);
        } else {
            return 1 + (root->left ? root->left->count : 0) +
                   rank_max(root->right, value);
        }
#else
        int rank = 0;
        while (root) {
            if (comp_(value, root->value)) {
                root = root->left;
            } else {
                rank += 1 + (root->left ? root->left->count : 0);
                root = root->right;
            }
        }
        return rank;
#endif
    }

    Node *find_min(Node *root) const {
        if (root == nullptr) {
            return nullptr;
        }
        while (root->left != nullptr) {
            root = root->left;
        }
        return root;
    }

    int size(Node *root) const { return root == nullptr ? 0 : root->count; }

    // precondition: root != nullptr
    void update_size(Node *root) {
        root->count = 1 + size(root->left) + size(root->right);
    }
};

// -fcode-hoisting enabled by -O2 will move the if (asc) outside the loop :)
template <typename T>
double rank_naive(const T *data, RankMethod method, bool asc, int row_idx,
                  int begin, int end) {
    int rank = 1;
    int equal = 0;  // excluding itself
    for (int i = begin; i < end; ++i) {
        if (i == row_idx) {
            continue;
        }
        if (asc) {
            if (data[i] < data[row_idx]) {
                rank++;
            }
        } else {
            if (data[i] > data[row_idx]) {
                rank++;
            }
        }
        if (data[i] == data[row_idx]) {
            equal++;
        }
    }

    if (method == RankMethod::Min) {
        return rank;
    } else if (method == RankMethod::Max) {
        return rank + equal;
    } else {
        return rank + equal / 2.0;
    }
}

template <typename T, bool Asc = true>
void rolling_rank(double *res, RankMethod method, const T *arr, int arr_size,
                  int window) {
    RankTree<T, std::conditional_t<Asc, std::less<T>, std::greater<T>>>
        rank_tree;
#if defined(IS_TALIB_RESULT)
    // for ta_res, the buffer is only of size `arr_size - window + 1`
    res -= window - 1;
#endif

    for (int i = 0; i < arr_size; i++) {
        if (i >= window) {
            rank_tree.remove(arr[i - window]);
        }

        const double rank_max = rank_tree.insert(arr[i]);

        if (i < window - 1) {
#if defined(IS_TALIB_RESULT)
            continue;
#else
            res[i] = -1;
#endif
        } else {
            switch (method) {
            case RankMethod::Min:
                res[i] = rank_tree.rank_min(arr[i]);
                break;
            case RankMethod::Max:
                // res[i] = rank_tree.rank_max(arr[i]);
                res[i] = rank_max;  // already cached
                break;
            case RankMethod::Average: {
                const double rank_min = rank_tree.rank_min(arr[i]);
                res[i] = rank_min + (rank_max - rank_min) / 2.0;
                break;
            }
            default:
                break;
            }
        }
    }
}

}  // namespace myRankingAlgo
