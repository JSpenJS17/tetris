#include <iostream>
#include "engine.hpp"
#include <limits>
#include <time.h>
#include <cmath>

using namespace std;

const unsigned int ENTER = 13, ESC = 27, SPACE = 32,
                   UP = 72, LEFT = 75, DOWN = 80, RIGHT = 77;

const unsigned int width = 10;
const unsigned int height = 20;
const unsigned int lock_delay_reset = 750;
Pixel bg = Pixel('-', LIGHT_GRAY, LIGHT_GRAY);
Board game = Board(width, height, bg);

struct Block{
    Block(const int init_row, const int init_col, 
            const Pixel face_pix){
        row = init_row;
        col = init_col;
        face = face_pix;
    }

    void set_col_relative(const int change){
        col += change;
    }

    void set_row_relative(const int change){
        row += change;
    }

    bool is_out_of_bounds(){
        return (col < 0 || col >= width || row >= height);
    }

    Block(){}

    Pixel face;
    int row;
    int col;
};

class Stacked_Blocks{
    public:
        Stacked_Blocks(){}

        void add_blocks(const vector<Block>& piece){          
            blocks.insert(blocks.end(), 
                          piece.begin(), piece.end());
            write();
        }

        void add_block(const Block& block){
            blocks.push_back(block);
            write();
        }

        void remove_block(const unsigned int index){
            blocks.erase(blocks.begin() + index);
        }

        void write(){
            for (unsigned int i = 0; i < blocks.size(); i++){
                Block block = blocks.at(i);
                game.write(block.row, block.col, block.face);
            }
        }

        bool is_on(const unsigned int row, const unsigned int col) const{
            if (row >= height || col >= width)
                return true;

            for (unsigned int i = 0; i < blocks.size(); i++){
                Block stack_block = blocks.at(i);
                if (row == stack_block.row && col == stack_block.col)
                    return true;
            }

            return false;
        }

        bool is_on(const Block& block) const{
            for (unsigned int i = 0; i < blocks.size(); i++){
                Block stack_block = blocks.at(i);
                if (block.row == stack_block.row &&
                    block.col == stack_block.col)
                    return true;
            }
            return false;
        }

        unsigned int clear_lines(){
            unsigned int row;
            vector<Block> in_row;
            unsigned int cleared = 0;

            for (row = 0; row < height; row++){
                for (unsigned int i = 0; i < blocks.size(); i++){
                    if (blocks.at(i).row == row)
                        in_row.push_back(blocks.at(i));
                }
                if (in_row.size() == width){
                    clear_row(row);
                    cleared++;
                }
                in_row.clear();
            }

            write();
            return cleared;
        }

        bool is_empty() const{
            return blocks.size() == 0;
        }

    private:
        vector<Block> blocks;
        
        void clear_row(const unsigned int row){
            unsigned int i = 0;
            while (i < blocks.size()){
                Block block = blocks.at(i);
                if (block.row == row){
                    remove_block(i);
                    game.write(block.row, block.col, bg);
                } else if (block.row < row) {
                    game.write(block.row, block.col, bg);
                    blocks.at(i).set_row_relative(1);
                    i++;
                } else {
                    i++;
                }
            }
            write();
        }
};

class Tetromino{
    public:
        vector<Block> blocks;
        char name;

