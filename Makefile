#
# This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
#

CXXFLAGS = -Wall -Wextra
LDLIBS = $(shell sdl2-config --libs) $(shell sdl2-config --libs)_mixer

SRC_DIR = src
CLIENT_SRC := $(wildcard $(SRC_DIR)/client.cpp)
CLIENT2_SRC := $(wildcard $(SRC_DIR)/client2.cpp)
SERVER_SRC := $(wildcard $(SRC_DIR)/server*.cpp)

# Heavily inspired by https://stackoverflow.com/a/30602701
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
CLIENT_OBJ := $(CLIENT_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CLIENT2_OBJ := $(CLIENT2_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
SERVER_OBJ := $(SERVER_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

PROGRAM = $(BIN_DIR)/client $(BIN_DIR)/server $(BIN_DIR)/client2

.DELETE_ON_ERROR:
.PHONY: all clean

all: $(PROGRAM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(^) -o $(@)

$(BIN_DIR)/server: $(SERVER_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(^) $(LDLIBS) -o $(@)

$(BIN_DIR)/client: $(CLIENT_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(^) -o $(@)

$(BIN_DIR)/client2: $(CLIENT2_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(^) -o $(@)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv $(BUILD_DIR)
