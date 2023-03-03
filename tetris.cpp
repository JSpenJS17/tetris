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

    private:
        vector<Block> blocks;
};

class Tetrimino{
    public:
        vector<Block> blocks;
        Tetrimino(const char type){
            switch (type){
                case 't':
                    Pixel t_face = Pixel('t', PURPLE, PURPLE);
                    blocks.push_back(Block(0, width/2-1, t_face));
                    blocks.push_back(Block(1, width/2-2, t_face));
                    blocks.push_back(Block(1, width/2-1, t_face));
                    blocks.push_back(Block(1, width/2, t_face));
                    break;
            }
            write();
        }

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
        
    private:
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

int main(){
    clear_screen();
    show_cursor(false);
    Stacked_Blocks stack;

    char key = -1;
    Tetrimino piece = Tetrimino('t');

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
            piece = Tetrimino('t');
        }

        cin.clear();
    }

    return 0;
}

