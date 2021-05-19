#include "Menu.h"
Menu::Menu(int w, int h, vector<vector<SDL_Surface *>> player_surfaces, SDL_Surface *main_screen, SDL_Surface *background, vector<SDL_Surface *> &mute_unmute, SDL_Surface *back_button, vector<SDL_Surface *> &control_buttons, vector<SDL_Surface *> &block_surfaces)
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
    initialise_buffer_menu();
    Menu::player_surfaces = player_surfaces;
    Menu::main_screen = main_screen;
    Menu::background = background;
    Menu::mute_unmute = mute_unmute;
    Menu::back_button = back_button;
    Menu::control_buttons = control_buttons;
    Menu::block_surfaces = block_surfaces;
    pressed = false;
    offline_selected = false;
    online_selected = false;
    player_is_ready = false;
    digit_map = {{SDLK_l, 'l'}, {SDLK_o, 'o'}, {SDLK_c, 'c'}, {SDLK_a, 'a'}, {SDLK_m, 'm'}, {SDLK_n, 'n'}, {SDLK_i, 'i'}, {SDLK_s, 's'}, {SDLK_h, 'h'}, {SDLK_0, '0'}, {SDLK_1, '1'}, {SDLK_2, '2'}, {SDLK_3, '3'}, {SDLK_4, '4'}, {SDLK_5, '5'}, {SDLK_6, '6'}, {SDLK_7, '7'}, {SDLK_8, '8'}, {SDLK_9, '9'}, {SDLK_PERIOD, '.'}};
}
void Menu::initialise_fonts()
{
    fonts.push_back(TTF_OpenFont("../assets/fonts/m5x7.ttf", 60));
    fonts.push_back(TTF_OpenFont("../assets/fonts/OpenSans-Regular.ttf", 24));
    fonts.push_back(TTF_OpenFont("../assets/fonts/m5x7.ttf", 35));
}
void Menu::initialise_main_menu()
{
    SDL_Rect rect0 = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720};
    SDL_Rect rect1 = {(window_width * 360) / 1280, (window_height * 74) / 100, (window_width * 200) / 1280, (window_height * 60 / 720)};
    SDL_Rect rect2 = {(window_width * 690) / 1280, (window_height * 74) / 100, (window_width * 200) / 1280, (window_height * 60 / 720)};
    vector<int> rgba = {0, 0, 0, 255}; //default rectangle color
    string text0 = "";
    string text1 = "Play";
    string text2 = "Help";
    SDL_Color font_color = {255, 255, 255};
    menu_buttons button0 = menu_buttons(rect0, font_color, text0, fonts[0], rgba);
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    main_menu_buttons.push_back(button0);
    main_menu_buttons.push_back(button1);
    main_menu_buttons.push_back(button2);
}
void Menu::initialise_help_menu()
{
    SDL_Rect rect0 = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720}; //mute button
    SDL_Rect rect1 = {(window_width * 305) / 1280, (window_height * 70) / 720, (window_width * 300) / 1280, (window_height * 60) / 720};
    SDL_Rect rect2 = {(window_width * 675) / 1280, (window_height * 70) / 720, (window_width * 300) / 1280, (window_height * 60) / 720};
    SDL_Rect rect3 = {(window_width * 30) / 1280, (window_height * 10) / 100, (window_width * 70) / 1280, (window_height * 50 / 720)};
    //back button
    vector<int> rgba = {0, 0, 0, 255}; //default rectangle color
    string text0 = "";
    string text1 = "Offline Help";
    string text2 = "Online Help";
    string text3 = "Back";
    SDL_Color font_color = {255, 255, 255};
    menu_buttons button0 = menu_buttons(rect0, font_color, text0, fonts[0], rgba);
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    help_menu_buttons.push_back(button0);
    help_menu_buttons.push_back(button3);
    help_menu_buttons.push_back(button1);
    help_menu_buttons.push_back(button2);
}
void Menu::initialise_play_menu()
{
    SDL_Rect rect0 = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720};
    SDL_Rect rect1 = {(window_width * 335) / 1280, (window_height * 74) / 100, (window_width * 270) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect2 = {(window_width * 675) / 1280, (window_height * 74) / 100, (window_width * 270) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect3 = {(window_width * 30) / 1280, (window_height * 10) / 100, (window_width * 70) / 1280, (window_height * 50 / 720)};
    vector<int> rgba = {0, 0, 0, 255}; //default rectangle color
    string text0 = "";
    string text1 = "Offline mode";
    string text2 = "Online mode";
    string text3 = "Back";
    SDL_Color font_color = {255, 255, 255};
    menu_buttons button0 = menu_buttons(rect0, font_color, text0, fonts[0], rgba);
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    play_menu_buttons.push_back(button0);
    play_menu_buttons.push_back(button3);
    play_menu_buttons.push_back(button1);
    play_menu_buttons.push_back(button2);
}
void Menu::initialise_offline_menu()
{
    //no. of players
    SDL_Rect rect0 = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720};
    SDL_Rect rect1 = {(window_width * 280) / 1280, (window_height * 74) / 100, (window_width * 200) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect2 = {(window_width * 540) / 1280, (window_height * 74) / 100, (window_width * 200) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect3 = {(window_width * 800) / 1280, (window_height * 74) / 100, (window_width * 200) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect4 = {(window_width * 30) / 1280, (window_height * 10) / 100, (window_width * 70) / 1280, (window_height * 50 / 720)};
    vector<int> rgba = {0, 0, 0, 255}; //default rectangle color
    string text0 = "";
    string text1 = "2 Players";
    string text2 = "3 Players";
    string text3 = "4 Players";
    string text4 = "Back";
    SDL_Color font_color = {255, 255, 255};
    menu_buttons button0 = menu_buttons(rect0, font_color, text0, fonts[0], rgba);
    menu_buttons button1 = menu_buttons(rect1, font_color, text1, fonts[0], rgba);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], rgba);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    menu_buttons button4 = menu_buttons(rect4, font_color, text4, fonts[0], rgba);
    offline_menu_buttons.push_back(button0);
    offline_menu_buttons.push_back(button4); //back button
    offline_menu_buttons.push_back(button1);
    offline_menu_buttons.push_back(button2);
    offline_menu_buttons.push_back(button3);
}
void Menu::initialise_online_menu()
{
    SDL_Rect rect0 = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720};
    SDL_Rect rect1 = {(window_width * 505) / 1280, (window_height * 80) / 720, (window_width * 270) / 1280, (window_height * 70 / 720)};
    SDL_Rect rect2 = {(window_width * 475) / 1280, (window_height * 230) / 720, (window_width * 330) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect3 = {(window_width * 540) / 1280, (window_height * 480) / 720, (window_width * 200) / 1280, (window_height * 50 / 720)};
    SDL_Rect rect4 = {(window_width * 30) / 1280, (window_height * 10) / 100, (window_width * 70) / 1280, (window_height * 50 / 720)};
    vector<int> rgba = {0, 0, 0, 255};        //default rectangle color
    vector<int> ipbox = {0, 0, 0, 255};       //ipbox will be variable
    vector<int> instruction = {0, 0, 0, 255}; //for enter ip instruction
    string text0 = "";
    string text1 = "Enter IP address in the box below :";
    string text2 = "";
    string text3 = "Start";
    string text4 = "Back";
    SDL_Color font_color = {255, 255, 255};
    SDL_Color prompt_color = {200, 0, 0};
    menu_buttons button0 = menu_buttons(rect0, prompt_color, text1, fonts[0], instruction);
    menu_buttons button1 = menu_buttons(rect1, prompt_color, text1, fonts[0], instruction);
    menu_buttons button2 = menu_buttons(rect2, font_color, text2, fonts[0], ipbox);
    menu_buttons button3 = menu_buttons(rect3, font_color, text3, fonts[0], rgba);
    menu_buttons button4 = menu_buttons(rect4, font_color, text4, fonts[0], rgba);
    online_menu_buttons.push_back(button0);
    online_menu_buttons.push_back(button4);
    online_menu_buttons.push_back(button1);
    online_menu_buttons.push_back(button2);
    online_menu_buttons.push_back(button3);
}
void Menu::initialise_buffer_menu()
{
    vector<int> instruction = {0, 0, 0, 255};
    SDL_Color prompt_color = {255, 255, 255};
    SDL_Rect rect0 = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720};
    SDL_Rect rect1 = {(window_width * 505) / 1280, (window_height * 450) / 720, (window_width * 270) / 1280, (window_height * 70 / 720)};
    string text0 = "";
    string text1 = "Ready";
    menu_buttons button0 = menu_buttons(rect0, prompt_color, text0, fonts[0], instruction);
    menu_buttons button1 = menu_buttons(rect1, prompt_color, text1, fonts[0], instruction);
    buffer_menu_buttons.push_back(button0);
    buffer_menu_buttons.push_back(button1);
}
void Menu::setOption(int option)
{
    Menu::option = option;
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
    case 5:
        display_current = buffer_menu_buttons;
        counter_current = buffer_menu_counter;
        break;
    }
    if (option == 1 || option == 4 || option == 5)
    {
        surface = background;
    }
    else
    {
        surface = main_screen;
    }
    SDL_Rect curr_rect = {0, 0, window_width, window_height};
    //SDL_RenderDrawRect(renderer, &curr_rect);
    SDL_Texture *display_texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_RenderFillRect(renderer, &curr_rect);
    if (!display_texture)
    {
        // cout << "Failed to create texture" << endl;
    }
    else
    {
        SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
    }
    SDL_DestroyTexture(display_texture);

    curr_rect = {window_width - (window_width * 100) / 1280, (window_height * 8) / 10, (window_width * 70) / 1280, (window_height * 50) / 720};
    //SDL_RenderDrawRect(renderer, &curr_rect);
    if (mute_state == 0)
    {
        surface = mute_unmute[1];
    }
    else
    {
        surface = mute_unmute[0];
    }
    display_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!display_texture)
    {
        // cout << "Failed to create texture" << endl;
        SDL_RenderFillRect(renderer, &curr_rect);
    }
    else
    {
        SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
    }
    SDL_DestroyTexture(display_texture);
    if (counter_current == 1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &curr_rect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &curr_rect);
    }
    if (option == 5)
    {
        display_buffer_menu(renderer, surface);
    }
    for (int i = 1; i < display_current.size(); i++)
    {
        vector<int> rgba;
        rgba = display_current[i].rect_color;
        SDL_Color curr_color = display_current[i].text_color;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (i == counter_current - 1 || (option == 1 && (i - 2 == ins_coloured_state)))
        {
            curr_color = {0, 255, 255};
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        }
        SDL_Rect curr_rect_outer = display_current[i].rect;
        SDL_Rect curr_rect_inner = display_current[i].rect;
        string curr_text = display_current[i].text;
        if (curr_text.size() != 0)
        {
            if (curr_text == "Back")
            {
                surface = back_button;
                display_texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!display_texture)
                {
                    // cout << "Failed to create texture" << endl;
                    SDL_RenderFillRect(renderer, &curr_rect_outer);
                }
                else
                {
                    SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect_outer);
                }
                SDL_DestroyTexture(display_texture);
                if (counter_current == i + 1)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &curr_rect_outer);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &curr_rect_outer);
                }
            }
            else
            {
                TTF_Font *curr_font = display_current[i].font;
                surface = TTF_RenderText_Solid(curr_font, curr_text.c_str(), curr_color);
                curr_rect_inner.w = surface->w;
                curr_rect_inner.x = (curr_rect_outer.w - curr_rect_inner.w) / 2 + curr_rect_inner.x;
                curr_rect_inner.h = surface->h;
                curr_rect_inner.y = (curr_rect_outer.h - curr_rect_inner.h) / 2 + curr_rect_inner.y;
                if (rgba[0] == rgba[1] && rgba[1] == rgba[2] && rgba[1] == 0 && !(i == 2 && option == 4))
                {
                    SDL_RenderDrawRect(renderer, &curr_rect_outer);
                }
                display_texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!display_texture)
                {
                    // cout << "Failed to create texture" << endl;
                }
                else
                {
                    SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect_inner);
                }
                SDL_DestroyTexture(display_texture);
            }
        }
        else
        {
            SDL_RenderDrawRect(renderer, &curr_rect_outer);
        }
    }
    if (option == 1)
    {
        //offline instructions
        SDL_Color curr_color = {255, 0, 0};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect curr_rect_outer = {(window_width * 230) / 1280, (window_height * 120) / 720, (window_width * 250) / 1280, (window_height * 90) / 720};
        SDL_Rect curr_rect_inner;
        string curr_text = "Movement/bomb release";
        TTF_Font *curr_font = fonts[0];
        surface = TTF_RenderText_Solid(curr_font, curr_text.c_str(), curr_color);
        curr_rect_inner.w = surface->w;
        curr_rect_inner.x = (curr_rect_outer.w - curr_rect_inner.w) / 2 + curr_rect_outer.x;
        curr_rect_inner.h = surface->h;
        curr_rect_inner.y = (curr_rect_outer.h - curr_rect_inner.h) / 2 + curr_rect_outer.y;
        display_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!display_texture)
        {
            // cout << "Failed to create texture" << endl;
            SDL_RenderFillRect(renderer, &curr_rect_outer);
        }
        else
        {
            SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect_inner);
        }
        SDL_DestroyTexture(display_texture);
        curr_rect_outer = {(window_width * 760) / 1280, (window_height * 120) / 720, (window_width * 450) / 1280, (window_height * 90) / 720};
        curr_text = "Power-ups";
        surface = TTF_RenderText_Solid(curr_font, curr_text.c_str(), curr_color);
        curr_rect_inner.w = surface->w;
        curr_rect_inner.x = (curr_rect_outer.w - curr_rect_inner.w) / 2 + curr_rect_outer.x;
        curr_rect_inner.h = surface->h;
        curr_rect_inner.y = (curr_rect_outer.h - curr_rect_inner.h) / 2 + curr_rect_outer.y;
        display_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!display_texture)
        {
            // cout << "Failed to create texture" << endl;
            SDL_RenderFillRect(renderer, &curr_rect_outer);
        }
        else
        {
            SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect_inner);
        }
        SDL_DestroyTexture(display_texture);
        if (ins_state == 1)
        {
            SDL_Rect curr_rect = {(window_width * 150) / 1280, (window_height * 410) / 720, (window_width * 80) / 1280, (window_height * 60) / 720};
            //SDL_RenderDrawRect(renderer, &curr_rect);
            surface = player_surfaces[0][1];
            display_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!display_texture)
            {
                // cout << "Failed to create texture" << endl;
                SDL_RenderFillRect(renderer, &curr_rect);
            }
            else
            {
                SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
            }
            SDL_DestroyTexture(display_texture);
            curr_rect = {(window_width * 300) / 1280, (window_height * 390) / 720, (window_width * 240) / 1280, (window_height * 90) / 720};
            //SDL_RenderDrawRect(renderer, &curr_rect);
            surface = control_buttons[4];
            display_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!display_texture)
            {
                // cout << "Failed to create texture" << endl;
                SDL_RenderFillRect(renderer, &curr_rect);
            }
            else
            {
                SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
            }
            SDL_DestroyTexture(display_texture);
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                SDL_Rect curr_rect = {(window_width * 150) / 1280, (window_height * 220) / 720 + (i * window_height * 130) / 720, (window_width * 80) / 1280, (window_height * 60) / 720};
                //SDL_RenderDrawRect(renderer, &curr_rect);
                surface = player_surfaces[i][1];
                display_texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!display_texture)
                {
                    // cout << "Failed to create texture" << endl;
                    SDL_RenderFillRect(renderer, &curr_rect);
                }
                else
                {
                    SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
                }
                SDL_DestroyTexture(display_texture);
            }
            for (int i = 0; i < 4; i++)
            {
                SDL_Rect curr_rect = {(window_width * 300) / 1280, (window_height * 200) / 720 + (i * window_height * 130) / 720, (window_width * 200) / 1280, (window_height * 90) / 720};
                if (i == 1)
                {
                    curr_rect.w = (window_width * 260) / 1280;
                }
                //SDL_RenderDrawRect(renderer, &curr_rect);
                surface = control_buttons[i];
                display_texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!display_texture)
                {
                    // cout << "Failed to create texture" << endl;
                    SDL_RenderFillRect(renderer, &curr_rect);
                }
                else
                {
                    SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
                }
                SDL_DestroyTexture(display_texture);
            }
        }
        for (int i = 0; i < 4; i++)
        {
            SDL_Rect curr_rect = {(window_width * 720) / 1280, (window_height * 220) / 720 + (i * window_height * 130) / 720, (window_width * 80) / 1280, (window_height * 60) / 720};
            //SDL_RenderDrawRect(renderer, &curr_rect);
            surface = block_surfaces[i + 3];
            display_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!display_texture)
            {
                // cout << "Failed to create texture" << endl;
                SDL_RenderFillRect(renderer, &curr_rect);
            }
            else
            {
                SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
            }
            SDL_DestroyTexture(display_texture);
        }
        curr_color = {255, 255, 255};
        for (int i = 0; i < 4; i++)
        {
            curr_rect_outer = {(window_width * 860) / 1280, (window_height * 220) / 720 + (i * window_height * 130) / 720, (window_width * 100) / 1280, (window_height * 60) / 720};
            if (i == 0)
            {
                curr_text = "Throw bombs";
            }
            else if (i == 1)
            {
                curr_text = "Increases movement speed";
            }
            else if (i == 2)
            {
                curr_text = "Increase explosion radius";
            }
            else
            {
                curr_text = "Drop more bombs";
            }
            curr_font = fonts[2];
            surface = TTF_RenderText_Solid(curr_font, curr_text.c_str(), curr_color);
            curr_rect_inner.w = surface->w;
            curr_rect_inner.x = curr_rect_outer.x;
            curr_rect_inner.h = surface->h;
            curr_rect_inner.y = (curr_rect_outer.h - curr_rect_inner.h) / 2 + curr_rect_outer.y;
            display_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!display_texture)
            {
                // cout << "Failed to create texture" << endl;
                SDL_RenderFillRect(renderer, &curr_rect_outer);
            }
            else
            {
                SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect_inner);
            }
            SDL_DestroyTexture(display_texture);
        }
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
    case 5:
        event_buffer_menu(e);
        break;
    }
}
void Menu::event_main_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (main_menu_counter == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
            else if (main_menu_counter == 2)
            {
                option = 2;
                main_menu_counter = 0;
            }
            else if (main_menu_counter == 3)
            {
                option = 1;
                main_menu_counter = 0;
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        main_menu_counter = 0;
        for (int i = 0; i < main_menu_buttons.size(); i++)
        {
            if (x1 > main_menu_buttons[i].rect.x && x1 < (main_menu_buttons[i].rect.x + main_menu_buttons[i].rect.w) &&
                y1 > main_menu_buttons[i].rect.y && y1 < (main_menu_buttons[i].rect.y + main_menu_buttons[i].rect.h))
            {
                main_menu_counter = i + 1;
            }
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        if (main_menu_counter == 0)
        {
            main_menu_counter = 2;
        }
        else
        {
            SDL_Keycode key_press = e.key.keysym.sym;
            if (key_press == SDLK_RIGHT)
            {
                main_menu_counter = (main_menu_counter) % 3 + 1;
            }
            else if (key_press == SDLK_LEFT)
            {
                main_menu_counter = (main_menu_counter - 1) + 3 * (main_menu_counter == 1);
            }
            else if (key_press == SDLK_RETURN)
            {
                if (main_menu_counter == 2)
                {
                    option = 2;
                    main_menu_counter = 0;
                }
                else if (main_menu_counter == 3)
                {
                    option = 1;
                    main_menu_counter = 0;
                }
                else if (main_menu_counter == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        mute_state = 0;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        mute_state = 1;
                    }
                }
            }
        }
    }
    }
}
void Menu::event_play_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (play_menu_counter == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
            else if (play_menu_counter == 2)
            {
                option = 0;
                play_menu_counter = 0;
            }
            else if (play_menu_counter == 3)
            {
                option = 3;
                play_menu_counter = 0;
            }
            else if (play_menu_counter == 4)
            {
                option = 4;
                play_menu_counter = 0;
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        play_menu_counter = 0;
        for (int i = 0; i < play_menu_buttons.size(); i++)
        {
            if (x1 > play_menu_buttons[i].rect.x && x1 < (play_menu_buttons[i].rect.x + play_menu_buttons[i].rect.w) &&
                y1 > play_menu_buttons[i].rect.y && y1 < (play_menu_buttons[i].rect.y + play_menu_buttons[i].rect.h))
            {
                play_menu_counter = i + 1;
            }
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        if (play_menu_counter == 0)
        {
            play_menu_counter = 2;
        }
        else
        {
            SDL_Keycode key_press = e.key.keysym.sym;
            if (key_press == SDLK_RIGHT)
            {
                play_menu_counter = (play_menu_counter) % 4 + 1;
            }
            else if (key_press == SDLK_LEFT)
            {
                play_menu_counter = (play_menu_counter - 1) + 4 * (play_menu_counter == 1);
            }
            else if (key_press == SDLK_RETURN)
            {
                if (play_menu_counter == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        mute_state = 0;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        mute_state = 1;
                    }
                }
                else if (play_menu_counter == 2)
                {
                    option = 0;
                    play_menu_counter = 0;
                }
                else if (play_menu_counter == 3)
                {
                    option = 3;
                    play_menu_counter = 0;
                }
                else if (play_menu_counter == 4)
                {
                    option = 4;
                    play_menu_counter = 0;
                }
            }
        }
    }
    }
}
void Menu::event_help_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (help_menu_counter == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
            else if (help_menu_counter == 2)
            {
                option = 0;
                help_menu_counter = 0;
            }
            else if (help_menu_counter == 3)
            {
                ins_state = 0;
            }
            else if (help_menu_counter == 4)
            {
                ins_state = 1;
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        help_menu_counter = 0;
        ins_coloured_state = ins_state;
        for (int i = 0; i < help_menu_buttons.size(); i++)
        {
            if (x1 > help_menu_buttons[i].rect.x && x1 < (help_menu_buttons[i].rect.x + help_menu_buttons[i].rect.w) &&
                y1 > help_menu_buttons[i].rect.y && y1 < (help_menu_buttons[i].rect.y + help_menu_buttons[i].rect.h))
            {
                if (i == 2 || i == 3)
                {
                    ins_coloured_state = i - 2;
                }
                help_menu_counter = i + 1;
            }
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        if (help_menu_counter == 0)
        {
            help_menu_counter = 3;
        }
        else
        {
            SDL_Keycode key_press = e.key.keysym.sym;
            if (key_press == SDLK_RIGHT)
            {
                help_menu_counter = (help_menu_counter) % 4 + 1;
                if (help_menu_counter == 3 || help_menu_counter == 4)
                {
                    ins_coloured_state = help_menu_counter - 3;
                }
            }
            else if (key_press == SDLK_LEFT)
            {
                help_menu_counter = (help_menu_counter - 1) + 4 * (help_menu_counter == 1);
                if (help_menu_counter == 3 || help_menu_counter == 4)
                {
                    ins_coloured_state = help_menu_counter - 3;
                }
            }
            else if (key_press == SDLK_RETURN)
            {
                if (help_menu_counter == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        mute_state = 0;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        mute_state = 1;
                    }
                }
                else if (help_menu_counter == 2)
                {
                    option = 0;
                    help_menu_counter = 0;
                }
                else if (help_menu_counter == 3)
                {
                    ins_state = 0;
                }
                else if (help_menu_counter == 4)
                {
                    ins_state = 1;
                }
            }
        }
    }
    }
}
void Menu::event_offline_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (offline_menu_counter == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
            else if (offline_menu_counter == 2)
            {
                option = 2;
                offline_menu_counter = 0;
            }
            else if (offline_menu_counter == 3)
            {
                offline_selected = true;
                players_selected = 2;
            }
            else if (offline_menu_counter == 4)
            {
                offline_selected = true;
                players_selected = 3;
            }
            else if (offline_menu_counter == 5)
            {
                offline_selected = true;
                players_selected = 4;
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        offline_menu_counter = 0;
        for (int i = 0; i < offline_menu_buttons.size(); i++)
        {
            if (x1 > offline_menu_buttons[i].rect.x && x1 < (offline_menu_buttons[i].rect.x + offline_menu_buttons[i].rect.w) &&
                y1 > offline_menu_buttons[i].rect.y && y1 < (offline_menu_buttons[i].rect.y + offline_menu_buttons[i].rect.h))
            {
                offline_menu_counter = i + 1;
            }
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        if (offline_menu_counter == 0)
        {
            offline_menu_counter = 2;
        }
        else
        {
            SDL_Keycode key_press = e.key.keysym.sym;
            if (key_press == SDLK_RIGHT)
            {
                offline_menu_counter = (offline_menu_counter) % 5 + 1;
            }
            else if (key_press == SDLK_LEFT)
            {
                offline_menu_counter = (offline_menu_counter - 1) + 5 * (offline_menu_counter == 1);
            }
            else if (key_press == SDLK_RETURN)
            {
                if (offline_menu_counter == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        mute_state = 0;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        mute_state = 1;
                    }
                }
                else if (offline_menu_counter == 2)
                {
                    option = 2;
                    offline_menu_counter = 0;
                }
                else if (offline_menu_counter == 3)
                {
                    offline_selected = true;
                    players_selected = 2;
                }
                else if (offline_menu_counter == 4)
                {
                    offline_selected = true;
                    players_selected = 3;
                }
                else if (offline_menu_counter == 5)
                {
                    offline_selected = true;
                    players_selected = 4;
                }
            }
        }
    }
    }
}
void Menu::event_online_menu(SDL_Event e)
{
    //first rectangle will indicate enter IP, this cannot be toggled through. Second will accept user IP. Third will be start
    //fourth will be back button
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (online_menu_counter == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
            else if (online_menu_counter == 2)
            {
                option = 2;
                online_menu_counter = 5;
            }
            else if (online_menu_counter == 5)
            {
                online_selected = true;
                IP_address = online_menu_buttons[3].text;
                if (online_menu_buttons[3].text == "aman")
                {
                    IP_address = "122.161.203.46";
                }
                else if (online_menu_buttons[3].text == "ishaan")
                {
                    IP_address = "182.68.33.240";
                }
                else if (online_menu_buttons[3].text == "local")
                {
                    IP_address = "127.0.0.1";
                }
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        online_menu_counter = 5;
        for (int i = 0; i < online_menu_buttons.size(); i++)
        {
            if (i != 2 && i != 3)
            {
                if (x1 > online_menu_buttons[i].rect.x && x1 < (online_menu_buttons[i].rect.x + online_menu_buttons[i].rect.w) &&
                    y1 > online_menu_buttons[i].rect.y && y1 < (online_menu_buttons[i].rect.y + online_menu_buttons[i].rect.h))
                {
                    online_menu_counter = i + 1;
                }
            }
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        if (online_menu_counter == 0)
        {
            online_menu_counter = 5;
        }
        else
        {
            SDL_Keycode key_press = e.key.keysym.sym;
            if (key_press == SDLK_RIGHT)
            {
                if (online_menu_counter == 1)
                {
                    online_menu_counter = 2;
                }
                else if (online_menu_counter == 2)
                {
                    online_menu_counter = 5;
                }
                else
                {
                    online_menu_counter = 1;
                }
            }
            else if (key_press == SDLK_LEFT)
            {
                if (online_menu_counter == 1)
                {
                    online_menu_counter = 5;
                }
                else if (online_menu_counter == 2)
                {
                    online_menu_counter = 1;
                }
                else
                {
                    online_menu_counter = 2;
                }
            }
            else if (key_press == SDLK_RETURN)
            {
                if (online_menu_counter == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        mute_state = 0;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        mute_state = 1;
                    }
                }
                else if (online_menu_counter == 2)
                {
                    option = 2;
                    online_menu_counter = 5;
                }
                else if (online_menu_counter == 5)
                {
                    online_selected = true;
                    IP_address = online_menu_buttons[3].text;
                    if (online_menu_buttons[3].text == "aman")
                    {
                        IP_address = "122.161.203.46";
                    }
                    else if (online_menu_buttons[3].text == "ishaan")
                    {
                        IP_address = "182.68.33.240";
                    }
                    else if (online_menu_buttons[3].text == "local")
                    {
                        IP_address = "127.0.0.1";
                    }
                }
            }
            //handle all key presses if online_menu_counter is 2
            else
            {
                string temp = online_menu_buttons[3].text;
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
                online_menu_buttons[3].text = temp;
            }
        }
    }
    }
}

