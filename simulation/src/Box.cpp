#include "Box.h"

Box::Box(int t)
{
    Box::type = t;
}

void Box::update(int new_type)
{
    if (new_type != 1 || rand() % 100 < 80)
        type = new_type;
}

int Box::get_block_type()
{
    return type;
}

void Box::render(SDL_Renderer *renderer, SDL_Surface *surface, int x, int y, int w, int h, vector<SDL_Surface *> &block_surface)
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
        surface = (block_surface[0]);
    }
    else
    {
        r = 0;
        g = 0;
        b = 255;
        a = 255;
        surface = (block_surface[1]);
    }
    if (!surface)
    {
        // cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!curr_texture)
    {
        // cout << "Failed to create texture" << endl;
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderCopy(renderer, curr_texture, nullptr, &rect);
    }
    SDL_DestroyTexture(curr_texture);
}