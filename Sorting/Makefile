CC := g++
CFLAGS := -std=c++17 -Wall -g

TESTS := Sort_test Sort_test2

all: $(TESTS) 

$(TESTS): %: %.cpp mySort.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TESTS) 

.PHONY: all clean
