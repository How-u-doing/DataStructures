/*
 *  unordered symbol tables:
 *  Hash map and multimap
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/HashMap/HashMap.h
 */

#ifndef HASHMAP_H
#define HASHMAP_H 1

#include "Hashtable_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::hash, std::equal_to
#include <stdexcept>  // std::out_of_range
#include <initializer_list>

namespace mySymbolTable {

template<
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, T>>
> class HashMap : public Hashtable<std::pair<const Key, T>, Hash, KeyEqual, Alloc, /*IsMap=*/true> {
    using _base = Hashtable<std::pair<const Key, T>, Hash, KeyEqual, Alloc, /*IsMap=*/true>;
public:
    using key_type = Key;
    using value_type = std::pair<const Key, T>;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = typename _base::iterator;
    using const_iterator = typename _base::const_iterator;
    using local_iterator = typename _base::local_iterator;
    using const_local_iterator = typename _base::const_local_iterator;
    using node_type = typename _base::node_type;

    /* I */

    // (1) a
    HashMap() : _base() {}

    // (1) b
    explicit HashMap( size_t bucket_count,
                      const Hash& hash = Hash(),
                      const key_equal& equal = key_equal(),
                      const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc) {}

    // (1) c
    HashMap(size_t bucket_count, const Alloc& alloc)
        : HashMap(bucket_count, Hash(), key_equal(), alloc) {}

    // (1) d
    HashMap(size_t bucket_count, const Hash& hash, const Alloc& alloc)
        : HashMap(bucket_count, hash, key_equal(), alloc) {}

    // (1) e
    explicit HashMap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< typename InputIt >
    HashMap( InputIt first, InputIt last,
             size_t bucket_count = 1,
             const Hash& hash = Hash(),
             const key_equal& equal = key_equal(),
             const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert_unique(first, last);
    }

    // (2) b
    template< typename InputIt >
    HashMap(InputIt first, InputIt last, size_t bucket_count, const Alloc& alloc)
        : HashMap(first, last, bucket_count, Hash(), key_equal(), alloc) {}

    // (2) c
    template< typename InputIt >
    HashMap(InputIt first, InputIt last, size_t bucket_count,
                                         const Hash& hash, const Alloc& alloc)
        : HashMap(first, last, bucket_count, hash, key_equal(), alloc) {}

    /* III */

    // (3) a
    HashMap(const HashMap& other) : _base(other) {}

    // (3) b
    HashMap(const HashMap& other, const Alloc& alloc) : _base(other, alloc) {}

    /* IV */

    // (4) a
    HashMap( std::initializer_list<value_type> init,
             size_t bucket_count = 1,
             const Hash& hash = Hash(),
             const key_equal& equal = key_equal(),
             const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert_unique(init.begin(), init.end());
    }

    // (4) b
    template< typename InputIt >
    HashMap(std::initializer_list<value_type> init, size_t bucket_count, const Alloc& alloc)
        : HashMap(init, bucket_count, Hash(), key_equal(), alloc) {}

    // (4) c
    template< typename InputIt >
    HashMap(std::initializer_list<value_type> init, size_t bucket_count,
                                                    const Hash& hash, const Alloc& alloc)
        : HashMap(init, bucket_count, hash, key_equal(), alloc) {}


    HashMap& operator=(const HashMap& other) {
        _base::operator=(other);
        return *this;
    }

    HashMap& operator=(std::initializer_list<value_type> ilist) {
        HashMap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* element access */

    T& at(const Key& key) {
        iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("HashMap<K, T> key does not exist");
        return it->second;
    }

    const T& at(const Key& key) const {
        const_iterator it = this->find(key);
        if (it == this->end()) throw std::out_of_range("HashMap<K, T> key does not exist");
        return it->second;
    }

    T& operator[](const Key& key) {
        iterator it = this->find(key);
        if (it == this->end())
            it = _base::insert_unique({ key, T() }).first;
        return it->second;
    }

    /* unique insertion for hash map */

    std::pair<iterator, bool> insert(const value_type& val) {
        return _base::insert_unique(val);
    }

    std::pair<iterator, bool> insert(const Key& key, const T& val) {
        return _base::insert_unique({ key, val });
    }

    template < typename InputIt >
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
        iterator it = _base::find(key);
        if (it == this->end()) return 0;
        _base::erase(it);
        return 1;
    }

    void swap(HashMap& rhs) {
        _base::swap(rhs);
    }

}; // class HashMap

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>
> void swap( HashMap<Key, Hash, KeyEqual, Alloc>& lhs,
             HashMap<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, T>>
> class HashMultimap : public Hashtable<std::pair<const Key, T>, Hash, KeyEqual, Alloc, /*IsMap=*/true> {
    using _base = Hashtable<std::pair<const Key, T>, Hash, KeyEqual, Alloc, /*IsMap=*/true>;
public:
    using key_type = Key;
    using value_type = std::pair<const Key, T>;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = typename _base::iterator;
    using const_iterator = typename _base::const_iterator;
    using local_iterator = typename _base::local_iterator;
    using const_local_iterator = typename _base::const_local_iterator;
    using node_type = typename _base::node_type;

    /* I */

    // (1) a
    HashMultimap() : _base() {}

    // (1) b
    explicit HashMultimap( size_t bucket_count,
                           const Hash& hash = Hash(),
                           const key_equal& equal = key_equal(),
                           const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc) {}

    // (1) c
    HashMultimap(size_t bucket_count, const Alloc& alloc)
        : HashMultimap(bucket_count, Hash(), key_equal(), alloc) {}

    // (1) d
    HashMultimap(size_t bucket_count, const Hash& hash, const Alloc& alloc)
        : HashMultimap(bucket_count, hash, key_equal(), alloc) {}

    // (1) e
    explicit HashMultimap(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< typename InputIt >
    HashMultimap( InputIt first, InputIt last,
                  size_t bucket_count = 1,
                  const Hash& hash = Hash(),
                  const key_equal& equal = key_equal(),
                  const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert_multi(first, last);
    }

    // (2) b
    template< typename InputIt >
    HashMultimap(InputIt first, InputIt last, size_t bucket_count, const Alloc& alloc)
        : HashMultimap(first, last, bucket_count, Hash(), key_equal(), alloc) {}

    // (2) c
    template< typename InputIt >
    HashMultimap(InputIt first, InputIt last, size_t bucket_count,
                                              const Hash& hash, const Alloc& alloc)
        : HashMultimap(first, last, bucket_count, hash, key_equal(), alloc) {}

    /* III */

    // (3) a
    HashMultimap(const HashMultimap& other) : _base(other) {}

    // (3) b
    HashMultimap(const HashMultimap& other, const Alloc& alloc) : _base(other, alloc) {}

    /* IV */

    // (4) a
    HashMultimap( std::initializer_list<value_type> init,
                  size_t bucket_count = 1,
                  const Hash& hash = Hash(),
                  const key_equal& equal = key_equal(),
                  const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert_multi(init.begin(), init.end());
    }

    // (4) b
    template< typename InputIt >
    HashMultimap(std::initializer_list<value_type> init, size_t bucket_count, const Alloc& alloc)
        : HashMultimap(init, bucket_count, Hash(), key_equal(), alloc) {}

    // (4) c
    template< typename InputIt >
    HashMultimap(std::initializer_list<value_type> init, size_t bucket_count,
                                                         const Hash& hash, const Alloc& alloc)
        : HashMultimap(init, bucket_count, hash, key_equal(), alloc) {}


    HashMultimap& operator=(const HashMultimap& other) {
        _base::operator=(other);
        return *this;
    }

    HashMultimap& operator=(std::initializer_list<value_type> ilist) {
        HashMultimap tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* duplicable insertion for hash multimap */

    iterator insert(const value_type& val) {
        return _base::insert_multi(val);
    }

    iterator insert(const Key& key, const T& val) {
        return _base::insert_multi({ key, val });
    }

    template < typename InputIt >
    void insert(InputIt first, InputIt last) {
        return _base::insert_multi(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert_multi(ilist.begin(), ilist.end());
    }

    void swap(HashMultimap& rhs) {
        _base::swap(rhs);
    }

}; // class HashMultimap

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>
> void swap( HashMultimap<Key, Hash, KeyEqual, Alloc>& lhs,
             HashMultimap<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !HASHMAP_H