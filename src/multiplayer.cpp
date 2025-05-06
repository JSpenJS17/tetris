#include "multiplayer.hpp"

Board other_game = game;

void draw_games() {
    other_game = game; // substitute for thread getting their actual game data
    other_game.draw(0, false, width*3);
    game.draw();
    fflush(stdout);
}

void multiplayer() {
    clear_screen();
    Stacked_Blocks stack;
    Bag bag;

    char key = -1;
    Tetromino piece = bag.get_piece();
    piece.write();

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
    int successful_move; // 0 if INVALID, 1 if VALID
    int t_spin = 0;      // 2 if TSPIN_MINI, 3 if TSPIN, 0 if NONE
    bool reset_piece = false;
    bool has_held = false;
    bool hard_dropping = false;
    bool game_over = false;
    bool made_move = false;
    uint frame_counter = 0;
    uint current_lines_cleared = 0;
    uint reset_count = 0;
    int lock_delay = lock_delay_reset;
    float gravity = (float) 1/60;
    char buffer[32];
    string prev_clear = "nothing";
    string* prev_clear_ptr = &prev_clear;
    Tetromino ghost_piece;

    // initialize the board to blank
    draw_games();
    
    if (gamedata.use_ghost) {
        // draw initial ghost
        ghost_piece = piece;
        ghost_piece.set_ghost_pos(stack);
        ghost_piece.write();
    }


    while (!game_over){
        frame_start_time = clock();

        key = get_kb_input();
        
        //gravity calculation
        gravity = (1.0/60.0) * (1.0/pow(.8-(((float) level-1)*.007), level-1));

        /* INPUT HANDLING */

        switch (key){
            case LEFT:
                successful_move = piece.move_horizontal(true, stack);
                made_move = true;
                break;

            case RIGHT:
                successful_move = piece.move_horizontal(false, stack);
                made_move = true;
                break;

            case DOWN:
                hit_bottom = piece.move_down(stack);
                if (!hit_bottom)
                    score += 1;
                break;

            case UP:
                successful_move = piece.rotate(true, stack);
                made_move = true;
                break;

            case 'z':
                successful_move = piece.rotate(false, stack);
                made_move = true;
                break;

            case 'c':
                //if the player has not held this turn
                if (!has_held){
                    lock_delay = lock_delay_reset;
                    //erase the current piece
                    piece.write(ERASE);

                    // erase the current ghost if we're using ghost
                    if (gamedata.use_ghost) {
                        // remove old ghost
                        ghost_piece.write(ERASE);
                    }

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


        if (successful_move == TSPIN || successful_move == TSPIN_MINI) {
            // store t_spin status
            t_spin = successful_move;
            successful_move = VALID;
        }

        /* 
         * intentionally use previous hit_bottom val here
         *    we want to know if we were just on the floor, 
         *    not if we are on the floor after the movement
         */
        if (made_move && successful_move && hit_bottom) {
            // if we made a move and it was successful and we were just on the floor,
            // we reset lock delay
            lock_delay = lock_delay_reset;
            reset_count++;

            // update our hit_bottom value so we know if the movement we just made put us on the floor
            hit_bottom = piece.move_down(stack, 1, true);
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

        /* A PIECE LANDED BUT ISN'T PLACED YET */

        if (hit_bottom){
            if (key == SPACE)
                lock_delay = 0;
            // if framerate is changed, this -16 will have to change as well
            lock_delay -= 16;
        }

        /* GHOST PIECE */

        if (gamedata.use_ghost) {
            // if the move was a success, remove the old ghost
            ghost_piece.write(ERASE);
            // hacky fix to make sure the ghost doesn't overwrite the piece
            piece.write();
            // update the ghost
            ghost_piece = piece;
            // update the ghost position
            ghost_piece.set_ghost_pos(stack);
            // draw new ghost
            ghost_piece.write();
        }

        // reset the made_move flag
        made_move = false;

        /* PLACED A PIECE */

        //if the piece has hit the bottom and the player is out of lock delay
        //or they've reset the lock delay 15 times
        if (lock_delay <= 0 || reset_count >= 15) {
            //reset the parts of the piece
            reset_piece = true;

            //add the current piece to the stack of blocks
            stack.add_blocks(piece.blocks);

            //clear lines and store how many were cleared
            current_lines_cleared = stack.clear_lines();
            line_total += current_lines_cleared;
            t_spin = 0;
            
            if (level < 20)
                level = line_total/10 + gamedata.starting_level;
            //change current piece to the previous next one
            piece = next_piece;
            // new piece is written later
            // This prevents some dumb graphical bug
            
            // write the stack to make sure we didn't miss anything
            stack.write();

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

            //only if they actually placed a block can we reset if they've used their hold
            has_held = false;
        }

        /* JUST SWAPPED CURRENT PIECE */

        //If the player needs a new piece because they held the previous one
        //or because they just placed their block
        if (reset_piece){            
            //reset tracker variables
            // Not sure why we update hit_bottom, but don't actually move the piece
            hit_bottom = piece.move_down(stack, 1, true);
            lock_delay = lock_delay_reset;
            reset_count = 0;
            reset_piece = false;

            //game over check, if we can't spawn a new piece, ripperino
            for (uint i = 0; i < piece.blocks.size(); i++){
                if (stack.is_on(piece.blocks.at(i)))
                    game_over = true;
            }
        }

        /* UPDATE DISPLAY */
        piece.write();
        stack.write();

        draw_games();

        set_cursor_pos(height+1, 0);

        color(GRAY, WHITE);
        sprintf(buffer, "%7d", level);
        cout << "Level: " << buffer << endl;

        sprintf(buffer, "%7d", line_total);
        cout << "Lines: " << buffer << endl;
        
        sprintf(buffer, "%7lu", score);
        cout << "Score: " << buffer;
        
        color(16, 16);

        frame_end_time = clock();
        int total_time = (frame_end_time - frame_start_time) / (CLOCKS_PER_SEC / 1000);

        fflush(stdin); // flush stdin to prevent input lag
        delay(16 - total_time);
    }
}