#pragma once

#include "TST.h"

namespace mySymbolTable {

template<typename T>
class RWayTST {
    typedef unsigned char uchar;
    static const size_t R = 256; // extended ascii

    T *inlined_values[R] = {};  // the values of one-letter keys
    TST<T> tsts[R];
public:
    ~RWayTST() {
        for (auto x : inlined_values)
            delete x;
    }

    // returns nullptr if key does not exist
    T* get(const std::string &key) {
        if (key.empty()) 
            return nullptr;
        if (key.size() == 1)
            return inlined_values[(uchar) key[0]];
        TST<T> &tst = tsts[(uchar) key[0]];
        auto it = tst.find(key.substr(1));
        if (it != tst.end())
            return &it.val();
        return nullptr;
    }

    T& insert(const std::string &key, const T &val) {
        if (key.empty())
            throw std::invalid_argument("insert() key cannot be empty");
        if (key.size() == 1) {
            auto &p = inlined_values[(uchar)key[0]];
            if (p)
                delete p;
            p = new T(val);
            return *p;
        }

        TST<T> &tst = tsts[(uchar) key[0]];
        return *tst.insert(key.substr(1), val)->pval;
    }

    T& operator[](const std::string& key) {
        return insert(key, T());
    }

    int height() const {
        int h[R] = {0};
        for (size_t i = 0; i < R; i++) 
            h[i] = tsts[i].height();
        int max = *std::max_element(h, h+R);
        if (max == 0) {
            for (auto x : inlined_values)
                if (x)
                    return 1;
            return 0;
        }
        return 1 + max;
    }
};

} // namespace mySymbolTable
