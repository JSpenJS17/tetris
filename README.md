## Description
Tetris for Linux and Windows terminal. 

Tested in Windows 11, Ubuntu 22.04, and Oracle Linux Server 9.5.

Adheres to the 2009 Tetris Handbook.

Uses a homebrew terminal graphics engine.

Purpose was to familiarize myself with C++.

Can run in headless environments!

Now has a global leaderboard!

## Binaries
Pre-built binaries can be found in the releases section or in the `/bin` folder.

## Building from Source

```
git clone https://github.com/JSpenJS17/tetris.git
cd tetris
make
```

`make` has a target named "offline" if you don't have <curl/curl.h>
i.e.
```
git clone https://github.com/JSpenJS17/tetris.git
cd tetris
make offline
```

You need the .dlls in the same directory as `tetris.exe` for it to run on Windows.

I trust that if you're on Linux you're smart enough to install the prerequisites for yourself.
You need `<curl/curl.h>`
