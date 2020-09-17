import time
import sys
import collections

start = time.time()

# python3 count_words.py leipzig1M.txt 8
file = open(sys.argv[1], encoding="utf8")
if len(sys.argv)<3:
    min_word_len = 0
else:
    min_word_len = int(sys.argv[2])

a = file.read()
wordcount = {} # using dictionary
for word in a.lower().split():
    if len(word)<min_word_len:
        continue
    if word not in wordcount:
        wordcount[word] = 1
    else:
        wordcount[word] += 1
    
word_counter = collections.Counter(wordcount)
for word, count in word_counter.most_common(10):
    print(word, ": ", count)
file.close()

duration = time.time() - start
print("\nUsed " + str(duration) + "s to find the 10 most common words")
