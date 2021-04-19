#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#include "Maze.h"
#include "Player.h"
#include "Block.h"
#include "Bomb.h"

using namespace std;

class Game
{
public:
    Game(int, int, int, int);
    void newLevel();
    void render(SDL_Renderer *);
    void update(int);
    void control(SDL_Event, int);
    Maze maze = Maze(1, 100, 100, 100);
    int width, height;
    vector<Player> players;
    vector<Bomb> bombs;
    int maze_size = 7;
    int level;
};