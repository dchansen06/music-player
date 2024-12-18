CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = $(shell sdl2-config --libs) $(shell sdl2-config --libs)_mixer
OBJ = main.o functions.o

.DELETE_ON_ERROR:
.PHONY: all clean

all: player

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $(@) $(^) $(LIBS)

player: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)

clean:
	rm player *.o
