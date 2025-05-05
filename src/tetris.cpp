#include "game.hpp"

// externs
const uint width = 10;
const uint height = 20;
const uint lock_delay_reset = 750;
Pixel bg = Pixel('-', LIGHT_GRAY, LIGHT_GRAY);
Board game = Board(width, height, bg);
ulong score = 0;
GAMETYPE game_type;
uint line_total = 0;
int level_selected = 0;
int level = 0;

int main_menu(){
    // Returns an int representing the main menu selections
        // abs value of the int is starting level
        // + int means YES ghost piece
        // - int means NO  ghost piece
    int level_selected = 1;
    char key = -1;
    char buffer[12];
    color(16, 16);
    cout << "           ";
    
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

    color(16, 16);
    cout << "           " << endl << endl;

    color(WHITE, BLACK);
    cout << "Controls:" << endl;
    cout << "    Left/Right to move" << endl;
    cout << "    Down to soft drop" << endl;
    cout << "    Space to hard drop" << endl;
    cout << "    Up to rotate clockwise" << endl;
    cout << "    Z to rotate counter clockwise" << endl;
    cout << "    C to hold a piece" << endl;
    cout << "    ESC to pause" << endl << endl;

    cout << "Level (arrows): " << endl;
    cout << "Ghost piece (space):   " << endl;
    cout << endl << "Press enter to start!";
    while (key != ENTER){
        // Do level display
        color(WHITE, RED);
        // Will have to change 10, 16 if menu text changes
        set_cursor_pos(11, 16);
        sprintf(buffer, "%2d", abs(level_selected));
        cout << buffer;
        
        // Do Ghost piece display
        // Will have to change the 11, 21 if menu text changes
        set_cursor_pos(12, 21);
        string yesno = "UND";
        // negative level_selected means NO ghost piece
            // just to encode some more info in the return of this menu
        if (level_selected < 0) {
            color(WHITE, RED);
            yesno = "NO ";
        } else {
            color(WHITE, GREEN);
            yesno = "YES";
        }
        cout << yesno;

        fflush(stdout); // need to flush here due to linux not 
        key = wait_for_kb_input();

        switch (key){
            case UP:
                if (abs(level_selected) < 20)
                    level_selected = level_selected > 0? level_selected + 1 : level_selected - 1;
                break;

            case DOWN:
                if (abs(level_selected) > 1)
                    level_selected = level_selected > 0? level_selected - 1 : level_selected + 1;
                break;

            case RIGHT:
                {
                    int inc_num = 5;
                    if (abs(level_selected) == 1)
                        inc_num = 4;

                    for (int i = 0; i < inc_num; i++)
                        if (abs(level_selected) < 20)
                            level_selected = level_selected > 0? level_selected + 1 : level_selected - 1;
                    break;
                }

            case LEFT:
                for (int i = 0; i < 5; i++)
                    if (abs(level_selected) > 1)
                        level_selected = level_selected > 0? level_selected - 1 : level_selected + 1;

                break;

            case SPACE:
                level_selected = -level_selected;
                break;
        }
    }

    color(16, 16);

    return level_selected;
}

void clear_score_output(){
    //clear previous score output
    color(16, 16);
    for (int i = 0; i < 5; i++){
        set_cursor_pos(13+i, width * 2 + 1);
        cout << "             ";
    }
    color(16, 16);
}