        Tetromino(const char type){
            instantiated = true;
            name = type;
            angle = 0;

            int not_i_shamt[8][5][2] = {
                                {{0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2}},
                                {{0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2}},
                                {{0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2}},
                                {{0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2}},
                                {{0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2}},
                                {{0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2}},
                                {{0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2}},
                                {{0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2}}
                                       };
            
            int i_shamt[8][5][2]     = {
                                {{0, 0}, {-2, 0}, { 1, 0}, {-2, -1}, { 1,  2}},
                                {{0, 0}, { 2, 0}, {-1, 0}, { 2,  1}, {-1, -2}},
                                {{0, 0}, {-1, 0}, { 2, 0}, {-1,  2}, { 2, -1}},
                                {{0, 0}, { 1, 0}, {-2, 0}, { 1, -2}, {-2,  1}},
                                {{0, 0}, { 2, 0}, {-1, 0}, { 2,  1}, {-1, -2}},
                                {{0, 0}, {-2, 0}, { 1, 0}, {-2, -1}, { 1,  2}},
                                {{0, 0}, { 1, 0}, {-2, 0}, { 1, -2}, {-2,  1}},
                                {{0, 0}, {-1, 0}, { 2, 0}, {-1,  2}, { 2, -1}}
                                       };


            //fill in blocks based on what the piece is.
            //blocks.at(0) is ALWAYS the pivot block.
            switch (name){
                case 't':
                    {
                        memcpy(shamts, not_i_shamt, sizeof(shamts));
                        Pixel t_face = Pixel('t', PURPLE, PURPLE);
                        blocks.push_back(Block(1, width/2-1, t_face));
                        blocks.push_back(Block(0, width/2-1, t_face));
                        blocks.push_back(Block(1, width/2-2, t_face));
                        blocks.push_back(Block(1, width/2, t_face));
                        break;
                    }

                case 'o':
                    {
                        Pixel o_face = Pixel('o', YELLOW, YELLOW);
                        blocks.push_back(Block(1, width/2-1, o_face));
                        blocks.push_back(Block(1, width/2, o_face));
                        blocks.push_back(Block(0, width/2-1, o_face));
                        blocks.push_back(Block(0, width/2, o_face));
                        break;
                    }

                case 'i':
                    {
                        memcpy(shamts, i_shamt, sizeof(shamts));
                        Pixel i_face = Pixel('i', CYAN, CYAN);
                        blocks.push_back(Block(1, width/2, i_face));
                        blocks.push_back(Block(1, width/2-1, i_face));
                        blocks.push_back(Block(1, width/2-2, i_face));
                        blocks.push_back(Block(1, width/2+1, i_face));
                        break;
                    }

                case 's':
                    {
                        memcpy(shamts, not_i_shamt, sizeof(shamts));
                        Pixel s_face = Pixel('s', LIGHT_GREEN, LIGHT_GREEN);
                        blocks.push_back(Block(1, width/2-1, s_face));
                        blocks.push_back(Block(0, width/2-1, s_face));
                        blocks.push_back(Block(0, width/2, s_face));
                        blocks.push_back(Block(1, width/2-2, s_face));
                        break;
                    }

                case 'z':
                    {
                        memcpy(shamts, not_i_shamt, sizeof(shamts));
                        Pixel z_face = Pixel('z', LIGHT_RED, LIGHT_RED);
                        blocks.push_back(Block(1, width/2-1, z_face));
                        blocks.push_back(Block(1, width/2, z_face));
                        blocks.push_back(Block(0, width/2-2, z_face));
                        blocks.push_back(Block(0, width/2-1, z_face));
                        break;
                    }

                case 'l':
                    {
                        memcpy(shamts, not_i_shamt, sizeof(shamts));
                        Pixel l_face = Pixel('L', GRAY, GRAY);
                        blocks.push_back(Block(1, width/2-1, l_face));
                        blocks.push_back(Block(0, width/2, l_face));
                        blocks.push_back(Block(1, width/2-2, l_face));
                        blocks.push_back(Block(1, width/2, l_face));
                        break;
                    }

                case 'j':
                    {
                        memcpy(shamts, not_i_shamt, sizeof(shamts));
                        Pixel j_face = Pixel('j', BLUE, BLUE);
                        blocks.push_back(Block(1, width/2-1, j_face));
                        blocks.push_back(Block(0, width/2-2, j_face));
                        blocks.push_back(Block(1, width/2-2, j_face));
                        blocks.push_back(Block(1, width/2, j_face));
                        break;
                    }
            }
        }

        Tetromino(){
            instantiated = false;
        };

