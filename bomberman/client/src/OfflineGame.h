#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "Player.h"
using namespace std;

class OfflineGame
{
public:
    OfflineGame(int, int, int, int, vector<vector<SDL_Surface *>>, vector<SDL_Surface *>, vector<SDL_Surface *>, vector<SDL_Surface *>);
    ~OfflineGame();
    void newLevel();
    void render(SDL_Renderer *, SDL_Surface *);
    void update(int);
    void control(SDL_Event, int);
    Maze maze = Maze(1, 100, 100, 100, 0);
    int width, height;
    vector<Player> players;
    vector<Bomb> bombs;
    vector<Explosion> explosions;
    vector<vector<SDL_Surface *>> player_surfaces;
    vector<SDL_Surface *> block_surfaces;
    vector<SDL_Surface *> bomb_surfaces;
    vector<SDL_Surface *> explosion_surfaces;
    int maze_size = 7;
    int level;
    int start_time, maze_update_time;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Window *win;
    SDL_Event e;
    int updates;
};