uint calculate_score(const uint lines_cleared,
                     const Tetromino& piece, 
                     const Stacked_Blocks& stack, 
                     const uint level, string* prev_clear,
                     const int t_spin_type){
    uint score = 0; 
    uint row_index = 13;
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
            if (t_spin_type == TSPIN_MINI){
                cur_clear = "don't break b2b";
                cout << "MINI T-SPIN";
                score += 100 * level;
            }
            else if (t_spin_type == TSPIN){
                cur_clear = "don't break b2b";
                cout << "T-SPIN";
                score += 400 * level;
            }
            else {
                score = 0;
            }
            break;

        case 1:
            if (t_spin_type == TSPIN_MINI){
                cur_clear = "b2b";
                cout << "MINI T-SPIN";
                set_cursor_pos(row_index++, width*2 + 1);
                score += 200 * level;
            }
            else if (t_spin_type == TSPIN){
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
            if (t_spin_type == TSPIN){
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
            if (t_spin_type == TSPIN){
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

void print_scoreboard() {
    #ifdef SCOREBOARD
    TopScore* top_ten = get_scores(whoami(), score);
    if (top_ten) 
    {
        printf("Top Ten Global Scores:\n");
        printf("     Score Name\n");
        for (int i = 0; i < num_topscores; i++)
        {
            printf("%2d %7d %s\n", i + 1, top_ten[i].score, top_ten[i].name);
        }
        if (placement != -1)
        {
            printf("\nYou placed number %d globally.\n\n", placement);
        }
    } 
    else 
    {
        printf("Failed to grab global scores.\n\n");
    }
    free(top_ten);
    #endif
}

void reset_everything() {
    color(16, 16);
    show_cursor(true);
    close_engine();
}

void game_over_screen() {
    /* PRINT GAME OVER SCREEN and EXIT */
    char buffer[32];

    clear_screen();
    color(WHITE, RED);
    cout << endl << "Game Over" << endl;

    color(WHITE, BLACK);
    sprintf(buffer, "%7d", level_selected);
    cout << "Starting Level: " << buffer << endl;
    
    sprintf(buffer, "%7d", level);
    cout << "Final Level:    " << buffer << endl;

    sprintf(buffer, "%7d", line_total);
    cout << "Lines Cleared:  " << buffer << endl;

    sprintf(buffer, "%7lu", score);
    cout << "Score:          " << buffer << endl << endl;

    color(16, 16);

    print_scoreboard();

    cout << "Press enter to exit." << endl;
    char key = -1;
    while (key != ENTER) {
        key = wait_for_kb_input();
    }

    reset_everything();
    exit(0);
}

void sigint_handler(int dummy) {
    // act like it's a game over
    game_over_screen();
}

void pause_game() {
    const char* paused = "  PAUSED  ";
    Pixel previous_pix[strlen(paused)];

    // put PAUSED display on the screen, save previous pixel values
    for (int i = 0; i < strlen(paused); i++) {
        previous_pix[i] = game.get_pix_at(5, i);
        game.write(5, i, Pixel(paused[i], BLACK, WHITE));
    }
    game.draw(0, false);
    fflush(stdout);
    
    while (wait_for_kb_input() != ESC);

    // put old pixels back
    for (int i = 0; i < strlen(paused); i++) {
        game.write(5, i, previous_pix[i]);
    }
    game.draw(0, false);
    fflush(stdout);
}

int main(){
    signal(SIGINT, sigint_handler);
    // in Linux only, we need to init the termios
    init_engine();
    clear_screen();
    show_cursor(false);
    //get a random seed
    srand(time(NULL));

    level_selected = main_menu();
    bool use_ghost = level_selected >= 0;
    level_selected = abs(level_selected);

    clear_screen();
    level = level_selected;

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
    int successful_move; // 0 if INVALID, 1 if VALID, 2 if TSPIN_MINI, 3 if TSPIN
    bool reset_piece = false;
    bool has_held = false;
    bool hard_dropping = false;
    bool game_over = false;
    bool made_move = false;
    uint frame_counter = 0;
    uint current_lines_cleared = 0;
    uint reset_count = 0;
    uint t_spin = 0;
    int lock_delay = lock_delay_reset;
    float gravity = (float) 1/60;
    char buffer[32];
    string prev_clear = "nothing";
    string* prev_clear_ptr = &prev_clear;
    Tetromino ghost_piece;

    // initialize the board to blank
    game.draw(0, 0);
    
    if (use_ghost) {
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

            case ESC:
                key = -1;
                // put the PAUSED display where score goes
                pause_game();
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
                    if (use_ghost) {
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

        if (use_ghost) {
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
            clear_score_output();
            t_spin = successful_move;
            score += calculate_score(current_lines_cleared, piece,
                                    stack, level, prev_clear_ptr, t_spin);
            line_total += current_lines_cleared;
            
            if (level < 20)
                level = line_total/10 + level_selected;
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

        game.draw(0, 0);

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

    game_over_screen();

    return 0;
}

