CXX := g++
CFLAGS := -std=c++17 -Wall -g

TEST := _Tree_test

all: $(TEST) 

$(TEST): %_test: %_test.cpp %.h
	$(CXX) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TEST) 

.PHONY: all clean
