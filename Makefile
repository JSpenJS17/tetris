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

SRC_DIR := src
OBJ_DIR := $(SRC_DIR)/obj
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
HDR := $(wildcard $(SRC_DIR)/*.hpp)

CC := g++
CFLAGS := -g -O2

ifeq ($(OS_TYPE), Windows)
	MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
	LIBS := -lcurl -I./include -L./lib 
	TARGET := tetris.exe
else
	MKDIR = mkdir -p $(1)
	LIBS := -lcurl
	TARGET := tetris
endif

# Final executable target
$(TARGET): $(HDR) $(OBJ)
	$(CC) $(OBJ) $(LIBS) $(DEFS) -o $(TARGET)

# Rule to compile .cpp -> .o (ensures obj/ exists)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

# Create the obj/ directory if it doesn't exist
$(OBJ_DIR):
	$(call MKDIR, $(OBJ_DIR))

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)