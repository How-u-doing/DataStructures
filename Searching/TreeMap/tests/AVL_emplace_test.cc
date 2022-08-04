#include <cstring>
#include <iostream>
#include <vector>
#include "../AvlMap.h"

class A {
public:
    A(const char* s) {
        sz = strlen(s);
        x = static_cast<char*>(operator new(sz+1));
        strcpy(x, s);
        std::cout << "A (const char*) at " << this << '\n';
    }

    A() : x(nullptr), sz(0) {
        std::cout << "A () at " << this << '\n';
    }

    A(const A &other) : sz(other.sz) {
        x = static_cast<char*>(operator new(sz+1));
        strcpy(x, other.x);
        std::cout << "A (A &) at " << this << '\n';
    }

    A(A &&other) noexcept : x(other.x), sz(other.sz), p(other.p) {
        other.x = nullptr;
        other.sz = 0;
        std::cout << "A (A &&) at " << this << '\n';
    }

    A& operator=(const A& rhs) {
        if (x)
            operator delete(x);
        sz = rhs.sz;
        x = static_cast<char*>(operator new(sz+1));
        strcpy(x, rhs.x);
        std::cout << "operator=(A &)\n";
        return *this;
    }

    A& operator=(A&& rhs) {
        std::swap(x, rhs.x);
        std::swap(sz, rhs.sz);
        std::swap(p, rhs.p);
        std::cout << "operator=(A &&)\n";
        return *this;
    }

    ~A() {
        operator delete(x);
        std::cout << "~A ()\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const A& a) {
        os << a.x;
        return os;
    }

private:
    char* x;
    size_t sz;
public:
    void* p = this;
};

#if 0
#  define __ns__  std
#  define __map__ map
#else
#  define __ns__  mySymbolTable
#  define __map__ AvlMap
#endif

__ns__::__map__<int, A> NRVO()
{
    __ns__::__map__<int, A> m;
    for (int i = 0; i < 3; ++i)
        m.emplace(i, "map");
    return m;
}

A foo()
{
    A a("howdy");
    return a;
}

A bar(bool x = false)
{
#if 0
    A a("x");
    A b("y");
    return x ? a : b;
#elif 1
    return x ? "x" : "y";
#else
    return x ? A("x") : A("y");
#endif
}

int main ()
{
    // named return value optimization
    __ns__::__map__<int, A> m = NRVO();
    for (auto &x : m)
        std::cout << x.first << " " << x.second << '\n';
}
