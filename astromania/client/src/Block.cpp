#include "Block.h"

Block::Block(int t, int i, int j)
{
    Block::type = t;
    Block::x = i;
    Block::y = j;
}

void Block::update(int new_type)
{
    type = new_type;
}

int Block::get_block_type()
{
    return type;
}
int Block::get_x()
{
    return x;
}
int Block::get_y()
{
    return y;
}
void Block::render(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Surface *surface, vector<SDL_Surface *> &block_surfaces)
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
        r = 200;
        g = 20;
        b = 0;
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
