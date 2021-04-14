#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>

#include "Maze.h"

using namespace std;

class Player
{
public:
    Player(int);
    int getId();
    // pair<int, int> getLocation();
    void updateLocation(Maze);
    void takeAction(SDL_Event);
    void render(SDL_Renderer *, Maze);
    void updateDimensions(Maze, int, int);

protected:
    int player_id;
    int x_offset, y_offset;
    int x, y;
    int score;
    int reset_values;
    int UP_PRESSED;
    int LEFT_PRESSED;
    int DOWN_PRESSED;
    int RIGHT_PRESSED;
    int block_size;
    int player_size;
    int left_offset, top_offset;
    int move_size;
};