        bool move_horizontal(bool left, const Stacked_Blocks& stack){
            unsigned int i;
            for (i = 0; i < blocks.size(); i++){
                Block block = blocks.at(i);
                
                //fake move the block
                if (left)
                    block.set_col_relative(-1);
                else
                    block.set_col_relative(1);

                //if that fake move put us out of bounds or on a stacked block
                if (stack.is_on(block) || block.is_out_of_bounds())
                    //the move failed
                    return false;
            }

            write(true);
            for (i = 0; i < blocks.size(); i++){
                if (left)
                    blocks.at(i).set_col_relative(-1);
                else
                    blocks.at(i).set_col_relative(1);
            }
            write();
            //the move succeeded
            return true;
        }

        bool move_down(const Stacked_Blocks& stack, const int amount = 1,
                const bool test = false){
            unsigned int i;
            for (i = 0; i < blocks.size(); i++){
                Block block = blocks.at(i);
                
                //fake move the block
                block.set_row_relative(amount);

                //if that fake move put us out of bounds or on a stacked block
                if (stack.is_on(block) || block.is_out_of_bounds())
                    //break out
                    return true;
            }

            if (!test){
                write(true);
                for (i = 0; i < blocks.size(); i++){
                    blocks.at(i).set_row_relative(amount);
                }
                write();
            }

            return false;
        }
        
        void write(bool erase = false){
            for (unsigned int i = 0; i < blocks.size(); i++){
                Block block = blocks.at(i);
                if (erase)
                    game.write(block.row, block.col, bg);
                else
                    game.write(block.row, block.col, block.face);
            }
        }

        int rotate(const bool clockwise, const Stacked_Blocks& stack){
            if (name == 'o')
                return 1;

            //temp block vector to store rotated blocks
            vector<Block> rotation = blocks;

            //define rotation point
            unsigned int center_x = blocks.at(0).col;
            unsigned int center_y = blocks.at(0).row;

            //rotate piece
            for (unsigned int i = 0; i < blocks.size(); i++){
                int *col = &rotation.at(i).col;
                int *row = &rotation.at(i).row;
                *col -= center_x;
                *row -= center_y;

                if (clockwise){
                    unsigned int temp = *row;
                    *row = *col;
                    *col = -temp;
                }

                else{
                    unsigned int temp = *row;
                    *row = -*col;
                    *col = temp;
                }

                //because the I piece midpoint is on a fraction, I just decided
                //to manually adjust all the rotations to where they should be.
                //it's the only piece like this.
                if (name == 'i'){
                    if (clockwise) {
                        switch(angle){
                            case 3:
                                *col = *col + 1;
                                break;
                            case 0:
                                *row = *row + 1;
                                break;
                            case 1:
                                *col = *col - 1;
                                break;
                            case 2:
                                *row = *row - 1;
                                break;
                        }
                    }
                    else {
                        switch(angle){
                            case 0:
                                *col = *col - 1;
                                break;
                            case 1:
                                *row = *row - 1;
                                break;
                            case 2:
                                *col = *col + 1;
                                break;
                            case 3:
                                *row = *row + 1;
                                break;
                        }
                    }
                }

                *col += center_x;
                *row += center_y;
            }
            
            /* we have the coordinates of all the rotated blocks now
             * these might be intersecting with the stack or be out of bounds.
             * so let's check some shamts[trans_set][trans_index]s of the rotation to see if they
             * are good.
             */

            // first, find the translations set we're using
            // this depends on angle and must be hardcoded
            
            unsigned int trans_set;
            if (clockwise){
                trans_set = get_trans_set(angle, (angle+1) % 4);
            }
            else{
                if (angle == 0)
                    trans_set = get_trans_set(angle, 3);
                else
                    trans_set = get_trans_set(angle, angle-1);
            }
            bool valid_rotation;
            int trans_index;

            //loop through all of the translationss in the translation set
            for (trans_index = 0; trans_index < 5; trans_index++){
                
                //assume valid rotation off the bat
                valid_rotation = true;
                
                for (int i = 0; i < blocks.size(); i++){
                    Block *block = &rotation.at(i);
                    //translate the row, col of the current block
                    block->col += shamts[trans_set][trans_index][0];
                    block->row -= shamts[trans_set][trans_index][1];
                    //check if it's good
                    if (stack.is_on(*block) || block->is_out_of_bounds()){
                        //it's not good, reset the block's coords
                        block->col -= shamts[trans_set][trans_index][0];
                        block->row += shamts[trans_set][trans_index][1];
                        //say it was a bad rotation
                        valid_rotation = false;
                        break;
                    }
                    //it's good, reset the block's coords
                    //Because we haven't checked all of the blocks, we can't
                    //assume the rotation is good yet. If it ends up being
                    //good, we'll shift them back later.
                    block->col -= shamts[trans_set][trans_index][0];
                    block->row += shamts[trans_set][trans_index][1];
                }

                //if the rotation was good
                if (valid_rotation){
                    //move everything back to the valid shamts[trans_set][trans_index]
                    for (unsigned int i = 0; i < blocks.size(); i++){
                        Block *block = &rotation.at(i);
                        block->col += shamts[trans_set][trans_index][0];
                        block->row -= shamts[trans_set][trans_index][1];
                    }
                    break;
                }
            }

            //finally, if there was no valid rotation available
            if (!valid_rotation){
                //return that there wasn't
                return 0;
            }

            //we know there was a valid rotation, so:

            //update the angle
            if (clockwise){
                angle++;
                angle %= 4;
            } 
            else {
                if (angle == 0)
                    angle = 3;
                else
                    angle--;
            }
            
            //erase the old piece
            write(true);

            //assign the rotated coords to the actual piece
            for (unsigned int i = 0; i < blocks.size(); i++){
                blocks.at(i) = rotation.at(i);
            }
            
            //write the new piece
            write();
            
            if (name == 't'){
                return t_spin_check(trans_index, stack);
            }
            else
                return 1;

        }

