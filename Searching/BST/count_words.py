import collections
file = open('leipzig1M.txt', encoding="utf8")
a = file.read()
wordcount = {} # using dictionary
for word in a.lower().split():
    if len(word)<8:
        continue
    if word not in wordcount:
        wordcount[word] = 1
    else:
        wordcount[word] += 1
        
n_print = 10
print("\nThe {} most common words are as follows\n".format(n_print))
word_counter = collections.Counter(wordcount)
for word, count in word_counter.most_common(n_print):
    print(word, ": ", count)
file.close()
