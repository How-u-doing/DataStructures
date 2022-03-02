#include "mySort.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
namespace mysa = mySortingAlgo;

void counting_bucket_sort_tests()
{
    cout << "counting/bucket sort tests:\n";

    string s = { "ABDWADFERCASYIJHGFDAB" };
    vector<unsigned char> vuc(s.begin(), s.end());

    mysa::counting_sort(s.begin(), s.end());
    cout << s << '\n';

    mysa::counting_sort_unstable(vuc.begin(), vuc.end());
    for (auto x : vuc) {
        cout << x;
    }
    cout << '\n';

    vector<int> vi = { -1, 3, -65, 65, 66, 63, 65, 66, 2, 3, -1 };
    mysa::counting_sort_ext/*_unstable*/(vi.begin(), vi.end());
    //mysa::bucket_sort(vi.begin(), vi.end(), 20);
    for (auto x : vi) {
        cout << x << "  ";
    }
    cout << '\n';

    vector<float> vf = { -1, 3.7f, -65, 65, 66, 63, 65.2f, 66, 2, 3.7f, -1.5f, 3.7f, 4.2f };
    mysa::bucket_sort(vf.begin(), vf.end(), 10);
    for (auto x : vf) {
        cout << x << "  ";
    }
    cout << '\n';
}

void LSD_radix_sort_tests()
{
#ifdef __linux__
    // Of course, std::string will also perfectly work on Windows.
    // Here we test wstring on windows.
    // Note that on Linux sizeof( wchar_t ) == 4, so radix sort an
    // array of wstrings will cause integer overflow. But Linux
    // supports wonderfully for utf8, so no need for wstring ;)
    // see more at
    // https://stackoverflow.com/questions/402283/stdwstring-vs-stdstring/402918#402918

    //string cards[] = {
    vector<string> cards = {
                            "7Ψ∮ℵΔΩ∑",
                            "2IYE230",
                            "3CIO720",
                            "1ICK750",
                            "9OHV845",
                            "4JZY524",
                            "1ICK750",
                            "3CIO720",
                            "8OHV845",
                            "1OHV845",
                            "2RLA629",
                            "2RLA629",
                            "3ATW723" };
    auto& out = cout;

#else    // _WIN32 || _WIN64
    //wstring cards[] = {
    vector<wstring> cards = {
                            L"7Ψ∮ℵΔΩ∑",
                            L"2IYE230",
                            L"3CIO720",
                            L"1ICK750",
                            L"9OHV845",
                            L"4JZY524",
                            L"1ICK750",
                            L"3CIO720",
                            L"8OHV845",
                            L"1OHV845",
                            L"2RLA629",
                            L"2RLA629",
                            L"3ATW723" };
    auto& out = wcout;
    setlocale(LC_ALL, "en_US.UTF-8");

#endif

    //mysa::radix_sort(cards, cards + 13, 7);
    mysa::radix_sort(cards.begin(), cards.end(), 7);
    cout << "LSD radix sorting an array of c++ strings: \n";
    for (const auto& card : cards)
        out << card << '\n';

    const char* cards2[] = {
    //vector<const char*> cards2 = {
                            "9Ψ∮ℵΔΩ∑",
                            "2IYE237",
                            "3CIO720",
                            "1ICK750",
                            "1OHV845",
                            "4JZY524",
                            "1ICK750",
                            "3CIO720",
                            "1OHV845",
                            "1OHV845",
                            "2RLA629",
                            "2RLA629",
                            "3ATW723" };
    mysa::radix_sort(cards2, cards2 + sizeof cards2 / sizeof(const char*), 7);
    //mysa::radix_sort(cards2.begin(), cards2.end(), 7);
    cout << "\nLSD radix sorting an array of c strings (const char*): \n";
    for (const auto& card : cards2)
        cout << card << '\n';
}

void MSD_radix_sort_tests()
{
    cout << "\nMSD radix sort tests:\n";
    char p[] = {'b', 'c', '\x9', '\x80', '\0'};
    //string arr[] = {
    const char* arr[] = {
        p, "a\x1rng", "ab", "a\x1rn", "a\xffv", "a\xffw", "aa", "a",
        "a\xffu", "a\x80z", "a\x80yz", "a", "anyone", "an", "aka",
        "alien", "a", "abs", "abc", "a\x81pi=3.14", "akin", "alike",
        "\0ab", "a\x80xyz", "aha", "amazing", "also", "ai", "alphaa",
        "appreciated", "abcd", "an", "apple", "amazon", "google",
        "she", "sells", "seashells", "by", "the", "sea", "shore",
        "the", "shells", "she", "sells", "are", "surely", "seashells"};
    //mysa::radix_sort(arr, arr + sizeof arr / sizeof(string));
    mysa::radix_sort(arr, arr + sizeof arr / sizeof(const char*));

    cout << "================================\n";
    for (const auto& x : arr)
        cout << x << '\n';
    cout << "================================\n";

    vector<string> arr2;
    cout << "Input your strings to be sorted:\n";
    string s;
    while ( cin >> s ) {
        arr2.emplace_back(s);
    }
    mysa::radix_sort(arr2.begin(), arr2.end());

    cout << "\nAfter sorting:\n";
    cout << "================================\n";
    for (const auto& x : arr2)
        cout << x << '\n';
    cout << "================================\n";
}

void str_qsort_tests()
{
    cout << "\nThree-way string quicksort tests:\n";
    char p[] = {'b', 'c', '\x9', '\x80', '\0'};
    //string arr[] = {
    const char* arr[] = {
        p, "a\x1rng", "ab", "a\x1rn", "a\xffv", "a\xffw", "aa", "a",
        "a\xffu", "a\x80z", "a\x80yz", "a", "a\xffp", "a\xffk",
        "alien", "a", "abs", "abc", "a\x81pi=3.14", "akin", "alike",
        "\0ab", "a\x80xyz", "aha", "amazing", "also", "ai", "alphaa",
        "appreciated", "abcd", "an", "apple", "amazon", "google",
        "she", "sells", "seashells", "by", "the", "sea", "shore",
        "the", "shells", "she", "sells", "are", "surely", "seashells"};
    //mysa::str_qsort(arr, arr + sizeof arr / sizeof(string));
    mysa::str_qsort(arr, arr + sizeof arr / sizeof(const char*));
    //mysa::quicksort(arr, arr + sizeof arr / sizeof(const char*),
    //        [](const char* a, const char* b) {
    //            return std::strcmp(a, b) < 0;
    //        });

    cout << "================================\n";
    for (const auto& x : arr)
        cout << x << '\n';
    cout << "================================\n";

    vector<string> arr2;
    cout << "Input your strings to be sorted:\n";
    string s;
    while ( cin >> s ) {
        arr2.emplace_back(s);
    }
    mysa::str_qsort(arr2.begin(), arr2.end());

    cout << "\nAfter sorting:\n";
    cout << "================================\n";
    for (const auto& x : arr2)
        cout << x << '\n';
    cout << "================================\n";
}

int main()
{
    counting_bucket_sort_tests();
    LSD_radix_sort_tests();
    MSD_radix_sort_tests();
    str_qsort_tests();
    return 0;
}
