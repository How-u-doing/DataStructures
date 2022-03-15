#include <vector>
#include <cmath>

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
    std::vector<unsigned char> _bitmap;
    size_t _count = 0;

public:
    Bitmap(size_t capacity) : _bitmap(std::ceil(capacity / 8.0), '\0') {};

    void resize(size_t n) {
        _bitmap.resize(std::ceil(n / 8.0), '\0');
    }

    size_t capacity() const {
        return _bitmap.size() * 8;
    }

    size_t size() const {
        return _count;
    }

    void insert(unsigned n) {
        const size_t index = n / 8;
        const unsigned char r = n % 8; // set the rth bit one
        _bitmap[index] = _bitmap[index] | static_cast<unsigned char>(1 << r);
        ++_count;
    }

    bool present(unsigned n) const {
        const size_t index = n / 8;
        const unsigned char r = n % 8;
        return (_bitmap[index] >> r) & 1;
    }

    void erase(unsigned n) {
        const size_t index = n / 8;
        const unsigned char r = n % 8; // set the rth bit zero
        _bitmap[index] = _bitmap[index] & ~static_cast<unsigned char>(1 << r);
        --_count;
    }
};
