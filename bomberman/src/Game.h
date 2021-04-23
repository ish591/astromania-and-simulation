#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "Player.h"
using namespace std;

class Game
{
public:
    Game(int, int, int, int);
    void newLevel();
    void render(SDL_Renderer *, SDL_Surface *);
    void update(int);
    void control(SDL_Event, int);
    void loadTextures();
    Maze maze = Maze(1, 100, 100, 100);
    int width, height;
    vector<Player> players;
    vector<Bomb> bombs;
    vector<Explosion> explosions;
    vector<vector<SDL_Surface *>> player_surfaces;
    vector<SDL_Surface *> block_surfaces;
    vector<SDL_Surface *> bomb_surfaces;
    SDL_Surface *explosion_surface;
    int maze_size = 7;
    int level;
};