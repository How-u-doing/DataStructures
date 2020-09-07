CC := g++
CFLAGS := -std=c++17 -Wall -g

TEST := Tree_test

all: $(TEST) 

$(TEST): %: %.cpp Tree.h Stack.h SeqStack.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TEST) 

.PHONY: all clean
