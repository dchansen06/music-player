CXX=g++
CXXFLAGS=-Wall -Wextra

.PHONY: all clean

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -o $(@) $(^)

clean:
	rm main
