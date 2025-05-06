# Tetris Makefile

ifeq ($(OS), Windows_NT)
	OS_TYPE := Windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		OS_TYPE := Linux
	else ifeq ($(UNAME_S), Darwin)
		OS_TYPE := MacOS
	else
		OS_TYPE := Unknown
	endif
endif

$(info Detected OS: $(OS_TYPE))

# Compiler and flags
CC := g++
CFLAGS := -g -O2
LIBS := -lcurl
DEFS := -DSCOREBOARD

# Directories
SRC_DIR := src
SB_DIR := $(SRC_DIR)/scoreboard
ENG_DIR := $(SRC_DIR)/engine
OBJ_DIR := obj

# Platform-specific settings
ifeq ($(OS_TYPE), Windows)
	MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
	LIBS += -I./include -L./lib
	FORCE := -Force
	TARGET := bin/windows/tetris
else
	MKDIR = mkdir -p $(1)
	FORCE := -f
	TARGET := tetris
endif

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SB_FILES  := $(wildcard $(SB_DIR)/*.cpp)
ENG_FILES := $(wildcard $(ENG_DIR)/*.cpp)

# Object file mappings (flattened into obj/)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
       $(patsubst $(SB_DIR)/%.cpp,$(OBJ_DIR)/scoreboard_%.o,$(SB_FILES)) \
       $(patsubst $(ENG_DIR)/%.cpp,$(OBJ_DIR)/engine_%.o,$(ENG_FILES))

# Default target
all: $(TARGET)

# Link all objects
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LIBS) $(DEFS) -o $(TARGET)

# Compile rules for each category
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

$(OBJ_DIR)/scoreboard_%.o: $(SB_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

$(OBJ_DIR)/engine_%.o: $(ENG_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

# Create the obj directory if needed
$(OBJ_DIR):
	$(call MKDIR, $(OBJ_DIR))

.PHONY: clean all
clean:
	rm $(FORCE) $(OBJ_DIR)/*.o $(TARGET)
