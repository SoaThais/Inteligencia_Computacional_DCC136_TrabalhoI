CXX=g++
INC=-I ./include
CFLAGS=-std=c++11 -W -Wall -pedantic $(INC)

trabOPHS: main.cpp ./src/*
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f trabOPHS

.PHONY: clean