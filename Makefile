CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

LIB_SRC = src/huffman.cpp
HEADERS = src/huffman.hpp

.PHONY: build run bench clean

build: main

main: src/main.cpp $(LIB_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) src/main.cpp $(LIB_SRC) -o main

benchmark: bench/benchmark.cpp $(LIB_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) bench/benchmark.cpp $(LIB_SRC) -o benchmark

run: main
	./main -c test.txt coded.out

bench: benchmark
	./benchmark

clean:
	rm -f main benchmark coded.out
