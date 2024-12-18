#
# This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
#

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
