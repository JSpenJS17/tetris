#ifndef GAME_HPP
#define GAME_HPP

#include <limits>
#include <string.h>
#include <csignal>
#include <thread>
#include "engine/engine.hpp"
#include "tetromino.hpp"
#include "blocks.hpp"
#include "engine/menu.hpp"
#include "engine/client_socket.hpp"
#include "client.hpp"

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

typedef struct GameData {
    GameData(){}
    GAMETYPE gametype;
    int starting_level;
    bool use_ghost = true;
} GameData;

extern const int width;
extern const int height;
extern const int lock_delay_reset;
extern Pixel bg;
extern Board game;
extern Board other_game;
extern long score;
extern GameData gamedata;
extern GAMETYPE game_type;
extern int line_total;
extern int level;

void reset_everything();

#endif