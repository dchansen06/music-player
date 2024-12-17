CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = -lSDL2 -lSDL2_mixer
OBJ = main.o functions.o

.DELETE_ON_ERROR:
.PHONY: all clean

all: prog

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $(@) $(^) $(LIBS)

prog: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)

clean:
	rm prog *.o
