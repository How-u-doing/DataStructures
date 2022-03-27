#ifndef BITMAP_H
#define BITMAP_H

#include <vector>
#include <cmath>
#include <cassert>

/*
 * Mapping an integer to a bit.
 * The idea is simple: instead of using a `char` (8 bits) to tell a number's
 * presence, we can use just a bit. Note that the smallest unit of memory we can
 * manipulate in RAM directly is byte. So we'll use some tricks to add a number,
 * to tell if a number exists, and to delete a number.
 *
 * This can be helpful if we want to tell if a number exists in a HUGE set of
 * numbers. For example, say we have a set of integers of size 2^32. If we use
 * a char/bool array to represent their presence, it will cost 4 GiB of RAM.
 * However, we can get it done just with 4 GiB / 8 = 512 MiB by using a bitmap.
 */
class Bitmap {
    std::vector<unsigned char> _bitarr;
    size_t _count = 0;

public:
    // construct an empty bitmap, please resize() later if you want to use it
    Bitmap () {}

    explicit Bitmap(size_t capacity) : _bitarr(std::ceil(capacity / 8.0), '\0') {};

    void resize(size_t n) {
        _bitarr.resize(std::ceil(n / 8.0), '\0');
    }

    size_t capacity() const { return _bitarr.size() * 8; }

    size_t bytes_used() const { return _bitarr.size(); }

    size_t size() const { return _count; }

    bool insert(unsigned n) {
        assert(n < capacity());
        const size_t index = n / 8;
        const unsigned char r = n % 8;
        if ((_bitarr[index] >> r) & 1) return false; // set the rth bit one
        _bitarr[index] |= static_cast<unsigned char>(1 << r);
        ++_count;
        return true;
    }

    bool contains(unsigned n) const {
        assert(n < capacity());
        const size_t index = n / 8;
        const unsigned char r = n % 8;
        return (_bitarr[index] >> r) & 1;
    }

    bool erase(unsigned n) {
        assert(n < capacity());
        const size_t index = n / 8;
        const unsigned char r = n % 8;
        if (!((_bitarr[index] >> r) & 1)) return false; // set the rth bit zero
        _bitarr[index] &= ~static_cast<unsigned char>(1 << r);
        --_count;
        return true;
    }

    // union
    bool merge(const Bitmap& other) {
        const size_t cap = capacity();
        if (other.capacity() != cap)
            return false;
        for (size_t i = 0; i < cap; ++i) {
            _bitarr[i] |= other._bitarr[i];
        }
        return true;
    }

    bool intersection(const Bitmap& other) {
        const size_t cap = capacity();
        if (other.capacity() != cap)
            return false;
        for (size_t i = 0; i < cap; ++i) {
            _bitarr[i] &= other._bitarr[i];
        }
        return true;
    }
};

#endif // BITMAP_H
