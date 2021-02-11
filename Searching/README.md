# Count Words
|   | std::unordered\_map | myst::[HashMap](https://github.com/How-u-doing/DataStructures/blob/master/Searching/HashMap/HashMap.h) | std::map | myst::[BstMap](https://github.com/How-u-doing/DataStructures/blob/master/Searching/TreeMap/BstMap.h) | myst::[AvlMap](https://github.com/How-u-doing/DataStructures/tree/master/Searching/TreeMap/AvlMap.h) | myst::[TST](https://github.com/How-u-doing/DataStructures/blob/master/Searching/TreeMap/TST.h) |
|---| --- | --- | --- | --- | --- | --- |
| build time | 5.088s | 4.978s | 6.902s | 6.937s | 6.827s | 5.671s |
| sort time  | 0.045s | 0.070s | 0.073s | 0.070s | 0.076s | 0.106s |
| total time | 5.133s | 5.048s | 6.975s | 7.007s | 6.903s | 5.777s |
| tree height|        |        |        |   47   |   21   |        |

and the size of the lookup table (words length >= 8) is `278063`.

# Unordered
![](img/count_words_unordered.png)

# Ordered
![](img/count_words_ordered.png)
