#ifndef GAME_HPP
#define GAME_HPP

#include <limits>
#include <string.h>
#include <csignal>
#include "engine.hpp"
#include "tetromino.hpp"
#include "blocks.hpp"

#ifdef SCOREBOARD
#include "scoreboard/scoreboard.hpp"
#include "scoreboard/whoami.hpp"
#endif

#ifndef uint
typedef unsigned int uint;
#endif
#ifndef ulong
typedef unsigned long ulong;
#endif

typedef enum GAMETYPE {
    SINGLEPLAYER,
    MULTIPLAYER,
    NUMMODES
} GAMETYPE;

typedef struct GAMEDATA {
    // todo
} GAMEDATA;

extern const uint width;
extern const uint height;
extern const uint lock_delay_reset;
extern Pixel bg;
extern Board game;
extern ulong score;
extern GAMETYPE game_type;
extern uint line_total;
extern int level_selected;
extern int level;

#endif