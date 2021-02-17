/*
 *  unordered symbol tables:
 *  Hash set and multiset
 *  see the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/tree/master/Searching/HashMap/HashSet.h
 */

#ifndef HASHSET_H
#define HASHSET_H 1

#include "Hashtable_impl.h"
#include <memory>     // std::allocator
#include <functional> // std::hash, std::equal_to
#include <initializer_list>

namespace mySymbolTable {

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>
> class HashSet : public Hashtable<Key, Hash, KeyEqual, Alloc, /*IsMap=*/false> {
    using _base = Hashtable<Key, Hash, KeyEqual, Alloc, /*IsMap=*/false>;
public:
    using key_type = Key;
    using value_type = Key;
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
    HashSet() : _base() {}

    // (1) b
    explicit HashSet( size_t bucket_count,
                      const Hash& hash = Hash(),
                      const key_equal& equal = key_equal(),
                      const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc) {}

    // (1) c
    HashSet(size_t bucket_count, const Alloc& alloc)
        : HashSet(bucket_count, Hash(), key_equal(), alloc) {}

    // (1) d
    HashSet(size_t bucket_count, const Hash& hash, const Alloc& alloc)
        : HashSet(bucket_count, hash, key_equal(), alloc) {}

    // (1) e
    explicit HashSet(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< typename InputIt >
    HashSet( InputIt first, InputIt last,
             size_t bucket_count = 2,
             const Hash& hash = Hash(),
             const key_equal& equal = key_equal(),
             const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert(first, last);
    }

    // (2) b
    template< typename InputIt >
    HashSet(InputIt first, InputIt last, size_t bucket_count, const Alloc& alloc)
        : HashSet(first, last, bucket_count, Hash(), key_equal(), alloc) {}

    // (2) c
    template< typename InputIt >
    HashSet(InputIt first, InputIt last, size_t bucket_count,
                                         const Hash& hash, const Alloc& alloc)
        : HashSet(first, last, bucket_count, hash, key_equal(), alloc) {}

    /* III */

    // (3) a
    HashSet(const HashSet& other) : _base(other) {}

    // (3) b
    HashSet(const HashSet& other, const Alloc& alloc) : _base(other, alloc) {}

    /* IV */

    // (4) a
    HashSet( std::initializer_list<value_type> init,
             size_t bucket_count = 2,
             const Hash& hash = Hash(),
             const key_equal& equal = key_equal(),
             const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert(init.begin(), init.end());
    }

    // (4) b
    template< typename InputIt >
    HashSet(std::initializer_list<value_type> init, size_t bucket_count, const Alloc& alloc)
        : HashSet(init, bucket_count, Hash(), key_equal(), alloc) {}

    // (4) c
    template< typename InputIt >
    HashSet(std::initializer_list<value_type> init, size_t bucket_count,
                                                    const Hash& hash, const Alloc& alloc)
        : HashSet(init, bucket_count, hash, key_equal(), alloc) {}


    HashSet& operator=(const HashSet& other) {
        _base::operator=(other);
        return *this;
    }

    HashSet& operator=(std::initializer_list<value_type> ilist) {
        HashSet tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* unique insertion for hash set */

    std::pair<iterator, bool> insert(const value_type& val) {
        return _base::insert(val);
    }

    template < typename InputIt >
    void insert(InputIt first, InputIt last) {
        return _base::insert(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert(ilist.begin(), ilist.end());
    }

    void swap(HashSet& rhs) {
        _base::swap(rhs);
    }

}; // class HashSet

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>
> void swap( HashSet<Key, Hash, KeyEqual, Alloc>& lhs,
             HashSet<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}


template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>
> class HashMultiset : public Hashtable<Key, Hash, KeyEqual, Alloc, /*IsMap=*/false> {
    using _base = Hashtable<Key, Hash, KeyEqual, Alloc, /*IsMap=*/false>;
public:
    using key_type = Key;
    using value_type = Key;
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
    HashMultiset() : _base() {}

    // (1) b
    explicit HashMultiset( size_t bucket_count,
                           const Hash& hash = Hash(),
                           const key_equal& equal = key_equal(),
                           const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc) {}

    // (1) c
    HashMultiset(size_t bucket_count, const Alloc& alloc)
        : HashMultiset(bucket_count, Hash(), key_equal(), alloc) {}

    // (1) d
    HashMultiset(size_t bucket_count, const Hash& hash, const Alloc& alloc)
        : HashMultiset(bucket_count, hash, key_equal(), alloc) {}

    // (1) e
    explicit HashMultiset(const Alloc& alloc) : _base(alloc) {}

    /* II */

    // (2) a
    template< typename InputIt >
    HashMultiset( InputIt first, InputIt last,
                  size_t bucket_count = 2,
                  const Hash& hash = Hash(),
                  const key_equal& equal = key_equal(),
                  const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert_multi(first, last);
    }

    // (2) b
    template< typename InputIt >
    HashMultiset(InputIt first, InputIt last, size_t bucket_count, const Alloc& alloc)
        : HashMultiset(first, last, bucket_count, Hash(), key_equal(), alloc) {}

    // (2) c
    template< typename InputIt >
    HashMultiset(InputIt first, InputIt last, size_t bucket_count,
                                              const Hash& hash, const Alloc& alloc)
        : HashMultiset(first, last, bucket_count, hash, key_equal(), alloc) {}

    /* III */

    // (3) a
    HashMultiset(const HashMultiset& other) : _base(other) {}

    // (3) b
    HashMultiset(const HashMultiset& other, const Alloc& alloc) : _base(other, alloc) {}

    /* IV */

    // (4) a
    HashMultiset( std::initializer_list<value_type> init,
                  size_t bucket_count = 2,
                  const Hash& hash = Hash(),
                  const key_equal& equal = key_equal(),
                  const Alloc& alloc = Alloc() )
        : _base(bucket_count, hash, equal, alloc)
    {
        _base::insert_multi(init.begin(), init.end());
    }

    // (4) b
    template< typename InputIt >
    HashMultiset(std::initializer_list<value_type> init, size_t bucket_count, const Alloc& alloc)
        : HashMultiset(init, bucket_count, Hash(), key_equal(), alloc) {}

    // (4) c
    template< typename InputIt >
    HashMultiset(std::initializer_list<value_type> init, size_t bucket_count,
                                                         const Hash& hash, const Alloc& alloc)
        : HashMultiset(init, bucket_count, hash, key_equal(), alloc) {}


    HashMultiset& operator=(const HashMultiset& other) {
        _base::operator=(other);
        return *this;
    }

    HashMultiset& operator=(std::initializer_list<value_type> ilist) {
        HashMultiset tmp{ ilist };
        this->swap(tmp);
        return *this;
    }

    /* duplicable insertion for hash multiset */

    iterator insert(const value_type& val) {
        return _base::insert_multi(val);
    }

    template < typename InputIt >
    void insert(InputIt first, InputIt last) {
        return _base::insert_multi(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        return _base::insert_multi(ilist.begin(), ilist.end());
    }

    void swap(HashMultiset& rhs) {
        _base::swap(rhs);
    }

}; // class HashMultiset

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>
> void swap( HashMultiset<Key, Hash, KeyEqual, Alloc>& lhs,
             HashMultiset<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

} // namespace mySymbolTable

#endif // !HASHSET_H

