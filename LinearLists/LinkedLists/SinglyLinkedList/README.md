```bash
$ make
g++ -o reverse_sublist reverse_sublist.cc -Wall -Wextra -Wpedantic -g
$ ./reverse_sublist 10 3 7
original list: 1 2 3 4 5 6 7 8 9 10 
reversed list: 1 2 7 6 5 4 3 8 9 10 
$ ./reverse_sublist 10 3 11
original list: 1 2 3 4 5 6 7 8 9 10 
reversed list: 1 2 10 9 8 7 6 5 4 3 
$ ./reverse_sublist 10 3 -1
original list: 1 2 3 4 5 6 7 8 9 10 
reversed list: 1 2 10 9 8 7 6 5 4 3
```
