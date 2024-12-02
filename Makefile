
CFILES = tetris.cpp engine.cpp

all: build

build:
	g++ tetris.cpp engine.cpp -o tetris.exe

