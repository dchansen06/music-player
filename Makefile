#
# This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
#

CXXFLAGS = -Wall -Wextra
LDLIBS = $(shell sdl2-config --libs --cflags) $(shell sdl2-config --libs --cflags)_mixer $(shell pkg-config gtkmm-4.0 --libs --cflags)

SRC_DIR = src
SERVER_SRC = $(wildcard $(SRC_DIR)/server*.cpp)
CLIENT_SRC = $(wildcard $(SRC_DIR)/client*.cpp)
GUI_SRC = $(wildcard $(SRC_DIR)/gui*.cpp) $(filter-out $(SRC_DIR)/client.cpp, $(CLIENT_SRC))

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
PROGRAM = $(BIN_DIR)/server $(BIN_DIR)/client $(BIN_DIR)/gui
OBJ_DIR = $(BUILD_DIR)/obj
SERVER_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SERVER_SRC))
CLIENT_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CLIENT_SRC))
GUI_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(GUI_SRC))

.DELETE_ON_ERROR:
.PHONY: all clean

all: $(PROGRAM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | Makefile $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(^) $(LDLIBS) -o $(@)

$(BIN_DIR)/server: $(SERVER_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(^) $(LDLIBS) -o $(@)

$(BIN_DIR)/client: $(CLIENT_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(^) -o $(@)

$(BIN_DIR)/gui: $(GUI_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(^) $(LDLIBS) -o $(@)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv $(BUILD_DIR)
