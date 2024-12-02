
CFILES = src/tetris.cpp src/engine.cpp
CARGS = -Wall

all: build

build:
	g++ $(CFILES) $(CARGS) -o tetris.exe

clean:
	rm tetris.exe
