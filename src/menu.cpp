#include "menu.hpp"
using namespace std;

MenuItem::MenuItem(string item_text, MENUTYPE type) : item_text(item_text), item_type(type) {
    number = 0;
    text = "";
    control_key = '\0';
}

string MenuItem::get_display_text() {
    char buffer[MAX_ITEM_SIZE];
    switch (item_type) {
        case SELECT:
            sprintf(buffer, "%s", item_text.c_str());
            break;
        case NUMBER:
            sprintf(buffer, "%s %4d", item_text.c_str(), number);
            break;
        case STRING:
            sprintf(buffer, "%s %s", item_text.c_str(), text.c_str());
            break;
        case CONTROL:
            sprintf(buffer, "%s %4s", item_text.c_str(), keycode_to_string(control_key).c_str());
            break;
        case BOOLEAN:
            sprintf(buffer, "%s %4s", item_text.c_str(), selected? " ON" : "OFF");
    }
    return buffer;
}

string MenuItem::get_item_text() {
    return item_text;
}

MENUTYPE MenuItem::get_type() {
    return item_type;
}

void MenuItem::incr_number() {
    number = number + 1;
    if (number > 100) {
        number = 100;
    }
}

void MenuItem::decr_number() {
    number = number - 1;
    if (number < -100) {
        number = -100;
    }
}

void MenuItem::set_string(string new_string) {
    text = new_string;
}

void MenuItem::set_control(char c) {
    control_key = c;
}

void MenuItem::set_selected() {
    selected = !selected;
}

char MenuItem::get_control_key() {
    return control_key;
}

int MenuItem::get_number() {
    return number;
}

string MenuItem::get_string() {
    return text;
}

bool MenuItem::get_selected() {
    return selected;
}

void set_selected() {

}
Menu::Menu(){}

void Menu::add_option(MenuItem option) {
    items.push_back(option);
}

void Menu::display(const unsigned int offset) {
    if (items.size() == 0) {
        return;
    }
    show_cursor(true);
    color(16, 16);
    char key = -1;
    int menu_idx = 0;

    // display initial menu
    for (auto & option : items) {
        set_cursor_pos(offset + menu_idx, 0);
        cout << option.get_display_text() << endl;
        menu_idx++;
    }

    menu_idx = 0;

    while (true) {
        key = -1;
        while (key != ENTER && key != SPACE) {
            set_cursor_pos(offset + menu_idx, 0);
            fflush(stdout);
            key = wait_for_kb_input();
            MenuItem *cur_item = &items.at(menu_idx);
    
            switch (key) {
                case UP:
                    set_cursor_pos(offset + menu_idx, 0);
                    menu_idx--;
                    break;
                case DOWN:
                    set_cursor_pos(offset + menu_idx, 0);
                    menu_idx++;
                    break;
    
                /* THIS IS WHERE WE RETURN FROM THE FUNCTION */
                case ESC:
                    // I know this is terrible but I'm too deep
                    return;
            }
    
            if (menu_idx < 0) {
                menu_idx = items.size() - 1;
            }
            else if (menu_idx >= items.size()) {
                menu_idx = 0;
            }
    
            cur_item = &items.at(menu_idx);
            set_cursor_pos(offset + menu_idx, 0);
            cout << cur_item->get_display_text();
        }
        color(16, 16);
    
        // they've selected the option they want to input to
        MenuItem *selection = &items.at(menu_idx);
    
        // second phase, let them edit the value
        set_cursor_pos(offset + menu_idx, 0);
        cout << selection->get_display_text() << endl;
        key = -1;
        size_t item_text_size = selection->get_item_text().size() + 1; // + 1 for a little buffer from item text

        if (selection->get_type() == NUMBER) {
            while (key != ENTER) {
                set_cursor_pos(offset + menu_idx, selection->get_display_text().size()-1);
                fflush(stdout);
                key = wait_for_kb_input();
                switch (key) {
                    case UP:
                        selection->incr_number();
                        break;
        
                    case DOWN:
                        selection->decr_number();
                        break;
                    
                    case LEFT:
                        for (int i = 0; i < 5; i++) {
                            selection->decr_number();
                        }
                        break;
        
                    case RIGHT:
                        for (int i = 0; i < 5; i++) {
                            selection->incr_number();
                        }
                        break;
                }
        
                set_cursor_pos(offset + menu_idx, 0);
                cout << selection->get_display_text() << endl;
                fflush(stdout);
            }
    
        } else if (selection->get_type() == STRING) {
            char buffer[MAX_STRING_SIZE];
            unsigned int string_idx = 0;
            
            set_cursor_pos(offset + menu_idx, item_text_size);
            fflush(stdout);

            while (key != ENTER && key != ESC) {
                key = wait_for_kb_input();

                if (is_alpha(key)) {
                    if (string_idx < MAX_STRING_SIZE - 1)
                    {
                        buffer[string_idx] = key;
                        buffer[string_idx + 1] = '\0';
                        string_idx += 1;
                    }
                } else if (key == BACKSPACE) {
                    if (string_idx > 0) {
                        string_idx -= 1;
                        buffer[string_idx] = '\0';
                        set_cursor_pos(offset + menu_idx, item_text_size + strlen(buffer));
                        cout << " ";
                        set_cursor_pos(offset + menu_idx, item_text_size);
                    }
                }

                set_cursor_pos(offset + menu_idx, item_text_size);
                cout << buffer;
                fflush(stdout);
            }
            selection->set_string(buffer);
    
        } else if (selection->get_type() == CONTROL) {
            set_cursor_pos(offset + menu_idx, item_text_size);
            cout << "         ";
            set_cursor_pos(offset + items.size() + 1, 0);
            cout << "Press a key or hit ESC to cancel";
            fflush(stdout);
            
            key = wait_for_kb_input();
    
            if (key != ESC) {
                selection->set_control(key);
            }

            set_cursor_pos(offset + items.size() + 1, 0);
            cout << "                                ";

            set_cursor_pos(offset + menu_idx, 0);
            cout << selection->get_display_text();
            fflush(stdout);
        
        } else if (selection->get_type() == SELECT) {
            selection->set_selected();
            return; // special case for SELECT, should just return out

        } else if (selection->get_type() == BOOLEAN) {
            selection->set_selected();
            set_cursor_pos(offset + menu_idx, 0);
            cout << selection->get_display_text();
            fflush(stdout);
        }
    }
}
