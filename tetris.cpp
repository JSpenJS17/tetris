#include <iostream>
#include "engine.hpp"
using namespace std;

#define ENTER 13
#define ESC 27
#define UP 72
#define LEFT 75
#define DOWN 80
#define RIGHT 77

unsigned int width = 10;
unsigned int height = 20;
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

        void add_blocks(vector<Block> piece){          
            blocks.insert(blocks.end(), 
                          piece.begin(), piece.end());
            write();
        }

        void add_block(const Block block){
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

        bool is_on(Block block){
            for (unsigned int i = 0; i < blocks.size(); i++){
                Block stack_block = blocks.at(i);
                if (block.row == stack_block.row &&
                    block.col == stack_block.col)
                    return true;
            }
            return false;
        }

        int clear_lines(){
            unsigned int row;
            vector<Block> in_row;
            for (row = 0; row < height; row++){
                for (unsigned int i = 0; i < blocks.size(); i++){
                    if (blocks.at(i).row == row)
                        in_row.push_back(blocks.at(i));
                }
                if (in_row.size() == width)
                    clear_row(row);
                in_row.clear();
            }
            write();
            return 0;
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

class Tetrimino{
    public:
        vector<Block> blocks;
        Tetrimino(const char type){
            switch (type){
                case 't':
                    {
                        Pixel t_face = Pixel('t', PURPLE, PURPLE);
                        blocks.push_back(Block(0, width/2-1, t_face));
                        blocks.push_back(Block(1, width/2-2, t_face));
                        blocks.push_back(Block(1, width/2-1, t_face));
                        blocks.push_back(Block(1, width/2, t_face));
                        break;
                    }

                case 'o':
                    {
                        Pixel o_face = Pixel('o', YELLOW, YELLOW);
                        blocks.push_back(Block(0, width/2-1, o_face));
                        blocks.push_back(Block(0, width/2, o_face));
                        blocks.push_back(Block(1, width/2-1, o_face));
                        blocks.push_back(Block(1, width/2, o_face));
                        break;
                    }

                case 'i':
                    {
                        Pixel i_face = Pixel('i', CYAN, CYAN);
                        for (int i = -2; i < 2; i++){
                            blocks.push_back(Block(1, width/2+i, i_face));
                        }
                        break;
                    }

                case 's':
                    {
                        Pixel s_face = Pixel('s', LIGHT_GREEN, LIGHT_GREEN);
                        blocks.push_back(Block(0, width/2-1, s_face));
                        blocks.push_back(Block(0, width/2, s_face));
                        blocks.push_back(Block(1, width/2-2, s_face));
                        blocks.push_back(Block(1, width/2-1, s_face));
                        break;
                    }

                case 'z':
                    {
                        Pixel z_face = Pixel('z', LIGHT_RED, LIGHT_RED);
                        blocks.push_back(Block(1, width/2-1, z_face));
                        blocks.push_back(Block(1, width/2, z_face));
                        blocks.push_back(Block(0, width/2-2, z_face));
                        blocks.push_back(Block(0, width/2-1, z_face));
                        break;
                    }

                case 'l':
                    {
                        Pixel l_face = Pixel('L', GRAY, GRAY);
                        blocks.push_back(Block(0, width/2, l_face));
                        for (int i = -2; i < 1; i++){
                            blocks.push_back(Block(1, width/2+i, l_face));
                        }
                        break;
                    }

                case 'j':
                    {
                        Pixel j_face = Pixel('j', BLUE, BLUE);
                        blocks.push_back(Block(0, width/2-2, j_face));
                        for (int i = -2; i < 1; i++){
                            blocks.push_back(Block(1, width/2+i, j_face));
                        }
                        break;
                    }
            }
        }

        Tetrimino(){};

        bool move_horizontal(bool left, Stacked_Blocks stack){
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
                    //break out
                    return true;
            }

            write(true);
            for (i = 0; i < blocks.size(); i++){
                if (left)
                    blocks.at(i).set_col_relative(-1);
                else
                    blocks.at(i).set_col_relative(1);
            }
            write();
            return false;
        }

        bool move_down(Stacked_Blocks stack){
            unsigned int i;
            for (i = 0; i < blocks.size(); i++){
                Block block = blocks.at(i);
                
                //fake move the block
                block.set_row_relative(1);

                //if that fake move put us out of bounds or on a stacked block
                if (stack.is_on(block) || block.is_out_of_bounds())
                    //break out
                    return true;
            }

            write(true);
            for (i = 0; i < blocks.size(); i++){
                blocks.at(i).set_row_relative(1);
            }
            write();

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
};

struct Bag{
    vector<bool> used_pieces;
    vector<Tetrimino> pieces;
    
    Bag(){
        for (int i = 0; i < 7; i++)
            used_pieces.push_back(false);
        pieces.push_back(Tetrimino('t'));
        pieces.push_back(Tetrimino('o'));
        pieces.push_back(Tetrimino('i'));
        pieces.push_back(Tetrimino('s'));
        pieces.push_back(Tetrimino('z'));
        pieces.push_back(Tetrimino('l'));
        pieces.push_back(Tetrimino('j'));
    }

    Tetrimino get_piece(){
        //check if all pieces have been used in this bag
        bool all_used = true;
        for (int i = 0; i < 6; i++){
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

int main(){
    clear_screen();
    show_cursor(false);
    //get a random seed
    srand(time(NULL));

    Stacked_Blocks stack;
    Bag bag;

    char key = -1;
    Tetrimino piece = bag.get_piece();
    piece.write();

    unsigned int clock = 0;
    bool hit_bottom = false;

    while (key != ESC){
        key = get_kb_input();
        switch (key){
            case LEFT:
                piece.move_horizontal(true, stack);
                break;
            case RIGHT:
                piece.move_horizontal(false, stack);
                break;
            case DOWN:
                hit_bottom = piece.move_down(stack);
                break;
        }

        game.draw(0, 0);
        delay(16);
        clock += 1;
        if (clock % 15 == 0)
            hit_bottom = piece.move_down(stack);

        if (hit_bottom){
            hit_bottom = false;
            stack.add_blocks(piece.blocks);
            piece = bag.get_piece();
            stack.clear_lines();
        }

        cin.clear();
    }

    return 0;
}

