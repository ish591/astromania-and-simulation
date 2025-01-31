#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "Player.h"
using namespace std;

class OfflineGame
{
public:
    OfflineGame(int, int, int, int, vector<vector<SDL_Surface *>>, vector<SDL_Surface *>, vector<SDL_Surface *>, vector<SDL_Surface *>, SDL_Surface *, Mix_Chunk *, Mix_Chunk *, TTF_Font *, int, vector<SDL_Surface *> &);
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
    SDL_Surface *heart;
    int maze_size = 7;
    int level;
    int start_time, maze_update_time;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Window *win;
    SDL_Event e;
    int updates;
    int winner = 0;
    void score_render(SDL_Renderer *, SDL_Surface *);
    Mix_Chunk *win_sound;
    Mix_Chunk *explosion_sound;
    TTF_Font *game_font;
    bool win_screen;
    int num_bombs;
    int mute_hover;
    int mute_state;
    SDL_Rect mute_rect{0, 0, 0, 0};
    vector<SDL_Surface *> mute_unmute;
};