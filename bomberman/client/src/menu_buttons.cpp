#include "menu_buttons.h"

menu_buttons::menu_buttons(SDL_Rect rect, SDL_Color color, string text, TTF_Font *font, vector<int> rgba)
{
    menu_buttons::rect = rect;
    menu_buttons::text_color = color;
    menu_buttons::text = text;
    menu_buttons::font = font;
    menu_buttons::rect_color = rgba;
}