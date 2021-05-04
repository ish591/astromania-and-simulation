#include "Menu.h"
Menu::Menu(int w, int h)
{
    option = 0;
    window_width = w;
    window_height = h;
    initialise_fonts();
    initialise_main_menu();
    initialise_help_menu();
    initialise_play_menu();
    initialise_offline_menu();
    initialise_online_menu();
    pressed = false;
    offline_selected = false;
    online_selected = false;
    digit_map = {{SDLK_0, '0'}, {SDLK_1, '1'}, {SDLK_2, '2'}, {SDLK_3, '3'}, {SDLK_4, '4'}, {SDLK_5, '5'}, {SDLK_6, '6'}, {SDLK_7, '7'}, {SDLK_8, '8'}, {SDLK_9, '9'}, {SDLK_PERIOD, '.'}};
}
void Menu::initialise_fonts()
{
    fonts.push_back(TTF_OpenFont("../assets/fonts/m5x7.ttf", 24));
    fonts.push_back(TTF_OpenFont("../assets/fonts/OpenSans-Regular.ttf", 24));
}
void Menu::initialise_main_menu()
{
    SDL_Rect rect1 = {(window_width - 100) / 2, 100, 100, 30};
    SDL_Rect rect2 = {(window_width - 100) / 2, 200, 100, 30};
    vector<int> rgba = {255, 255, 255, 255}; //default rectangle color
    string text1 = "Play";
    string text2 = "Help";
    SDL_Color font_color = {0, 0, 200};
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    main_menu_buttons.push_back(button1);
    main_menu_buttons.push_back(button2);
}
void Menu::initialise_help_menu()
{
    SDL_Rect rect1 = {(window_width - 1000) / 2, 100, 1000, 100};
    SDL_Rect rect2 = {(window_width - 100) / 2, 400, 100, 30};
    vector<int> rgba = {255, 255, 255, 255}; //default rectangle color
    string text1 = "Instructions : Use arrow keys to navigate, press enter to throw bombs.";
    string text2 = "Back";
    SDL_Color font_color = {0, 0, 200};
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[1], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    help_menu_buttons.push_back(button1);
    help_menu_buttons.push_back(button2);
}
void Menu::initialise_play_menu()
{
    SDL_Rect rect1 = {(window_width - 100) / 2, 100, 100, 30};
    SDL_Rect rect2 = {(window_width - 100) / 2, 200, 100, 30};
    SDL_Rect rect3 = {(window_width - 100) / 2, 300, 100, 30};
    vector<int> rgba = {255, 255, 255, 255}; //default rectangle color
    string text1 = "Offline mode";
    string text2 = "Online mode";
    string text3 = "Back";
    SDL_Color font_color = {0, 0, 200};
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    play_menu_buttons.push_back(button1);
    play_menu_buttons.push_back(button2);
    play_menu_buttons.push_back(button3);
}
void Menu::initialise_offline_menu()
{
    //no. of players
    SDL_Rect rect1 = {(window_width - 100) / 2, 100, 100, 30};
    SDL_Rect rect2 = {(window_width - 100) / 2, 200, 100, 30};
    SDL_Rect rect3 = {(window_width - 100) / 2, 300, 100, 30};
    SDL_Rect rect4 = {(window_width - 100) / 2, 400, 100, 30};
    vector<int> rgba = {255, 255, 255, 255}; //default rectangle color
    string text1 = "2 Players";
    string text2 = "3 Players";
    string text3 = "4 Players";
    string text4 = "Back";
    SDL_Color font_color = {0, 0, 200};
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    menu_buttons button4 = menu_buttons(rect4, font_color, text4, fonts[0], rgba);
    offline_menu_buttons.push_back(button1);
    offline_menu_buttons.push_back(button2);
    offline_menu_buttons.push_back(button3);
    offline_menu_buttons.push_back(button4);
}
void Menu::initialise_online_menu()
{
    SDL_Rect rect1 = {(window_width - 400) / 2, 100, 400, 60};
    SDL_Rect rect2 = {(window_width - 400) / 2, 200, 400, 60};
    SDL_Rect rect3 = {(window_width - 400) / 2, 300, 400, 60};
    SDL_Rect rect4 = {(window_width - 400) / 2, 400, 400, 60};
    vector<int> rgba = {255, 255, 255, 255}; //default rectangle color
    vector<int> ipbox = {100, 255, 255, 255};
    vector<int> instruction = {0, 0, 0, 255}; //for enter ip instruction
    string text1 = "Enter IP address in the box below :";
    string text2 = "Enter here";
    string text3 = "Start";
    string text4 = "Back";
    SDL_Color font_color = {0, 0, 200};
    SDL_Color prompt_color = {200, 0, 0};
    menu_buttons button1 = menu_buttons(rect1, prompt_color, text1, fonts[0], instruction);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], ipbox);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    menu_buttons button4 = menu_buttons(rect4, font_color, text4, fonts[0], rgba);
    online_menu_buttons.push_back(button1);
    online_menu_buttons.push_back(button2);
    online_menu_buttons.push_back(button3);
    online_menu_buttons.push_back(button4);
}
void Menu::display(SDL_Renderer *renderer, SDL_Surface *surface)
{
    vector<menu_buttons> display_current;
    int counter_current;
    switch (option)
    {
    case 0:
        display_current = main_menu_buttons;
        counter_current = main_menu_counter;
        break;
    case 1:
        display_current = help_menu_buttons;
        counter_current = help_menu_counter;
        break;
    case 2:
        display_current = play_menu_buttons;
        counter_current = play_menu_counter;
        break;
    case 3:
        display_current = offline_menu_buttons;
        counter_current = offline_menu_counter;
        break;
    case 4:
        display_current = online_menu_buttons;
        counter_current = online_menu_counter;
        break;
    default:
        display_current = main_menu_buttons;
        counter_current = main_menu_counter;
        break;
    }
    for (int i = 0; i < display_current.size(); i++)
    {
        vector<int> rgba;
        if (i == counter_current - 1)
        {
            rgba = {200, 180, 100, 255};
        }
        else
        {
            rgba = display_current[i].rect_color;
        }
        SDL_Rect curr_rect = display_current[i].rect;
        SDL_Color curr_color = display_current[i].text_color;
        string curr_text = display_current[i].text;
        TTF_Font *curr_font = display_current[i].font;
        SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
        SDL_RenderDrawRect(renderer, &curr_rect);
        surface = TTF_RenderText_Solid(curr_font, curr_text.c_str(), curr_color);
        SDL_Texture *display_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderFillRect(renderer, &curr_rect);
        if (!display_texture)
        {
            // cout << "Failed to create texture" << endl;
        }
        else
        {
            SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
        }
        SDL_DestroyTexture(display_texture);
    }
    SDL_RenderPresent(renderer);
}
void Menu::HandleEvents(SDL_Event e)
{
    //an event can cause : 1.) option to change, 2.) highlighted button to change its color, basically maintain a current var for each screen
    //which can take values upto number of buttons on it
    switch (option)
    {
    case 0:
        event_main_menu(e);
        break;
    case 1:
        event_help_menu(e);
        break;
    case 2:
        event_play_menu(e);
        break;
    case 3:
        event_offline_menu(e);
        break;
    case 4:
        event_online_menu(e);
        break;
    default:
        event_main_menu(e);
        break;
    }
}
void Menu::event_main_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_KEYDOWN):
    {
        if (main_menu_counter == 0)
        {
            pressed = true;
            main_menu_counter = 1;
            //button 1 is highlighted
        }
        else
        {
            if (!pressed)
            {
                pressed = true;
                SDL_Keycode key_press = e.key.keysym.sym;
                if (key_press == SDLK_DOWN)
                {
                    main_menu_counter = (main_menu_counter) % 2 + 1;
                }
                else if (key_press == SDLK_UP)
                {
                    main_menu_counter = (main_menu_counter) % 2 + 1;
                }
                else if (key_press == SDLK_RETURN)
                {
                    if (main_menu_counter == 1)
                    {
                        option = 2;
                    }
                    else
                    {
                        option = 1;
                    }
                }
            }
        }
        break;
    }
    case (SDL_KEYUP):
        pressed = false;
        break;
    }
}
void Menu::event_play_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_KEYDOWN):
    {
        if (play_menu_counter == 0)
        {
            pressed = true;
            play_menu_counter = 1;
            //button 1 is highlighted
        }
        else
        {
            if (!pressed)
            {
                pressed = true;
                SDL_Keycode key_press = e.key.keysym.sym;
                if (key_press == SDLK_DOWN)
                {
                    play_menu_counter = (play_menu_counter) % 3 + 1;
                }
                else if (key_press == SDLK_UP)
                {
                    play_menu_counter = (play_menu_counter - 1) + 3 * (play_menu_counter == 1);
                }
                else if (key_press == SDLK_RETURN)
                {
                    if (play_menu_counter == 1)
                    {
                        option = 3;
                    }
                    else if (play_menu_counter == 2)
                    {
                        option = 4;
                    }
                    else
                    {
                        option = 0;
                        play_menu_counter = 0;
                        main_menu_counter = 0;
                    }
                }
            }
        }
        break;
    }
    case (SDL_KEYUP):
        pressed = false;
        break;
    }
}
void Menu::event_help_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_KEYDOWN):
    {
        if (help_menu_counter == 0)
        {
            pressed = true;
            help_menu_counter = 1;
            //button 2 is highlighted
        }
        else
        {
            if (!pressed)
            {
                pressed = true;
                SDL_Keycode key_press = e.key.keysym.sym;
                if (key_press == SDLK_RETURN)
                {
                    option = 0;
                    help_menu_counter = 0;
                    main_menu_counter = 0;
                }
            }
        }
        break;
    }
    case (SDL_KEYUP):
        pressed = false;
        break;
    }
}
void Menu::event_offline_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_KEYDOWN):
    {
        if (offline_menu_counter == 0)
        {
            pressed = true;
            offline_menu_counter = 1;
            //button 1 is highlighted
        }
        else
        {
            if (!pressed)
            {
                pressed = true;
                SDL_Keycode key_press = e.key.keysym.sym;
                if (key_press == SDLK_DOWN)
                {
                    offline_menu_counter = (offline_menu_counter) % 4 + 1;
                }
                else if (key_press == SDLK_UP)
                {
                    offline_menu_counter = (offline_menu_counter - 1) + 4 * (offline_menu_counter == 1);
                }
                else if (key_press == SDLK_RETURN)
                {
                    if (offline_menu_counter == 1)
                    {
                        //option = 3;
                        offline_selected = true;
                        players_selected = 2;
                    }
                    else if (offline_menu_counter == 2)
                    {
                        offline_selected = true;
                        players_selected = 3;
                        //option = 4;
                    }
                    else if (offline_menu_counter == 3)
                    {
                        offline_selected = true;
                        players_selected = 4;
                    }
                    else
                    {
                        option = 2;
                        offline_menu_counter = 0;
                        play_menu_counter = 0;
                    }
                }
            }
        }
        break;
    }
    case (SDL_KEYUP):
    {
        pressed = false;
        break;
    }
    }
}
void Menu::event_online_menu(SDL_Event e)
{
    //first rectangle will indicate enter IP, this cannot be toggled through. Second will accept user IP. Third will be start
    //fourth will be back button
    switch (e.type)
    {
    case (SDL_KEYDOWN):
    {
        if (online_menu_counter == 0)
        {
            pressed = true;
            online_menu_counter = 2;
            //text box is highlighted (box number 2)
        }
        else
        {
            if (!pressed)
            {
                pressed = true;
                SDL_Keycode key_press = e.key.keysym.sym;
                if (key_press == SDLK_DOWN)
                {
                    online_menu_counter = (online_menu_counter + 1) - 3 * (online_menu_counter == 4);
                    //since we are effectively toggling in last three boxes (2,3,4)
                }
                else if (key_press == SDLK_UP)
                {
                    online_menu_counter = (online_menu_counter - 1) + 3 * (online_menu_counter == 2);
                }
                else if (key_press == SDLK_RETURN)
                {
                    if (online_menu_counter == 3)
                    {
                        //provide an IP and start!
                        online_selected = true;
                        IP_address = online_menu_buttons[1].text;
                    }
                    else if (online_menu_counter == 4)
                    {
                        option = 2;
                        online_menu_counter = 0;
                        play_menu_counter = 0;
                    }
                }
                else
                {
                    //handle all key presses if online_menu_counter is 2
                    if (online_menu_counter == 2)
                    {
                        string temp = online_menu_buttons[1].text;
                        if (temp.length() <= 20)
                        { //max allowed
                            //not allowing space bars
                            //only allowing numbers and decimal
                            if (digit_map.find(key_press) != digit_map.end())
                            {
                                if (temp == "Enter here")
                                {
                                    temp = "";
                                }
                                temp += digit_map[key_press];
                            }
                        }
                        if (key_press == SDLK_BACKSPACE && temp.length() != 0)
                        {
                            if (temp == "Enter here")
                            {
                                temp = "";
                            }
                            else
                            {
                                temp.erase(temp.length() - 1);
                            }
                        }
                        online_menu_buttons[1].text = temp;
                    }
                }
            }
        }
        break;
    }
    case (SDL_KEYUP):
    {
        pressed = false;
        break;
    }
    }
}
bool Menu::ended() { return offline_selected || online_selected; }
// void Menu::display_online_menu(SDL_Renderer *renderer, SDL_Surface *surface)
// {
//     //recieve IP address as input, back button
// }