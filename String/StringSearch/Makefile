CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean all

TEST := StringSearch_test

all: $(TEST) gwnre generate_random_genome

StringSearch.o: StringSearch.cpp StringSearch.h
	$(CXX) $(CXXFLAGS) -O3 -c $<

StringSearch_test.o: StringSearch_test.cpp StringSearch.h
	$(CXX) $(CXXFLAGS) -g -c $<

StringSearch_test: StringSearch_test.o StringSearch.o
	$(CXX) $(CXXFLAGS) -o $@ $^

gwnre.o: gwnre.cc StringSearch.h
	$(CXX) $(CXXFLAGS) -O3 -c $<

gwnre: gwnre.o StringSearch.o
	$(CXX) $(CXXFLAGS) -o $@ $^

generate_random_genome: generate_random_genome.cc
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

clean:
	rm -f $(TEST) gwnre generate_random_genome *.o
