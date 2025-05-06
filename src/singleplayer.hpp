#ifndef SINGLEPLAYER_HPP
#define SINGLEPLAYER_HPP
#include "game.hpp"

void print_scoreboard();

uint calculate_score(const uint lines_cleared,
                     const Tetromino& piece, 
                     const Stacked_Blocks& stack, 
                     const uint level, string* prev_clear,
                     const int t_spin_type);


void clear_score_output();

void game_over_screen();

void draw_games();

void pause_game();

void singleplayer();

#endif