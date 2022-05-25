CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean all

TEST := matrix_chain_multiplication_test

all: $(TEST)

matrix_chain_multiplication.o: matrix_chain_multiplication.cc matrix_chain_multiplication.h
	$(CXX) $(CXXFLAGS) -g -c $<

matrix_chain_multiplication_test.o: matrix_chain_multiplication_test.cc matrix_chain_multiplication.h
	$(CXX) $(CXXFLAGS) -g -c $<

matrix_chain_multiplication_test: matrix_chain_multiplication_test.o matrix_chain_multiplication.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TEST) *.o
