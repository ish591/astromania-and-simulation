#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include <SDL2/SDL_image.h>
#include "Bomb.h"
using namespace std;

class Enemy
{
public:
    Enemy(int);

    int x_offset, y_offset;
    int x, y;
    void updateDimensions(Maze &, int, int);
    void render(SDL_Renderer *, SDL_Surface *, vector<vector<SDL_Surface *>> &);

private:
    int player_id;
    int player_size;
    int block_size;

    int left_offset, top_offset;

    int color_r, color_g, color_b;
};