# Count Words
|   | std::unordered\_map | myst::[Hashtable](https://github.com/How-u-doing/DataStructures/blob/master/Searching/HashMap/Hashtable_impl.h) | std::map | myst::[BST](https://github.com/How-u-doing/DataStructures/blob/master/Searching/TreeMap/BST_impl.h) | myst::[AVL](https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/AVLtree_impl.h) | myst::[TST](https://github.com/How-u-doing/DataStructures/blob/master/Searching/TreeMap/TST.h) | myst::[SkipList](https://github.com/How-u-doing/DataStructures/blob/master/Searching/Randomized/SkipList_impl.h) |
|   :---           | :---:  | :---:  | :---:  | :---:  | :---:  | :---:  | :---:   |
| build time       | 5.126s | 5.181s | 7.047s | 7.037s | 6.877s | 5.745s | 10.545s |
| sort time        | 0.042s | 0.062s | 0.068s | 0.068s | 0.066s | 0.107s |  0.086s |
| total time       | 5.169s | 5.242s | 7.115s | 7.106s | 6.942s | 5.852s | 10.631s |
| tree/list height |        |        |        |   47   |   21   |   97   |   18    |

and the size of the lookup table (word length >= 8) is `278063`.

Note that `height(TST) >=  max_word_length`, which is `80` in this case.
We can reduce a TST's height by hybriding TST with R^2-way branching at the root. 

# Unordered
![](img/count_words_unordered.png)

# Ordered
![](img/count_words_ordered.png)
