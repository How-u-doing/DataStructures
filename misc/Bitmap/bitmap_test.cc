#include "bitmap.h"
#include <iostream>

using namespace std;

void test1()
{
    Bitmap bitmap(10);
    for (int i = 0; i < 10; ++i)
        bitmap.insert(i);

    cout << bitmap.size() << '\t'
         << bitmap.capacity() << '\t'
         << bitmap.present(9) << '\n';

    bitmap.erase(9);

    cout << bitmap.size() << '\t'
         << bitmap.capacity() << '\t'
         << bitmap.present(9) << '\n';
}

void test2()
{
    // 2^32 integers, but only using 2^32 B / 8 = 512 MiB of RAM.
    // If we just to store these 2^32 integers, it will consume
    // 4 * 2^32 = 16 GiB (HUGE!!) of memory. Even if we use a char
    // array, it will cost 2^32 B = 4 GiB of memory.
    // One might think of virtual memory, which, theoretically, can
    // be "infinite" (magnitude of TiB at least). But don't forget
    // virtual memory and paging are painfully slow.
    unsigned capacity = static_cast<unsigned>(-1);
    Bitmap bitmap(capacity);
    for (size_t i = 0; i < capacity; ++i)
        bitmap.insert(i);

    bitmap.erase(2017);
    bitmap.erase(2021);
    bitmap.erase(2022);
    bitmap.erase(2024);
    for (int year = 2017; year < 2027; ++year)
        cout << year << ':' << bitmap.present(year) << '\n';
}

int main()
{
    test1();
    test2();
    return 0;
}
