#include "Box.h"

Box::Box(int t)
{
    Box::type = t;
}

void Box::update(int new_type)
{
    type = new_type;
}

int Box::get_block_type()
{
    return type;
}

void Box::render(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect = {x, y, w, h};
    int r, g, b, a;
    if (type == 0)
    {
        r = 0;
        g = 0;
        b = 0;
        a = 255;
    }
    else if (type == 1)
    {
        r = 0;
        g = 0;
        b = 255;
        a = 200;
    }
    else
    {
        r = 200;
        g = 20;
        b = 0;
        a = 255;
    }
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}