#ifndef BLOCKS_HPP
#define BLOCKS_HPP
#include "game.hpp"

struct Block{
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

        bool is_on(const int row, const int col) const;

        bool is_on(const Block& block) const;

        uint clear_lines();

        bool is_empty() const;

        void create_garbage(int num_lines);

    private:
        vector<Block> blocks;
        
        void clear_row(const uint row);
};

#endif