CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = $(shell sdl2-config --libs) $(shell sdl2-config --libs)_mixer
SERVEROBJ = server.o server_functions.o
CLIENTOBJ = client.o

.DELETE_ON_ERROR:
.PHONY: all clean

all: server client

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $(@) $(^)

server: $(SERVEROBJ)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) $(LIBS)

client: $(CLIENTOBJ)
	$(CXX) $(CXXFLAGS) -o $(@) $(^)

clean:
	rm client server *.o
