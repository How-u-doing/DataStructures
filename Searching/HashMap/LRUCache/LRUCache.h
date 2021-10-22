#ifndef LRUCACHE_H
#define LRUCACHE_H 1

#include "../HashMap.h"
#include <iterator>

namespace myst = mySymbolTable;

template<
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, T>>
> class LRUCache {
    struct CacheEntry {
        struct Data {
            Key first;
            T second;
            Data(const Key& key, const T& val) : first(key), second(val) {}
        } data_;
        CacheEntry *prev_, *next_;
        CacheEntry(const Key& key, const T& val, CacheEntry *prev, CacheEntry *next)
            : data_(key, val), prev_(prev), next_(next) {}
    };

    size_t capacity_;
    CacheEntry *head_ = nullptr; // points to least recently used entry
    CacheEntry *tail_ = nullptr; // points to most  recently used entry
    myst::HashMap<Key, CacheEntry, Hash, KeyEqual, Alloc> lru_cache_;
    /*
     *     [ A  B  C  D  E  F  G  H ]
     *       ^                    ^
     *       |                    |
     *  LRU(head_)           MRU(tail_)
     *
     */
public:
    class iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;

    LRUCache(size_t capacity) : capacity_(capacity) {}

    size_t capacity () const noexcept {
        return capacity_;
    }

    size_t size() const noexcept {
        return lru_cache_.size();
    }

    iterator get(const Key& key) {
        auto it = lru_cache_.find(key);
        if (it != lru_cache_.end()) {
            CacheEntry *entry = &it->second;
            move_entry_to_tail(entry);
            return iterator(entry, this);
        }
        return end();
    }

    bool put(const Key& key, const T& val) {
        auto it = lru_cache_.find(key);
        if (it != lru_cache_.end()) {
            CacheEntry *entry = &it->second;
            entry->data_.second = val;
            move_entry_to_tail(entry);
            return true; // write hit
        }
        else {
            if (lru_cache_.size() == capacity_) {
                CacheEntry *new_head = head_->next_;
                if (new_head) new_head->prev_ = nullptr; // cap >= 1
                else /* cap == 1 */ tail_ = nullptr;
                lru_cache_.erase(head_->data_.first);
                head_ = new_head;
            }
            CacheEntry *entry = &lru_cache_.insert(key, CacheEntry(
                key, val, tail_, nullptr)).first->second;
            if (tail_) tail_ = tail_->next_ = entry;
            else /* head_ == tail_ == 0 */ head_ = tail_ = entry;
            return false; // write miss
        }
    }

private:
    void move_entry_to_tail(CacheEntry *entry) {
        if (tail_ != entry) {
            entry->next_->prev_ = entry->prev_;
            if (entry->prev_) entry->prev_->next_ = entry->next_;
            else /* head_ == entry */ head_ = entry->next_;
            tail_->next_ = entry;
            entry->prev_ = tail_;
            entry->next_ = nullptr;
            tail_ = entry;
        }
    }

public:
    iterator begin() noexcept {
        return iterator(head_, this);
    }

    iterator end() noexcept {
        return iterator(nullptr, this);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    class iterator {
        CacheEntry *ptr_ = nullptr;
        LRUCache *cache_ = nullptr;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename CacheEntry::Data;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() {}
        iterator(CacheEntry *entry, LRUCache *cache) : ptr_(entry), cache_(cache) {}
#if 0
        bool has_next() const {
            return ptr_ != cache_->tail_;
        }

        bool has_prev() const {
            return ptr_ != cache_->head_;
        }
#endif
        reference operator*() const {
            return ptr_->data_;
        }

        pointer operator->() const {
            return &ptr_->data_;
        }

        iterator& operator++() {
            ptr_ = ptr_->next_;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp{ *this };
            ptr_ = ptr_->next_;
            return tmp;
        }

        iterator& operator--() {
            if (ptr_) ptr_ = ptr_->prev_;
            else      ptr_ = cache_->tail_;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp{ *this };
            operator--();
            return tmp;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return lhs.ptr_ != rhs.ptr_;
        }
    };
};

#endif
