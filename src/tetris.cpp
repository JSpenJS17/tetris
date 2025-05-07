#include "game.hpp"
#include "singleplayer.hpp"
#include "multiplayer.hpp"
#include <exception>

// externs
const int width = 10;
const int height = 20;
const int lock_delay_reset = 750;
Pixel bg = Pixel('-', LIGHT_GRAY, LIGHT_GRAY);
Board game = Board(width, height, bg);
Board other_game = game;
Board partner_game = Board(width, height, bg); // displayed to the right
long score = 0;
GameData gamedata = GameData();
int line_total = 0;
int level = 0;

void reset_everything() {
    color(16, 16);
    show_cursor(true);
    close_engine();
}

void main_menu() {
    // modifies global gamedata based on menu inputs
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

    Menu main_menu = Menu();
    bool single_player = false;
    bool multi_player = false;
    main_menu.add_item(MenuItem("-Singleplayer", SELECT, &single_player));
    main_menu.add_item(MenuItem("-Multiplayer ", SELECT, &multi_player));
    main_menu.display(11); // 11 lines down from top
    gamedata.gametype = single_player ? SINGLEPLAYER : MULTIPLAYER;
    
    if (gamedata.gametype == SINGLEPLAYER) {
        // singleplayer submenu
        Menu sp_menu = Menu();
        bool dummy = false;
        sp_menu.add_item(MenuItem("Starting Level:", NUMBER, &gamedata.starting_level, 0, 20));
        sp_menu.add_item(MenuItem("Ghost Piece    ", BOOLEAN, &gamedata.use_ghost));
        sp_menu.add_item(MenuItem("Start", SELECT, &dummy));
        sp_menu.display(11);

    } else {
        // multiplayer submenu 
    }
}

void sigint_handler(int dummy) {
    // act like it's a game over
    if (gamedata.gametype != MULTIPLAYER) {
        game_over_screen();
        reset_everything();
        exit(0);
    }
}

void custom_terminate() {
    std::cerr << "🔥 std::terminate called! No active exception." << std::endl;
    std::abort(); // still crashes, but now you know why
}

int main(){
    errno = 0;
    std::set_terminate(custom_terminate);
    signal(SIGINT, sigint_handler);
    init_engine();
    clear_screen();
    show_cursor(false);
    //get a random seed
    srand(time(NULL));

    main_menu();
    clear_screen();

    if (gamedata.gametype == SINGLEPLAYER) {
        singleplayer();
    } else {
        multiplayer();
    }

    reset_everything();
    clear_screen();

    return 0;
}

