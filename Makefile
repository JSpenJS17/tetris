CFILES = src/tetris.cpp src/engine.cpp
CARGS = -Wall

all: build

debug:
	g++ $(CFILES) $(CARGS) -g -o tetris

build:
	g++ $(CFILES) $(CARGS) -o tetris

clean:
	rm tetris
