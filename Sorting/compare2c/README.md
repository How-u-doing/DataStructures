# C qsort() vs C++ sort() on large objects
Say let's try to sort 10M objects:
```c++
enum Gender : char  {M, W};

struct Employee {
	int id;
	Gender gender;
	char addr[10];
	double salary;
	double s;
	int t;	
};
	
const int N = (int) 1e7;
```
In `sort.cc`, the size of a single object is 40 bytes, so each array will require about 400 MiB of memory. In `sort+.cc`, we increase the `addr` size to 370, in turn, the size of each object will be 9 times larger (i.e. 400 bytes). Meanwhile we decrease the number of objects to 1M, otherwise it would be about 4GiB:scream:. `sort0.cc` and `sort0+.cc` ditto. 

Here are some results I've tested on my CentOS server and Kali virtual machine:

![](img/CentOS.png "CentOS")

![](img/KaliVM.png "Kali VM")

See also some tests on *coliru* online compiler: [sort0](https://coliru.stacked-crooked.com/a/af0ba8941deb64f4),
 [sort0+](https://coliru.stacked-crooked.com/a/62800e4bbcbbbe8d). You can change the size of `addr` and `N` to see
how result varies. 

## Discussion
From above results, we can see that as the struct size increses, the performance of sort() routine seems
to decline, whereas the qsort() seems to remain unchanged (by comparison of first two items of `./sort0` and
`./sort0+`, `N` changes to its `1/10`, qsort() time cost also changes to about its `1/10`, though with struct
 size getting 10 times as large (and different data); sort(), however, doesn't scale in this way.)

Then, what possibly caused it? Well, first I thought it was the compare parameter type: qsort() takes `const void*`,
 whereas sort() takes `const T&`. The pointer type surely has less overhead when arguments passed. In this way,
each time a compare function is called, sort() has to ~~construct~~ 2 objects whereas 0 from qsort(). (I made a 
mistake:sweat\_smile:, that the two arguments should be elements that are parts of the array. So don't need to *construct*)
 That sounds reasonable, but qsort() also need to `swap` objects if need to, which usually just does *3 times copy*. While C++
introduced `std::move` (since c++11) to elude copy. It might have some overhead, but should be trivial. Additionally, I made
a pair of contrast between pass by `pointer` and by `reference` using quicksort algorithm (see `../mySort.h` and `mySort2.h`).
It turned out that the deference is insignificant (see last two items of `./sort[+]` from above screenshots).

So, if no constructing objects in sort() plus qsort() uses *3 times copy* when swapping, then what makes qsort() faster than 
sort() when sorting large objects?
