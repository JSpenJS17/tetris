#include "tetromino.hpp"

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



Tetromino::Tetromino(const char type){
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
                Pixel o_face = Pixel('o', LIGHT_YELLOW, LIGHT_YELLOW);
                blocks.push_back(Block(1, width/2-1, o_face));
                blocks.push_back(Block(1, width/2, o_face));
                blocks.push_back(Block(0, width/2-1, o_face));
                blocks.push_back(Block(0, width/2, o_face));
                break;
            }

        case 'i':
            {
                memcpy(shamts, i_shamt, sizeof(shamts));
                Pixel i_face = Pixel('i', SKY_BLUE, SKY_BLUE);
                blocks.push_back(Block(1, width/2, i_face));
                blocks.push_back(Block(1, width/2-1, i_face));
                blocks.push_back(Block(1, width/2-2, i_face));
                blocks.push_back(Block(1, width/2+1, i_face));
                break;
            }

        case 's':
            {
                memcpy(shamts, not_i_shamt, sizeof(shamts));
                Pixel s_face = Pixel('s', GREEN, GREEN);
                blocks.push_back(Block(1, width/2-1, s_face));
                blocks.push_back(Block(0, width/2-1, s_face));
                blocks.push_back(Block(0, width/2, s_face));
                blocks.push_back(Block(1, width/2-2, s_face));
                break;
            }

        case 'z':
            {
                memcpy(shamts, not_i_shamt, sizeof(shamts));
                Pixel z_face = Pixel('z', RED, RED);
                blocks.push_back(Block(1, width/2-1, z_face));
                blocks.push_back(Block(1, width/2, z_face));
                blocks.push_back(Block(0, width/2-2, z_face));
                blocks.push_back(Block(0, width/2-1, z_face));
                break;
            }

        case 'l':
            {
                memcpy(shamts, not_i_shamt, sizeof(shamts));
                Pixel l_face = Pixel('L', LIGHT_RED, LIGHT_RED);
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

Tetromino::Tetromino(){
    instantiated = false;
};

bool Tetromino::move_horizontal(bool left, const Stacked_Blocks& stack){
    uint i;
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

bool Tetromino::move_down(const Stacked_Blocks& stack, int amount/*= 1*/, 
                bool test/*= false*/){
    uint i;
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

void Tetromino::write(bool erase/*= false*/){
    for (uint i = 0; i < blocks.size(); i++){
        Block block = blocks.at(i);
        if (erase)
            game.write(block.row, block.col, bg);
        else
            game.write(block.row, block.col, block.face);
    }
}

int Tetromino::rotate(const bool clockwise, const Stacked_Blocks& stack){
    if (name == 'o')
        return 1;

    //temp block vector to store rotated blocks
    vector<Block> rotation = blocks;

    //define rotation point
    uint center_x = blocks.at(0).col;
    uint center_y = blocks.at(0).row;

    //rotate piece
    for (uint i = 0; i < blocks.size(); i++){
        int *col = &rotation.at(i).col;
        int *row = &rotation.at(i).row;
        *col -= center_x;
        *row -= center_y;

        if (clockwise){
            uint temp = *row;
            *row = *col;
            *col = -temp;
        }

        else{
            uint temp = *row;
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
    
    uint trans_set;
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
        
        for (uint i = 0; i < blocks.size(); i++){
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
            for (uint i = 0; i < blocks.size(); i++){
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
    for (uint i = 0; i < blocks.size(); i++){
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

void Tetromino::draw_at_pos(const uint row, const uint col, 
                    bool erase/*= false*/, bool write_not_draw/*= false*/){
    // we'll use these to find the offset of each block
    // in the tetromino from the center, which is always blocks[0]
    uint center_x = blocks.at(0).col;
    uint center_y = blocks.at(0).row;
    
    // set the pixel we're using to draw with
    Pixel draw_pix = erase? bg : blocks.at(0).face;

    // var to track of we should draw this block or not
    bool draw = true;

    // loop through the blocks
    for (uint i = 0; i < blocks.size(); i++){
        Block block = blocks.at(i);
        // block.row - center_y will give us the relative offset (+/- 1) 
            // from the center of the tetromino
        block.row -= center_y;
        block.col -= center_x;
        
        // add on the position they gave us
        block.row += row;
        block.col += col;

        if (draw){
            set_cursor_pos(block.row, block.col*2);
            if (write_not_draw){
                // should check bounds here, can't write outside of the Board
                if (block.row < (int)height && block.col < (int)width) {
                    game.write(block.row, block.col, draw_pix);
                }
            } else {
                draw_pixel(draw_pix);
            }
        } else {
            // if we ever set it to false, reset it here
            draw = true;
        }
    }
}

POS Tetromino::get_ghost_pos(const Stacked_Blocks& stack) {
    vector<Block> temp_blocks = blocks;

    // Assume all blocks can descend infinitely
    bool can_descend = true;

    while (can_descend) {
        // loop through our blocks
        for (auto& block : temp_blocks) {
            // Temporarily move the block down
            block.row++;

            // Check if we hit something
            if (stack.is_on(block) || (uint)block.row >= height) {
                // we did, we're done
                can_descend = false;
                break;
            }
        }
    }
    
    // return the origin of the piece - 1 cause we overshot to check collision
    return (POS){temp_blocks.at(0).row - 1, temp_blocks.at(0).col};
}

Tetromino::operator bool() const{
    return instantiated;
}

// Private:
uint Tetromino::get_trans_set(const int from_angle, const int dest_angle){
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

uint Tetromino::t_spin_check(int trans_index, const Stacked_Blocks& stack){
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

Bag::Bag(){
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

Tetromino Bag::get_piece(){
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

    uint piece_index;
    do { 
        //get a random number from 0-6
        piece_index = rand_int(6);
    //keep looping until you find an unused piece
    } while (used_pieces.at(piece_index) == true);
    
    used_pieces.at(piece_index) = true;
    //return the piece we got
    return pieces.at(piece_index);
}