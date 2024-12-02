
CFILES = src/tetris.cpp src/engine.cpp

all: build

build:
	g++ $(CFILES) -o tetris.exe

clean:
	rm tetris.exe