        void draw_at_pos(const unsigned int row, const unsigned int col){
            //center the block at the origin
            unsigned int center_x = blocks.at(0).col;
            unsigned int center_y = blocks.at(0).row;

            for (unsigned int i = 0; i < blocks.size(); i++){
                Block block = blocks.at(i);
                block.row -= center_y;
                block.col -= center_x;
                set_cursor_pos(block.row+row, block.col*2+col*2);
                draw_pixel(block.face);
            }
        }

        explicit operator bool() const{
            return instantiated;
        }

    private:
        bool instantiated;
        int angle;
        int shamts[8][5][2];
        unsigned int get_trans_set(const int from_angle, const int dest_angle){
            switch (from_angle){
                case 0:
                    if (dest_angle == 1)
                        return 0;
                    else if (dest_angle == 3)
                        return 7;
                    break;

                case 1:
                    if (dest_angle == 0)
                        return 1;
                    else if (dest_angle == 2)
                        return 2;
                    break;

                case 2:
                    if (dest_angle == 1)
                        return 3;
                    else if (dest_angle == 3)
                        return 4;
                    break;

                case 3:
                    if (dest_angle == 2)
                        return 5;
                    else if (dest_angle == 0)
                        return 6;
                    break;
            }
            throw invalid_argument("Bad angles given.");
        }      

