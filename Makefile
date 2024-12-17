CXX=g++
CXXFLAGS=-Wall -Wextra
SDLFLAGS=-lSDL2 -lSDL2_mixer

.PHONY: all clean

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(SDLFLAGS)

clean:
	rm main
