#ifndef MENU_HPP
#define MENU_HPP
#include <string>
#include <string.h>
#include <vector>
#include "engine.hpp"

#define MAX_ITEM_SIZE 64
#define MAX_STRING_SIZE 32

typedef enum MENUTYPE {
    SELECT,
    NUMBER,
    STRING,
    CONTROL,
    BOOLEAN,
    NUM_MENUTYPES,
} MENUTYPE;

class MenuItem {
    public:
        MenuItem(string text, MENUTYPE type);
        string get_display_text();
        string get_item_text();
        MENUTYPE get_type();

        string get_string();
        int get_number();
        char get_control_key();
        bool get_selected();
        
        void incr_number();
        void decr_number();
        void set_string(string new_string);
        void set_control(char c);
        void set_selected();

    private:
        string item_text;
        MENUTYPE item_type;
    
        string text;
        int number;
        char control_key;
        bool selected;
};

class Menu {
    public:
        Menu();

        void add_item(MenuItem option);

        void display(const unsigned int offset);

        vector<MenuItem> items;
};

#endif