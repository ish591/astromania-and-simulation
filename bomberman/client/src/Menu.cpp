#include "Menu.h"
Menu::Menu() {
     option = 0; 
     initialise_fonts();
}

void Menu::initialise_fonts(){
    fonts.push_back(TTF_OpenFont("src/m5x7.ttf", 24));
}

void Menu::display(SDL_Renderer *renderer, SDL_Surface *surface)
{
    switch(option)
    {
        case 0:
        main_menu(renderer,  surface);
        break;
        case 1:
        help_menu(renderer,  surface);
        break;
        case 2:
        play_menu(renderer,  surface);
        break;
        case 3:
        offline_menu(renderer,  surface);
        break;
        case 4:
        online_menu(renderer,  surface);
        break;
        default:
        main_menu(renderer,  surface);
        break;
    }
}

void Menu::main_menu(SDL_Renderer *renderer, SDL_Surface *surface)
{
    
        SDL_Rect rect = {100, 100, 100, 30};
        SDL_Rect rect2 = {300,300,100,30};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        string text = "Play";
        SDL_Color color = {255, 255, 255};
        surface = TTF_RenderText_Solid(fonts[0], text.c_str(), color);
        SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
        if (!curr_text)
        {
            cout << "Failed to create texture" << endl;
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
        }
        //SDL_RenderPresent(renderer);
       SDL_DestroyTexture(curr_text);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect2);
        text = "Help";
        surface = TTF_RenderText_Solid(fonts[0], text.c_str(), color);
        curr_text = SDL_CreateTextureFromSurface(renderer, surface);
        if (!curr_text)
        {
            cout << "Failed to create texture" << endl;
            SDL_RenderFillRect(renderer, &rect2);
        }
        else
        {
            SDL_RenderCopy(renderer, curr_text, nullptr, &rect2);;
        }
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(curr_text);
}
void Menu::help_menu(SDL_Renderer *renderer, SDL_Surface *surface)
{
    //display instructions etc
}
void Menu::play_menu(SDL_Renderer *renderer, SDL_Surface *surface){
    //offline, online to be displayed, and a back button
}
void Menu::offline_menu(SDL_Renderer *renderer, SDL_Surface *surface){
    //number of players, 2, 3, 4 and a back button
}
void Menu::online_menu(SDL_Renderer *renderer, SDL_Surface *surface){
    //recieve IP address as input, back button
}