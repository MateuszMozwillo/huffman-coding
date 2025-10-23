SRC = $(wildcard src/*.cpp)

run: build
	./main

build:
	g++ -Wall $(SRC) -o main
