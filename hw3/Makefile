CXX = g++
CXXFLAGS = -std=c++11 -I$(INCLUDE_DIR) -Ofast
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
SRC_FILES = main.cpp graph.cpp solution.cpp
SRC = $(SRC_FILES:%=$(SRC_DIR)/%)
OBJ = $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = hw3
EXECUTABLE2 = tester

$(EXECUTABLE): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

$(EXECUTABLE2): obj/tester.o obj/graph.o obj/solution.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

obj/tester.o: src/tester.cpp
	$(CXX) -c -o $@ $^ $(LDFLAGS) $(CXXFLAGS)
	
obj/multistart.o: src/multistart.cpp
	$(CXX) -c -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

all: $(EXECUTABLE)

clean:
	rm -rf $(OBJ) $(EXECUTABLE) $(EXECUTABLE2)

run:
	@./$(EXECUTABLE)