        unsigned int t_spin_check(int trans_index, const Stacked_Blocks& stack){
            //t spin check. refer to the 2009 tetris guideline page 22
            bool a = false;
            bool b = false;
            bool c = false;
            bool d = false;
            if (name != 't')
                throw runtime_error("Only T pieces can check for T-spins.");

            //if we used the 5th translation on this t piece, it's a
            //guaranteed regular T spin
            if (trans_index == 4)
                return 3;

            //I figure that if we take the corners of the 3x3 of the t piece,
            //we can check if those are walls/on the stack.
            Block center = blocks.at(0);
            int row = center.row;
            int col = center.col;
            a = stack.is_on(row - 1, col - 1);
            b = stack.is_on(row - 1, col + 1);
            c = stack.is_on(row + 1, col - 1);
            d = stack.is_on(row + 1, col + 1);

            //rotate a, b, c, and d to always be the same relative to
            //the angle of the t piece.
            //first, store const values of the originals
            const bool temp_a = a;
            const bool temp_b = b;
            const bool temp_c = c;
            const bool temp_d = d;
            switch (angle) {
                case 0:
                    //already aligned correctly
                    //a -> b -> d -> c
                    break;
                case 1:
                    //rotated 90 degrees clockwise.
                    //c -> a -> b -> d
                    a = temp_b;
                    b = temp_d;
                    d = temp_c;
                    c = temp_a;
                    break;
                case 2:
                    //rotated 180 degrees
                    //d -> c -> a -> b
                    a = temp_d;
                    b = temp_c;
                    d = temp_a;
                    c = temp_b;
                    break;
                case 3:
                    //rotated 270 degrees clockwise
                    //b -> d -> c -> a
                    a = temp_c;
                    b = temp_a;
                    d = temp_b;
                    c = temp_d;
                    break;
            }
            
            if (a && b && (c || d))
                return 3;
            
            if (c && d && (a || b))
                return 2;
            
            return 1;
        }
};

struct Bag{
    vector<bool> used_pieces;
    vector<Tetromino> pieces;
    
    Bag(){
        for (int i = 0; i < 7; i++)
            used_pieces.push_back(false);
        pieces.push_back(Tetromino('t'));
        pieces.push_back(Tetromino('o'));
        pieces.push_back(Tetromino('i'));
        pieces.push_back(Tetromino('s'));
        pieces.push_back(Tetromino('z'));
        pieces.push_back(Tetromino('l'));
        pieces.push_back(Tetromino('j'));
    }

    Tetromino get_piece(){
        //check if all pieces have been used in this bag
        bool all_used = true;
        for (int i = 0; i < 7; i++){
            if (used_pieces.at(i) == false){
                all_used = false;
                break;
            }   
        }

        //if they have been all used
        if (all_used){
            //reset used pieces
            used_pieces.clear();
            for (int i = 0; i < 7; i++)
                used_pieces.push_back(false);
        }

        unsigned int piece_index;
        do { 
            //get a random number from 0-6
            piece_index = rand_int(6);
        //keep looping until you find an unused piece
        } while (used_pieces.at(piece_index) == true);
        
        used_pieces.at(piece_index) = true;
        //return the piece we got
        return pieces.at(piece_index);
    }
};

unsigned int main_menu(){
    unsigned int level_selected = 1;
    char key = -1;
    char buffer[3];
    color(BLACK, BLACK);
    cout << ".          ";
    
    const int title_bg = WHITE;

    color(title_bg, LIGHT_RED);
    cout << "T ";
    color(title_bg, YELLOW);
    cout << "E ";
    color(title_bg, LIGHT_GREEN);
    cout << "T ";
    color(title_bg, CYAN);
    cout << "R ";
    color(title_bg, BLUE);
    cout << "I ";
    color(title_bg, PURPLE);
    cout << "S";

    color(BLACK, BLACK);
    cout << "           " << endl << endl;

    color(WHITE, BLACK);
    cout << "Controls:" << endl;
    cout << "    Left/Right to move" << endl;
    cout << "    Down to soft drop" << endl;
    cout << "    Space to hard drop" << endl;
    cout << "    Up to rotate clockwise" << endl;
    cout << "    Z to rotate counter clockwise" << endl;
    cout << "    C to hold a piece" << endl << endl;

    cout << "Level: " << endl;
    cout << endl << "Press enter to start!";
    color(WHITE, RED);
    while (key != ENTER){
        set_cursor_pos(10, 7);
        sprintf(buffer, "%2d", level_selected);
        cout << buffer;
        key = wait_for_kb_input();

        switch (key){
            case UP:
                if (level_selected < 20)
                    level_selected++;
                break;

            case DOWN:
                if (level_selected > 1)
                    level_selected--;
                break;

            case RIGHT:
                {
                    int inc_num = 5;
                    if (level_selected == 1)
                        inc_num = 4;

                    for (int i = 0; i < inc_num; i++)
                        if (level_selected < 20)
                            level_selected++;
                    break;
                }

            case LEFT:
                for (int i = 0; i < 5; i++)
                    if (level_selected > 1)
                        level_selected--;
                break;
        }
    }

    color(16, 16);

    return level_selected;
}

