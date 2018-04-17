CXX = g++
CXXFLAGS = -std=c++11 -I$(INCLUDE_DIR)
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
SRC_FILES = main.cpp graph.cpp solution.cpp
SRC = $(SRC_FILES:%=$(SRC_DIR)/%)
OBJ = $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = hw1

$(EXECUTABLE): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

all: $(EXECUTABLE)

clean:
	rm -rf $(OBJ) $(EXECUTABLE)

run:
	@./$(EXECUTABLE)
