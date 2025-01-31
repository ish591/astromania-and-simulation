#include "Box.h"

Box::Box(int t, int i, int j)
{
    Box::type = t;
    Box::start_time = -1;
    Box::duration = -1;
    Box::x = i;
    Box::y = j;
}

void Box::update(int new_type, int current_time)
{
    type = new_type;
    if (type <= 2)
    {
        start_time = -1;
        duration = -1;
    }
    else
    {
        start_time = current_time;
        duration = 9000;
    }
}

int Box::get_block_type()
{
    return type;
}
int Box::get_start_time()
{
    return start_time;
}
int Box::get_duration()
{
    return duration;
}
int Box::get_x()
{
    return x;
}
int Box::get_y()
{
    return y;
}
void Box::render(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Surface *surface, vector<SDL_Surface *> &block_surfaces)
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
    else if (type == 2)
    {
        r = 135;
        g = 120;
        b = 105;
        a = 255;
    }
    else if (type == 3) //power ups for now
    {
        r = 120;
        g = 100;
        b = 25;
        a = 200;
    }
    else if (type == 4)
    {
        r = 180;
        g = 30;
        b = 70;
        a = 200;
    }
    else if (type == 5)
    {
        r = 30;
        g = 230;
        b = 0;
        a = 200;
    }
    else
    {
        r = 100;
        g = 100;
        b = 100;
        a = 200;
    }
    // if (type < 3)
    // {
    //     SDL_SetRenderDrawColor(renderer, r, g, b, a);
    //     SDL_RenderFillRect(renderer, &rect);
    // }
    // else
    // {
    surface = (block_surfaces[type]);
    if (!surface)
    {
        cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!curr_texture)
    {
        cout << "Failed to create texture" << endl;
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderCopy(renderer, curr_texture, nullptr, &rect);
    }
    SDL_DestroyTexture(curr_texture);
    // }
}