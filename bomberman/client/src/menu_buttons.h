#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
using namespace std;
class menu_buttons
{
public:
    menu_buttons(SDL_Rect, SDL_Color, string, TTF_Font *, vector<int>);
    SDL_Rect rect;
    SDL_Color text_color;
    vector<int> rect_color; //r,g,b,a
    string text;
    TTF_Font *font;
};