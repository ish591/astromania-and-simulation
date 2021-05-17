#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <SDL2/SDL_mixer.h>

#include "Maze.h"

class Coin
{
public:
    Coin(int, int, int, int, SDL_Surface *);
    void render(SDL_Renderer *, SDL_Surface *);
    pair<int, int> getCoordinates();
    void setLocation(vector<Coin>);
    void collect();
    static Mix_Chunk *collect_coin;

private:
    int N;
    int left_offset, top_offset;
    int x, y;
    int coin_size = 10;
    int block_size;
    bool collected;
    SDL_Surface *coin_surface;
};
