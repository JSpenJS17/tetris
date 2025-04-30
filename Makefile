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
SB_DIR := $(SRC_DIR)/scoreboard
OBJ_DIR := $(SRC_DIR)/obj
SB_SRC := $(wildcard $(SB_DIR)/*.cpp)
SB_HDR := $(wildcard $(SB_DIR)/*.hpp)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
HDR := $(wildcard $(SRC_DIR)/*.hpp)

ALL_SRC := $(SRC) $(SB_SRC)
ALL_HDR := $(HDR) $(SB_HDR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)) \
       $(patsubst $(SB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SB_SRC))
OBJ_NOSB := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%_nosb.o, $(SRC))

CC := g++
CFLAGS := -g -O2
LIBS := -lcurl
DEFS := -DSCOREBOARD
TARGET := tetris
TARGET_NOSB := tetris_noscoreboard

ifeq ($(OS_TYPE), Windows)
	MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
	LIBS += -I./include -L./lib
else
	MKDIR = mkdir -p $(1)
endif

# Default target includes scoreboard
all: $(TARGET)

# No-scoreboard target
noscoreboard: DEFS := 
noscoreboard: LIBS :=
noscoreboard: $(TARGET_NOSB)

# Final executable target
$(TARGET): $(ALL_HDR) $(OBJ)
	$(CC) $(OBJ) $(LIBS) $(DEFS) -o $(TARGET)

$(TARGET_NOSB): $(HDR) $(OBJ_NOSB)
	$(CC) $(OBJ_NOSB) $(LIBS) $(DEFS) -o $(TARGET_NOSB)

# Rule to compile .cpp -> .o (no scoreboard)
$(OBJ_DIR)/%_nosb.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

# Rule to compile .cpp -> .o (scoreboard)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SB_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

# Create the obj/ directory if it doesn't exist
$(OBJ_DIR):
	$(call MKDIR, $(OBJ_DIR))

.PHONY: clean all noscoreboard
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)*
