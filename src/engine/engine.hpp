#ifndef ENGINE_HPP
#define ENGINE_HPP

using namespace std;

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <cmath>

#ifdef _WIN32
// Windows specific includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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
const int ENTER = 13,
          BACKSPACE = 8;

#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
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
const int ENTER = 10,
          BACKSPACE = 127;
#endif

typedef struct POS {
        int row;
        int col;
} POS;

// Linux and windows shared code
const int SPACE = 32, ESC = 27,
        UP = -10, DOWN = -20, LEFT = -30, RIGHT = -40;

void init_engine();
void close_engine();

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

struct PosPixel {
        PosPixel(unsigned int row, unsigned int col, Pixel face) : 
                row(row), col(col), face(face) {}
        unsigned int row, col;
        Pixel face;
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

        void draw(unsigned const int height_offset = 0, bool last_col_no_space = false, unsigned const int width_offset = 0);
 
        void draw_from_changes(vector<PosPixel>* changes, unsigned const int height_offset = 0, bool last_col_no_space = false, 
                                unsigned const int width_offset = 0);

        void update_changes();

        void clear_board(const bool redraw_whole_board);

        Pixel get_pix_at(unsigned const int row, unsigned const int col);

        vector<PosPixel>* get_changes() { return &changes; } // changes updated in draw()

    private:
        Pixel filler;
        unsigned int len;
        unsigned int hei;
        vector<vector<Pixel> > freshboard;
        vector<vector<Pixel> > oldboard;
        vector<vector<Pixel> > board;
        vector<PosPixel> changes;
        
        void print_in_bounds(Pixel pix, unsigned const int col);

        void print_pixel(Pixel pix);
};

void draw_pixel(Pixel pix);

string keycode_to_string(char keycode);
bool is_alpha(char keycode);

#endif