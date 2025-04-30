# Source and object files
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)/obj
CFILES = $(wildcard $(SRC_DIR)/*.cpp)
OFILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CFILES))

# Compiler and flags
CXX = g++
CARGS = -Wall

# Target executable
TARGET = tetris

# Default target
all: build

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile .cpp files to .o files in obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CARGS) -c $< -o $@

# Build release version
build: $(OFILES)
	$(CXX) $(CARGS) $(OFILES) -o $(TARGET)

# Build debug version
debug: CARGS += -g
debug: build

# Clean up
clean:
	rm -f $(TARGET) $(OBJ_DIR)/*.o

