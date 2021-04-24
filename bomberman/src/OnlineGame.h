#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "Player.h"
#include "Network.h"
#include "Enemy.h"
using namespace std;

class OnlineGame
{
public:
    OnlineGame(int, int, int, int);
    void newLevel(int);
    void render(SDL_Renderer *, SDL_Surface *);
    void update(int);
    void control(SDL_Event, int);
    void loadTextures();
    Maze maze = Maze(1, 100, 100, 100, 1);
    int width, height;
    vector<Player> players;
    vector<Enemy> enemies;
    vector<Bomb> bombs;
    vector<Explosion> explosions;
    vector<vector<SDL_Surface *>> player_surfaces;
    vector<SDL_Surface *> block_surfaces;
    vector<SDL_Surface *> bomb_surfaces;
    SDL_Surface *explosion_surface;
    int maze_size = 7;
    int level;
    int start_time;
    Network network = Network("127.0.0.1");
};