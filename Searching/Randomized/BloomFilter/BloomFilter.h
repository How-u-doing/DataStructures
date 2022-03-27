#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "Bitmap.h"
#include <functional> // std::hash
#include <vector>
#include <random>
#include <stdexcept>
#include <cmath>
#include <cstdint>

/*
 * Bloom filters are space-efficient probabilistic data structures, which can
 * be used for high-speed membership tests to detect if an element is in a set,
 * with some small false positive probability.
 *
 * There're many awesome applications of Bloom filters, for exmaple, a web
 * crawler can detect if a URL is added to the queue (which will then be
 * processed by the seach engine for indexing) without storing the whole URL
 * string which can be quite costly (the queue itself of course contains those
 * URLs, but the telling phase does not have to store the URLs, as opposed to
 * a hash set which has to store a key because in case of hash collisions, the
 * key must be compared to decide if a key is present in the hash set/table).
 * Note that Bloom filters may lie, if they say a URL doesn't in the set then
 * it's definitely not in it; while if they say otherwise (yes, it's in here)
 * you should be doubted (no, it's maybe in or maybe not in there). It's okay
 * in this case since the false positive answers are acceptable to the web
 * crawler, given there're a HUGE number (billions) of web URLs, it doesn't
 * have to collect every single URL. Besides, contents in a website can be
 * updated. So the web crawler should do web crawling regularly. In practice,
 * some well-known websites will be given a high priority for crawling and
 * indexing by the search engine.
 *
 * Another application I found interesting is full-text search. Say you have a
 * blog and many articles therein, and of course you want to provide a search
 * engine for visitors (users) to search through your blog to find the articles
 * that contain the specific topics (words) they are interested in. For example,
 * someone may want to see the articles that contains the words 'UNIX shell',
 * 'opencv python', or "your girlfriend's name" :)
 * There's a great article about how to write a full-text search engine using
 * Bloom filters, see https://www.stavros.io/posts/bloom-filter-search-engine/.
 *
 * And there're tons of other applications:
 *      reducing the space required in probabilistic routing tables;
 *      speeding longest-prefix matching for IP addresses;
 *      improving network state management and monitoring;
 *      encoding multicast forwarding information in packets, etc.
 * (These are stolen from the paper
 * https://www.cs.cmu.edu/~dga/papers/cuckoo-conext2014.pdf
 * which introduced a more advanced probabilistic data structure,
 * cuckoo filters, which permit deletion of items from the set.)
 * Deletion is not allowed without rebuilding the filter is because
 * deleting an item from the set (erase some bits) may lead to a
 * false answer (not present) when querying an existing item.
 * For example, suppose the length of the bit vector is 10, and we
 * have inserted item 'apple' with hash codes 2, 7, 9, and item
 * 'orange' with hash codes 3, 7, 8. Now if we delete 'apple' from
 * the set, bit 2, 7, 9 will be set to 0 and then if we try to test
 * if item 'orange' is in the set, oops, oh damn, it's not in there.
 *
 * About how to choose the optimal number of hash functions please
 * see 'BloomFilter.ipynb' (some math involved).
 * See also the excellent wiki page
 * https://en.wikipedia.org/wiki/Bloom_filter.
 */
template<typename T>
class BloomFilter {
    typedef uint32_t uint32;

    size_t _n; // input capacity
    int    _k; // number of hash functions
    size_t _count = 0;
    Bitmap _bitmap;
    std::vector<uint32> _hash_keys;

    // taken from https://chromium.googlesource.com/chromium/chromium/+/refs/heads/main/chrome/browser/safe_browsing/bloom_filter.cc
    static uint32 hash_mix(uint32 hash_key, uint32 c) {
        uint32 a = hash_key;
        uint32 b = 0;
        a -= (b + c);  a ^= (c >> 13);
        b -= (c + a);  b ^= (a << 8);
        c -= (a + b);  c ^= (b >> 13);
        a -= (b + c);  a ^= (c >> 12);
        b -= (c + a);  b ^= (a << 16);
        c -= (a + b);  c ^= (b >> 5);
        a -= (b + c);  a ^= (c >> 3);
        b -= (c + a);  b ^= (a << 10);
        c -= (a + b);  c ^= (b >> 15);
        return c;
    }

public:
    BloomFilter(size_t capacity, float error_rate) : _n(capacity) {
        if (error_rate <= 0 || error_rate >= 1)
            throw std::invalid_argument("invalid error_rate: must be in (0, 1)");
        // Note that unary minus has a higher precedence than multiplication!
        // So DON'T write:  - _n * std::log(error_rate) / ...
        // It is equivalent to (-_n) * std::log(error_rate) / ...
        // Now we're dead (throw bad_alloc)! (-_n) is HUGE because _n is of type size_t,
        // which can take up 8 bytes.
        _bitmap.resize(std::round( - (_n * std::log(error_rate)) / std::pow(log(2), 2) ));
        _k = std::round( std::log(2) * _bitmap.capacity() / _n );
        _hash_keys.resize(_k);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32> distrib(0, 0xFFFFFFFF);
        for (int i = 0; i < _k; ++i)
            _hash_keys[i] = distrib(gen);
    }

    size_t size() const { return _count; }

    size_t capacity() const { return _n; }

    // m
    size_t bitarray_size() const { return _bitmap.capacity(); }

    size_t bytes_used() const { return _bitmap.bytes_used(); }

    int num_of_hash_func() const { return _k; }

    // -1 if the filter is full
    //  0 if x already exists
    //  1 if x is newly inserted
    int insert(const T& x) {
        if (_count == _n) // full
            return -1;
        const size_t m = _bitmap.capacity();
        uint32 hash_code = std::hash<T>{}(x);
        bool inserted, contains = true;
        for (int i = 0; i < _k; ++i) {
            inserted = _bitmap.insert( hash_mix(_hash_keys[i], hash_code) % m );
            if (inserted) contains = false;
        }
        if (contains) return 0;
        ++_count;
        return 1;
    }

    // true if the filter *possibly* contains x,
    // false if the filter *definitely* does not contain x.
    bool contains(const T& x) const {
        const size_t m = _bitmap.capacity();
        uint32 hash_code = std::hash<T>{}(x);
        for (int i = 0; i < _k; ++i) {
            if (!_bitmap.contains( hash_mix(_hash_keys[i], hash_code) % m ))
                return false;
        }
        return true;
    }

    // union
    // false when filter bit array size incompatible
    // true otherwise
    bool merge(const BloomFilter& other) {
        return _bitmap.merge(other._bitmap);
    }

    // false when filter bit array size incompatible
    // true otherwise
    bool intersection(const BloomFilter& other) {
        return _bitmap.intersection(other._bitmap);
    }
};

#endif // BLOOMFILTER_H
