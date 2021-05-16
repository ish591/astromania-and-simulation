#include "menu_buttons.h"
using namespace std;
class Menu
{
private:
    int option = 0;
    int main_menu_counter = 0;
    int help_menu_counter = 0;
    int play_menu_counter = 0;
    int offline_menu_counter = 0;
    int online_menu_counter = 0;
    int window_width;
    int window_height;
    vector<menu_buttons> main_menu_buttons;
    vector<menu_buttons> help_menu_buttons;
    vector<menu_buttons> play_menu_buttons;
    vector<menu_buttons> offline_menu_buttons;
    vector<menu_buttons> online_menu_buttons;
    map<SDL_Keycode, char> digit_map;
    bool player_is_ready = false;
    vector<vector<SDL_Surface *>> player_surfaces;

public:
    Menu(int, int, vector<vector<SDL_Surface *>>);
    void initialise_fonts();
    void initialise_main_menu();
    void initialise_help_menu();
    void initialise_play_menu();
    void setOption(int);
    void initialise_offline_menu();
    void initialise_online_menu();
    void display(SDL_Renderer *, SDL_Surface *);
    void HandleEvents(SDL_Event);
    void event_main_menu(SDL_Event);
    void event_help_menu(SDL_Event);
    void event_play_menu(SDL_Event);
    void event_offline_menu(SDL_Event);
    void event_online_menu(SDL_Event);
    void event_buffer_menu(SDL_Event);
    void display_buffer_menu(SDL_Renderer *, SDL_Surface *);
    bool player_ready();
    bool ended();
    bool pressed = false;
    int players_selected;
    bool offline_selected = false;
    bool online_selected = false;
    int joined_players = 0; //the sole information needed for buffer menu
    string IP_address;
    vector<TTF_Font *> fonts;
};