#ifndef MENU_HPP
#define MENU_HPP
#include <string>
#include <string.h>
#include <vector>
#include <variant>

#include "engine.hpp"

using namespace std;
using MenuValue = variant<bool*, char*, int*, string*>;

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
        MenuItem(string text, MENUTYPE type, MenuValue value_ptr = MenuValue{}, int min_number = -100, int max_number = 100)
            : item_text(move(text)), item_type(type), value(value_ptr), min_num(min_number), max_num(max_number) {}
    
        void incr_number() {
            if (item_type == NUMBER && holds_alternative<int*>(value)) {
                (*get<int*>(value))++;
                if (*get<int*>(value) > max_num) {
                    (*get<int*>(value))--;
                }
            }
        }
    
        void decr_number() {
            if (item_type == NUMBER && holds_alternative<int*>(value)) {
                (*get<int*>(value))--;
                if (*get<int*>(value) < min_num) {
                    (*get<int*>(value))++;
                }
            }
        }
    
        void toggle_bool() {
            if ((item_type == BOOLEAN || item_type == SELECT) && holds_alternative<bool*>(value)) {
                *get<bool*>(value) = !(*get<bool*>(value));
            }
        }

        void set_string(string new_string) {
            if (item_type == STRING && holds_alternative<string*>(value)) {
                *get<string*>(value) = new_string;
            }
        }

        void set_control(char c) {
            if (item_type == CONTROL && holds_alternative<char*>(value)) {
                *get<char*>(value) = c;
            }
        }
    
        string get_display_text() {
            char buffer[MAX_ITEM_SIZE];
            switch (item_type) {
                case NUMBER:
                    sprintf(buffer, "%s %9d", item_text.c_str(), *get<int*>(value));
                    break;
                case BOOLEAN:
                    sprintf(buffer, "%s %9s", item_text.c_str(), *get<bool*>(value) ? " ON" : "OFF");
                    break;
                case STRING:
                    sprintf(buffer, "%s %9s", item_text.c_str(), get<string*>(value)->c_str());
                    break;
                case CONTROL:
                    sprintf(buffer, "%s %9s", item_text.c_str(), keycode_to_string(*get<char*>(value)).c_str());
                    break;
                default:
                    sprintf(buffer, "%s", item_text.c_str());
                    break;
            }
            return buffer;
        }

        string get_item_text() const { return item_text; }
    
        MENUTYPE get_type() const { return item_type; }
    
    private:
        string item_text;
        MENUTYPE item_type;
        MenuValue value;
        int min_num, max_num;
};
    

class Menu {
    public:
        Menu(){};

        void add_item(MenuItem item) {
            items.push_back(item);
        }

        void display(const unsigned int offset);

        vector<MenuItem> items;
};

#endif