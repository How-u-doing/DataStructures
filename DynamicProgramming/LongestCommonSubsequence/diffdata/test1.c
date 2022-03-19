#include <stdio.h>
int main()
{
    int x = 1;
    int y = 2;
    int i = 0;
    while (x != y*y)
        ++i;
    printf("%d\n", i);
    return 0;
}
