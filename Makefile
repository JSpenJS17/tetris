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

ifeq ($(OS_TYPE), Windows)
	MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
	CFLAGS += -static-libgcc -static-libstdc++
	LDFLAGS := -static-libgcc -static-libstdc++
	LIBS += -I./include -L./lib -lcurl -lws2_32 -lcrypt32
	TARGET := bin/windows/tetris
	TARGET_NOSB := bin/windows/tetris_offline
else
	MKDIR = mkdir -p $(1)
	TARGET := tetris
	TARGET_NOSB := tetris_offline
endif

# Default target includes scoreboard
all: $(TARGET)

# No-scoreboard target
offline: DEFS := 
offline: LIBS :=
offline: $(TARGET_NOSB)

# Final executable target
$(TARGET): $(ALL_HDR) $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) $(DEFS) -o $(TARGET)

$(TARGET_NOSB): $(HDR) $(OBJ_NOSB)
	$(CC) $(OBJ_NOSB) $(LDFLAGS) $(LIBS) $(DEFS) -o $(TARGET_NOSB)

# Rule to compile .cpp -> .o (no scoreboard)
$(OBJ_DIR)/%_nosb.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

# Rule to compile .cpp -> .o (scoreboard)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SB_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(DEFS) -c $< -o $@

.PHONY: clean all offline
clean:
	rm $(OBJ_DIR)/*.o $(TARGET)*
