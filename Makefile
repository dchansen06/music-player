CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = -lSDL2 -lSDL2_mixer
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
