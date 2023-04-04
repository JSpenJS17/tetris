#include "engine.hpp"
#include <iostream>
#include <cmath>

using namespace std;

float dist(const int x1, const int y1, const int x2, const int y2){
    return sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
}

void delay(int ms){
    //delay a millisecond amount
    //storing start time
    clock_t start_time = clock();
 
    //looping till desired time is achieved
    while (clock() < start_time + ms);
}

int rand_int(int limit) {
    //return a random integer between 0 and the limit.
    int divisor = RAND_MAX/(limit+1); 
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

void clear_screen(){
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
}

void show_cursor(bool show){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = show; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void color(unsigned const short bgc,
           unsigned const short font_color){
    /* changes the background and text color to a color between 0-15.
     * if the input is (16, 16), resets the colors to console default.
     */
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    if (bgc == 16 && font_color == 16){
        SetConsoleTextAttribute(out, 0x0F);

    } else {
        unsigned const short color = bgc * 16 + font_color;
        SetConsoleTextAttribute(out, color);
    }
}

char wait_for_kb_input(){
    /* waits for user keyboard input and returns the character they input */
    char key;
    while (1){
        if (_kbhit()){
            key = _getch();
            break;
        }
    }
    return key;
}

char get_kb_input(){
    /* doesn't wait for user keyboard press, just asks if there was one and 
     * returns the key value it was. If there was no input, returns -1
     */
    char key = -1;
    if (_kbhit())
        key = _getch();
    return key;
}

void set_cursor_pos(unsigned const int row, unsigned const int col){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = col;
    pos.Y = row;
    SetConsoleCursorPosition(out, pos);
}

void draw_pixel(Pixel pix){
    /* draws a pixel at the current cursor position */
    color(pix.fgc, pix.bgc);
    cout << pix.val << " ";
    color(16, 16);
    delay(16);
}

Pixel::Pixel(char value, unsigned short bg_color = 16,
      unsigned short fg_color = 16){
    val = value;
    bgc = bg_color;
    fgc = fg_color;
}

Pixel::Pixel(){}

bool operator == (Pixel& me, Pixel& other) {
    return me.bgc == other.bgc &&
           me.fgc == other.fgc &&
           me.val == other.val;
}

bool operator != (Pixel& me, Pixel& other){
    return !(me.bgc == other.bgc &&
           me.fgc == other.fgc &&
           me.val == other.val);
}

//board constructor
//defines the board var as a double vector of fillers in the dimensions
//given by the user
Board::Board(const unsigned int length, const unsigned int height,
            const Pixel filler_pix){
    //assign vals to class variables
    len = length;
    hei = height;
    filler = filler_pix;

    //create the fully empty board
    for (unsigned int i = 0; i < hei; i++){
        board.push_back(vector<Pixel> (len));
    }

    //set oldboard and freshboard to the fully empty board
    oldboard = board;
    freshboard = board;

    //fill the regular board with filler chars
    clear_board(false);
}

void Board::write(const unsigned int row, const unsigned int col,
        const Pixel pixel){
    /* writes a val to a given row, col in the board */
    board.at(row).at(col) = pixel;
}

void Board::write(const unsigned int row, const unsigned int col){
    board.at(row).at(col) = filler;
}

void Board::draw(unsigned const int height_offset, bool last_col_no_space){
    /* draws the updated parts of the board */

    //loop through game board
    for (unsigned int row = 0; row < hei; row++){
        for (unsigned int col = 0; col < len; col++){
            //if the current board doesn't match the old board at the row, col
            if (board.at(row).at(col) != oldboard.at(row).at(col)){
                //set the cursor pos to the right spot
                set_cursor_pos(row + height_offset, col*2);
                //draw the pixel
                if (last_col_no_space)
                    print_in_bounds(board.at(row).at(col), col);
                else
                    print_pixel(board.at(row).at(col));
            }
        }
    }
    //update the old board to the current board
    oldboard = board;
    //reset the color
    color(16, 16);
    //wait a frame to let the color change, for some reason it can break if it
    //doesn't wait a frame
    delay(16);
}

void Board::clear_board(const bool redraw_whole_board){
    /* sets all positions in the board to the filler character and totally
     * resets oldboard to redraw the whole screen if asked
     */
    for (unsigned int i = 0; i < hei; i++){
        for (unsigned int j = 0; j < len; j++){
            board.at(i).at(j) = filler;
        }
    }
    if (redraw_whole_board)
        oldboard = freshboard;
}


void Board::print_in_bounds(Pixel pix, unsigned const int col){
    if (col < len - 1){
        color(pix.bgc, pix.fgc);
        cout << pix.val << " ";
    } else {
        color(pix.bgc, pix.fgc);
        cout << pix.val;
    }
}

void Board::print_pixel(Pixel pix){
    color(pix.bgc, pix.fgc);
    cout << pix.val << " ";
}

Pixel Board::get_pix_at(unsigned const int row, unsigned const int col){
    return board.at(row).at(col);
}