void Menu::event_buffer_menu(SDL_Event e)
{
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (buffer_menu_counter == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
            else if (buffer_menu_counter == 2)
            {
                player_is_ready = !player_is_ready;
                if (player_is_ready)
                {
                    buffer_menu_buttons[1].text = "Unready";
                }
                else
                {
                    buffer_menu_buttons[1].text = "Ready";
                }
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        buffer_menu_counter = 0;
        for (int i = 0; i < buffer_menu_buttons.size(); i++)
        {
            if (x1 > buffer_menu_buttons[i].rect.x && x1 < (buffer_menu_buttons[i].rect.x + buffer_menu_buttons[i].rect.w) &&
                y1 > buffer_menu_buttons[i].rect.y && y1 < (buffer_menu_buttons[i].rect.y + buffer_menu_buttons[i].rect.h))
            {
                buffer_menu_counter = i + 1;
            }
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        if (buffer_menu_counter == 0)
        {
            buffer_menu_counter = 2;
        }
        else
        {
            SDL_Keycode key_press = e.key.keysym.sym;
            if (key_press == SDLK_RIGHT)
            {
                buffer_menu_counter = (buffer_menu_counter) % 2 + 1;
            }
            else if (key_press == SDLK_LEFT)
            {
                buffer_menu_counter = (buffer_menu_counter - 1) + 2 * (buffer_menu_counter == 1);
            }
            else if (key_press == SDLK_RETURN)
            {
                if (buffer_menu_counter == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        mute_state = 0;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        mute_state = 1;
                    }
                }
                else if (buffer_menu_counter == 2)
                {
                    player_is_ready = !player_is_ready;
                    if (player_is_ready)
                    {
                        buffer_menu_buttons[1].text = "Unready";
                    }
                    else
                    {
                        buffer_menu_buttons[1].text = "Ready";
                    }
                }
            }
        }
    }
    }
}
bool Menu::ended() { return offline_selected || online_selected; }
bool Menu::player_ready()
{
    return player_is_ready;
}
void Menu::display_buffer_menu(SDL_Renderer *renderer, SDL_Surface *surface)
{
    // SDL_RenderClear(renderer);
    vector<int> rgba = {0, 0, 0, 255};
    SDL_Color curr_color = {255, 0, 0};
    string curr_text;
    if (player_is_ready)
    {
        curr_text = "Waiting for other Players ...";
    }
    else
    {
        curr_text = "Click on Ready button when ready !";
    }
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    surface = TTF_RenderText_Solid(fonts[0], curr_text.c_str(), curr_color);
    SDL_Rect curr_rect = {(window_width - surface->w) / 2, (window_height - surface->h) / 6, surface->w, surface->h};
    //SDL_RenderDrawRect(renderer, &curr_rect);
    SDL_Texture *display_texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_RenderFillRect(renderer, &curr_rect);
    if (!display_texture)
    {
        // cout << "Failed to create texture" << endl;
    }
    else
    {
        SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
    }
    SDL_DestroyTexture(display_texture);
    int vertical_offset = (window_height * 65) / 144;
    int horizontal_offset = (window_width - total_players * ((window_width * 7) / 128) - (total_players - 1) * (2 * window_width) / 128) / 2;
    for (int i = 0; i < total_players; i++)
    {
        surface = (player_surfaces[i][0]);
        if (!surface)
        {
            cout << "Failed to create surface" << endl;
        }
        SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {horizontal_offset + i * (window_width)*9 / 128, vertical_offset, (window_width * 7) / 128, (window_height * 7) / 72};
        if (!curr_text)
        {
            cout << "Failed to create texture" << endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
        }
        SDL_DestroyTexture(curr_text);
    }
    rgba = {0, 0, 0, 255};
    curr_color = {255, 0, 0};
    curr_text = "Number of players ready = " + to_string(joined_players) + "/" + to_string(total_players);
    SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
    surface = TTF_RenderText_Solid(fonts[2], curr_text.c_str(), curr_color);
    curr_rect = {(window_width - surface->w) / 2, window_height * 110 / 144, surface->w, surface->h};
    //SDL_RenderDrawRect(renderer, &curr_rect);
    display_texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_RenderFillRect(renderer, &curr_rect);
    if (!display_texture)
    {
        cout << "Failed to create texture" << endl;
    }
    else
    {
        SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
    }

    SDL_DestroyTexture(display_texture);
}