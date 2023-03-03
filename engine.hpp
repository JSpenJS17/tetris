#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>
#include <conio.h>
#include <stdio.h>

using namespace std;

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

