CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean all

TEST := ed_test

all: $(TEST)

ed.o: ed.cc ed.h
	$(CXX) $(CXXFLAGS) -O3 -c $<

ed_test.o: ed_test.cc ed.h
	$(CXX) $(CXXFLAGS) -g -c $<

ed_test: ed_test.o ed.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TEST) *.o
