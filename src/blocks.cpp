#include "blocks.hpp"

Block::Block(const int init_row, const int init_col, const Pixel face_pix){
    row = init_row;
    col = init_col;
    face = face_pix;
}

Block::Block(){}

void Block::set_col_relative(const int change){
    col += change;
}

void Block::set_row_relative(const int change){
    row += change;
}

bool Block::is_out_of_bounds(){
    return (row < 0 || col < 0 || col >= (int)width || row >= (int)height);
}

bool Block::operator==(const Block& other) const {
    return row == other.row && col == other.col;
}


Stacked_Blocks::Stacked_Blocks(){}

void Stacked_Blocks::add_blocks(const vector<Block>& piece){          
    blocks.insert(blocks.end(), piece.begin(), piece.end());
}

void Stacked_Blocks::add_block(const Block& block){
    blocks.push_back(block);
}

void Stacked_Blocks::remove_block(const uint index){
    blocks.erase(blocks.begin() + index);
}

void Stacked_Blocks::write(){
    for (uint i = 0; i < blocks.size(); i++){
        Block block = blocks.at(i);
        game.write(block.row, block.col, block.face);
    }
}

bool Stacked_Blocks::is_on(const uint row, const uint col) const{
    if (row >= height || col >= width)
        return true;

    for (uint i = 0; i < blocks.size(); i++){
        Block stack_block = blocks.at(i);
        if ((int)row == stack_block.row && (int)col == stack_block.col)
            return true;
    }

    return false;
}

bool Stacked_Blocks::is_on(const Block& block) const{
    for (uint i = 0; i < blocks.size(); i++){
        Block stack_block = blocks.at(i);
        if (block.row == stack_block.row &&
            block.col == stack_block.col)
            return true;
    }
    return false;
}

uint Stacked_Blocks::clear_lines(){
    uint row;
    vector<Block> in_row;
    uint cleared_rows = 0;

    // loop through all the rows
    for (row = 0; row < height; row++){
        // loop through all the blocks
        for (uint i = 0; i < blocks.size(); i++){
            // if the block is in the current row
            if (blocks.at(i).row == (int)row)
                // add it to the in_row vector
                in_row.push_back(blocks.at(i));
        }
        // if the row is full, clear it
        if (in_row.size() == width){
            // only clear the row if we've been told to
            clear_row(row);
            // otherwise, add it to the count of cleared rows
            cleared_rows++;
        }
        // reset in_row vector
        in_row.clear();
    }

    // update the screen
    return cleared_rows;
}

bool Stacked_Blocks::is_empty() const{
    return blocks.size() == 0;
}

void Stacked_Blocks::clear_row(const uint row){
    uint i = 0;
    while (i < blocks.size()){
        Block block = blocks.at(i);
        if (block.row == (int)row){
            remove_block(i);
            game.write(block.row, block.col, bg);
        } else if (block.row < (int)row) {
            game.write(block.row, block.col, bg);
            blocks.at(i).set_row_relative(1);
            i++;
        } else {
            i++;
        }
    }
}