void clear_score_output(){
    //clear previous score output
    color(BLACK, WHITE);
    for (int i = 0; i < 5; i++){
        set_cursor_pos(13+i, width * 2 + 1);
        cout << "             ";
    }
    color(16, 16);
}

unsigned int calculate_score(const unsigned int lines_cleared,
                             const Tetromino& piece, 
                             const Stacked_Blocks& stack, 
                             const unsigned int level, string* prev_clear,
                             const int t_spin_type){
    unsigned int score = 0;
    unsigned int row_index = 13;
    string cur_clear = "break b2b";
    
    clear_score_output();

    if (stack.is_empty()){
        set_cursor_pos(row_index++, width*2 + 1);
        score += 1250 * level;
        cout << "PERFECT CLEAR";
    }

    set_cursor_pos(row_index++, width*2 + 1);
    switch (lines_cleared){
        case 0:
            if (t_spin_type == 2){
                cur_clear = "don't break b2b";
                cout << "MINI T-SPIN";
                score += 100 * level;
            }
            else if (t_spin_type == 3){
                cur_clear = "don't break b2b";
                cout << "T-SPIN";
                score += 400 * level;
            }
            else {
                score = 0;
            }
            break;

        case 1:
            if (t_spin_type == 2){
                cur_clear = "b2b";
                cout << "MINI T-SPIN";
                set_cursor_pos(row_index++, width*2 + 1);
                score += 200 * level;
            }
            else if (t_spin_type == 3){
                cur_clear = "b2b";
                cout << "T-SPIN";
                set_cursor_pos(row_index++, width*2 + 1);
                score += 800 * level;
            }
            else {
                cur_clear = "break b2b";
                score += 100 * level;
            }
            cout << "SINGLE";
            break;

        case 2:
            if (t_spin_type == 3){
                cur_clear = "b2b";
                cout << "T-SPIN";
                set_cursor_pos(row_index++, width*2 + 1);
                score += 1200 * level;
            }
            else{
                cur_clear = "break b2b";
                score += 300 * level; 
            }
            cout << "DOUBLE";
            break;

        case 3:
            if (t_spin_type == 3){
                cur_clear = "b2b";
                cout << "T-SPIN";
                set_cursor_pos(row_index++, width*2 + 1);
                score += 1600 * level;
            }
            else{
                cur_clear = "break b2b";
                score += 500 * level;
            }
            cout << "TRIPLE";
            break;

        case 4:
            cur_clear = "b2b";
            cout << "TETRIS";
            score += 800 * level;
            break;
    }

    if (score > 0){
        if (*prev_clear == "b2b" && cur_clear == "b2b"){
            set_cursor_pos(row_index++, width*2 + 1);
            score *= 1.5;
            cout << "BACK-TO-BACK";
        }
        
        if (cur_clear != "don't break b2b")
            *prev_clear = cur_clear;
        
        set_cursor_pos(row_index++, width*2 + 1);
        cout << score;
    }


    color(16, 16);
    return score;
}

