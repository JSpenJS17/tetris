#ifndef TETROMINO_HPP
#define TETROMINO_HPP
#include "game.hpp"

struct Block {
    Block(const int init_row, const int init, const Pixel face_pix);

    void set_col_relative(const int change);

    void set_row_relative(const int change);

    bool is_out_of_bounds();
    
    bool operator==(const Block& other) const;

    Block();

    Pixel face;
    int row;
    int col;
};

class Stacked_Blocks{
    public:
        Stacked_Blocks();

        void add_blocks(const vector<Block>& piece);

        void add_block(const Block& block);

        void remove_block(const uint index);

        void write();

        bool is_on(const uint row, const uint col) const;

        bool is_on(const Block& block) const;

        uint clear_lines();

        bool is_empty() const;

    private:
        vector<Block> blocks;
        
        void clear_row(const uint row);
};

class Tetromino{
    public:
        Tetromino(const char type);

        Tetromino();

        bool move_horizontal(bool left, const Stacked_Blocks& stack);

        bool move_down(const Stacked_Blocks& stack, int amount = 1, 
                        bool test = false);

        void write(bool erase = false);

        int rotate(const bool clockwise, const Stacked_Blocks& stack);

        void draw_at_pos(const uint row, const uint col, 
                         bool erase = false, bool write_not_draw = false);

        POS get_ghost_pos(const Stacked_Blocks& stack);
        
        explicit operator bool() const;

    
        vector<Block> blocks;
        char name;

    private:
        bool instantiated;
        int angle;
        int shamts[8][5][2];
        uint get_trans_set(const int from_angle, const int dest_angle);

        uint t_spin_check(int trans_index, const Stacked_Blocks& stack);
};

struct Bag{
    vector<bool> used_pieces;
    vector<Tetromino> pieces;
    
    Bag();

    Tetromino get_piece();
};


#endif