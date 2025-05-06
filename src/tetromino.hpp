#ifndef TETROMINO_HPP
#define TETROMINO_HPP
#include "game.hpp"

struct Block;
class Stacked_Blocks;

#define ERASE true

enum ROTATION_VALS {
    INVALID,
    VALID,
    TSPIN_MINI,
    TSPIN,
    NUM_ROTATION_VALS
};

enum ANGLES {
    R0,     //   0 degrees clockwise
    R90,    //  90 degrees clockwise
    R180,   // 180 degrees clockwise
    R270,   // 270 degrees clockwise
    NUM_ANGLES
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

        void set_ghost_pos(const Stacked_Blocks& stack);
        
        explicit operator bool() const;

        vector<Block> blocks; // all four of our blocks
        char name; // 't', 'l', 'j', etc.

    private:
        bool instantiated;
        int angle; // angle of tetromino 
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