int main(){
    clear_screen();
    show_cursor(false);
    //get a random seed
    srand(time(NULL));
    bool play_again = true;

    while (play_again){
        const int level_selected = main_menu();
        clear_screen();
        int level = level_selected;

        Stacked_Blocks stack;
        Bag bag;

        char key = -1;
        Tetromino piece = bag.get_piece();
        piece.write();
        piece.move_down(stack);

        set_cursor_pos(0, width*2+1);
        color(GRAY, WHITE);
        cout << "Next";
        set_cursor_pos(5, width*2+1);
        cout << "Held";
        color(16, 16);

        Tetromino next_piece = bag.get_piece();
        next_piece.draw_at_pos(3, width+3);

        Tetromino held_piece;

        clock_t frame_start_time;
        clock_t frame_end_time;

        bool hit_bottom = false;
        int successful_move;
        bool reset_piece = false;
        bool has_held = false;
        bool hard_dropping = false;
        bool game_over = false;
        unsigned int frame_counter = 0;
        unsigned int current_lines_cleared = 0;
        unsigned int line_total = 0;
        unsigned int reset_count = 0;
        unsigned int t_spin = 0;
        int lock_delay = lock_delay_reset;
        unsigned long score = 0;
        float gravity = (float) 1/60;
        char buffer[10];
        string prev_clear = "nothing";
        string* prev_clear_ptr = &prev_clear;
        
        while (!game_over){
            key = get_kb_input();

            frame_start_time = clock();
            
            //gravity calculation
            gravity = (1.0/60.0) * (1.0/pow(.8-(((float) level-1)*.007), level-1));

            switch (key){
                case LEFT:
                    successful_move = piece.move_horizontal(true, stack);
                    hit_bottom = piece.move_down(stack, 1, true);
                    if (hit_bottom && successful_move){
                        lock_delay = lock_delay_reset;
                        reset_count++;
                    }
                    break;

                case RIGHT:
                    successful_move = piece.move_horizontal(false, stack);
                    hit_bottom = piece.move_down(stack, 1, true);
                    if (hit_bottom && successful_move){
                        lock_delay = lock_delay_reset;
                        reset_count++;
                    }
                    break;

                case DOWN:
                    hit_bottom = piece.move_down(stack);
                    if (!hit_bottom)
                        score += 1;
                    break;

                case UP:
                    successful_move = piece.rotate(true, stack);
                    hit_bottom = piece.move_down(stack, 1, true);
                    if (successful_move && hit_bottom){
                        lock_delay = lock_delay_reset;
                        reset_count++;
                    }
                    break;

                case ESC:
                    key = -1;
                    while (wait_for_kb_input() != ESC);
                    break;

                case 'z':
                    successful_move = piece.rotate(false, stack);
                    hit_bottom = piece.move_down(stack, 1, true);
                    if (successful_move && hit_bottom){
                        lock_delay = lock_delay_reset;
                        reset_count++;
                    }
                    break;

                case 'c':
                    //if the player has not held this turn
                    if (!has_held){
                        //erase the current piece
                        piece.write(true);

                        //if held_piece exists
                        if (held_piece){
                            //swap piece and held piece 
                            //and reset piece coords as it's stored
                            Tetromino temp = held_piece;
                            held_piece = Tetromino(piece.name);
                            piece = temp;
                            //set the has_held this turn flag to true
                            has_held = true;

                        //if held_piece does not exist
                        } else {
                            //store the current piece (reset coords) in held_piece
                            held_piece = Tetromino(piece.name);
                            //set the current piece to the next one in line
                            piece = next_piece;
                            //update the next_piece in line
                            next_piece = bag.get_piece();

                            //clear the next piece spot
                            color(16, 16);
                            for (int row = 2; row < 4; row++){
                                set_cursor_pos(row, width*2);
                                cout << "            ";
                            }

                            //draw it in the next piece spot
                            next_piece.draw_at_pos(3, width+3);

                            has_held = true;
                        }

                        //whatever piece is, write it
                        piece.write();
                        
                        //clear the held piece slot
                        for (int row = 7; row < 9; row++){
                            set_cursor_pos(row, width*2);
                            cout << "            ";
                        }

                        //draw the held piece in the held piece slot
                        held_piece.draw_at_pos(8, width+3);

                        //and reset our variables
                        reset_piece = true;
                    }
                    break;

                case SPACE:
                    gravity = 20;
                    hard_dropping = true;
                    break;
                    
            }

            /* GRAVITY */

            frame_counter += 1;
            if (frame_counter * gravity >= 1){
                int moves = frame_counter * gravity;
                for (int i = 0; i < moves; i++){
                    hit_bottom = piece.move_down(stack);
                    if (hard_dropping)
                        score += 2;
                    if (hit_bottom)
                        break;
                }
                if (hard_dropping)
                    hard_dropping = false;
                frame_counter = 0;
            }

            /* A BLOCK LANDED BUT ISN'T PLACED YET */

            if (hit_bottom){
                if (key == SPACE)
                    lock_delay = 0;
                lock_delay -= 16;
            }

            /* PLACED A BLOCK */

            //if the piece has hit the bottom and the player is out of lock delay
            //or they've reset the lock delay 15 times
            if (lock_delay <= 0 || reset_count >= 15){
                //reset the parts of the piece
                reset_piece = true;
                //add the current piece to the stack of blocks
                stack.add_blocks(piece.blocks);
                //clear lines and store how many were cleared
                current_lines_cleared = stack.clear_lines();
                clear_score_output();
                t_spin = successful_move;
                score += calculate_score(current_lines_cleared, piece,
                                        stack, level, prev_clear_ptr, t_spin);
                line_total += current_lines_cleared;

                if (level < 20)
                    level = line_total/10 + level_selected;
                //change current piece to the previous next one
                piece = next_piece;
                piece.write();

                //get a new next piece
                next_piece = bag.get_piece();

                //clear the Next: piece
                color(16, 16);
                for (int row = 2; row < 4; row++){
                    set_cursor_pos(row, width*2);
                    cout << "            ";
                }

                //draw it in the next piece spot
                next_piece.draw_at_pos(3, width+3);

                //only if they actually placed a block can we reset if they've held
                //a block.
                has_held = false;
            }

            /* JUST SWAPPED CURRENT PIECE */

            //If the player needs a new piece because they held the previous one
            //or because they just placed their block
            if (reset_piece){            
                //reset tracker variables
                hit_bottom = piece.move_down(stack);
                lock_delay = lock_delay_reset;
                reset_count = 0;
                reset_piece = false;

                //game over check, if we can't spawn a new piece, ripperino
                for (int i = 0; i < piece.blocks.size(); i++){
                    if (stack.is_on(piece.blocks.at(i)))
                        game_over = true;
                }
            }

            /* UPDATE DISPLAY */

            game.draw(0, 0);

            set_cursor_pos(height+1, 0);

            color(GRAY, WHITE);
            sprintf(buffer, "%7d", level);
            cout << "Level: " << buffer << endl;

            sprintf(buffer, "%7d", line_total);
            cout << "Lines: " << buffer << endl;
            
            sprintf(buffer, "%7d", score);
            cout << "Score: " << buffer;
            
            color(16, 16);

            frame_end_time = clock();
            delay(16-(frame_end_time-frame_start_time));    
        }

        /* PRINT GAME OVER SCREEN */
        clear_screen();
        color(WHITE, RED);
        cout << "Game Over" << endl;

        color(WHITE, BLACK);
        sprintf(buffer, "%7d", level_selected);
        cout << "Starting Level: " << buffer << endl;
        
        sprintf(buffer, "%7d", level);
        cout << "Final Level:    " << buffer << endl;

        sprintf(buffer, "%7d", line_total);
        cout << "Lines Cleared:  " << buffer << endl;

        sprintf(buffer, "%7d", score);
        cout << "Score:          " << buffer << endl << endl;

        color(16, 16);
        cout << "Press enter to play again." << endl;
        cout << "Press escape to quit.";

        key = -1;
        while (key != ENTER && key != ESC){
            key = wait_for_kb_input();
        }
        play_again = key == ENTER;
        clear_screen();
        game.clear_board(true);
    }

    return 0;
}

