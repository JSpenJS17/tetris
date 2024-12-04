#ifndef ENGINE_HPP
#define ENGINE_HPP

using namespace std;

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <cmath>


#ifdef WINDOWS
// Windows specific includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

// Windows color codes
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define SKY_BLUE        3
#define RED             4
#define PURPLE          5
#define YELLOW          6
#define LIGHT_GRAY      7
#define GRAY            8
#define LIGHT_BLUE      9
#define LIGHT_GREEN     10
#define CYAN            11
#define LIGHT_RED       12
#define MAGENTA         13
#define LIGHT_YELLOW    14
#define WHITE           15

// Windows key codes
const int ENTER = 13, P = 112, SPACE = 32,
                   UP = 72, LEFT = 75, DOWN = 80, RIGHT = 77;

#endif

#ifdef LINUX
// Linux specific includes
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
// Linux color codes
#define BLACK           30
#define RED             31
#define GREEN           32
#define YELLOW          33
#define BLUE            34
#define PURPLE          35
#define CYAN            36
#define LIGHT_GRAY      37
// All colors past here are non-standard -- might not work on all machines!
#define GRAY            90 
#define LIGHT_RED       91
#define LIGHT_GREEN     92
#define LIGHT_YELLOW    93
#define LIGHT_BLUE      94
#define MAGENTA         95
#define SKY_BLUE        96
#define WHITE           97

// Linux key codes
const uint ENTER = 10, P = 112, SPACE = 32,
                   UP = 65, LEFT = 68, DOWN = 66, RIGHT = 67;

// Terminal controls specific to linux - init controller and reset controller
void init_termios();
void reset_termios();
#endif

float dist(const int x1, const int y1, const int x2, const int y2);

void set_cursor_pos(const unsigned int row, const unsigned int col);

void delay(int ms);

int rand_int(int limit);

void clear_screen();

void show_cursor(bool show);

void color(unsigned const short bgc,
           unsigned const short font_color);

char wait_for_kb_input();

char get_kb_input();

void set_cursor_pos(unsigned const int row, unsigned const int col);

struct Pixel{
    unsigned short bgc;
    unsigned short fgc;
    char val;

    Pixel(char value, unsigned short bg_color, unsigned short fg_color);

    Pixel();

    friend bool operator==(Pixel& me, Pixel& other);

    friend bool operator!=(Pixel&me, Pixel& other);
};

class Board{
    public:
        //board constructor
        //defines the board var as a double vector of fillers in the dimensions
        //given by the user
        Board(const unsigned int length, const unsigned int height,
                const Pixel filler_pix);

        void write(const unsigned int row, const unsigned int col,
                const Pixel pixel);

        void write(const unsigned int row, const unsigned int col);

        void draw(unsigned const int height_offset, bool last_col_no_space);
 
        void clear_board(const bool redraw_whole_board);

        Pixel get_pix_at(unsigned const int row, unsigned const int col);

    private:
        Pixel filler;
        unsigned int len;
        unsigned int hei;
        vector<vector<Pixel>> freshboard;
        vector<vector<Pixel>> oldboard;
        vector<vector<Pixel>> board;
        
        void print_in_bounds(Pixel pix, unsigned const int col);

        void print_pixel(Pixel pix);
};

void draw_pixel(Pixel pix);

#endif

