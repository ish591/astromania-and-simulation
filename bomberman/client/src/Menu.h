#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
class Menu
{
private:
    int option = 0;

public:
    Menu();
    void initialise_fonts();
    void display(SDL_Renderer *, SDL_Surface *);
    void main_menu(SDL_Renderer* , SDL_Surface *);
    void help_menu(SDL_Renderer *, SDL_Surface *);
    void play_menu(SDL_Renderer *, SDL_Surface *);
    void offline_menu(SDL_Renderer *, SDL_Surface *);
    void online_menu(SDL_Renderer *, SDL_Surface *);
    vector<TTF_Font *> fonts;
};