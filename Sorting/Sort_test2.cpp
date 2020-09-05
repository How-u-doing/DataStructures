#include "mySort.h"
#include <array>
#include <iostream>

using namespace std;
namespace sa = mySortingAlgo;

int main()
{    
    array<int, 20> s1 = { 18,7,12,8,9,5,13,1,19,16,4,11,0,17,15,3,6,2,14,10 };
    int n1 = 10;
    sa::quickselect(s1.begin(), s1.begin() + n1, s1.end()/*, greater<int>{}*/);
    for (int a : s1) {
        cout << a << " ";
    }

    cout << "\n\n";
    array<int, 20> s2 = { 19,12,18,15,16,3,11,6,9,8,13,7,5,0,17,14,4,10,2,1 };
    int n2 = 4;
    sa::partial_quicksort(s2.begin(), s2.begin() + n2, s2.end()/*, greater<int>{}*/);
    for (int a : s2) {
        cout << a << " ";
    }

    cout << "\n\n";
    array<int, 20> s3 = {7,5,18,0,15,4,16,3,11,6,9,19,12,8,13,17,10,2,14,1 };
    int n3 = 5;
    sa::partial_sort(s3.begin(), s3.begin() + n3, s3.end(), greater<int>{});
    for (int a : s3) {
        cout << a << " ";
    }
    cout << '\n';
    return 0;
}