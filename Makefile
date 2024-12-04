CFILES = src/tetris.cpp src/engine.cpp
CARGS = -Wall

ifeq ($(OS),Windows_NT)
	CARGS += -D WINDOWS
else 
# Assume linux if not windows, no Mac version yet
	CARGS += -D LINUX
endif

all: build

debug:
	g++ $(CFILES) $(CARGS) -g -o tetris

build:
	g++ $(CFILES) $(CARGS) -o tetris

clean:
	rm tetris
