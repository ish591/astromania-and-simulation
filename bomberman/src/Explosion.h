#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "Maze.h"
class Explosion
{
public:
    Explosion(int, int, int, int, int, Maze &, int);
    int get_id();
    void create_actual_rectangle();
    vector<int> update(int, vector<Explosion> &, Maze &, vector<vector<int>>);
    bool isOver(int);
    void kill_blocks(Maze &, int, int, int);
    bool check_player_killed(vector<int>, vector<int>);
    vector<int> kill(Maze &, vector<vector<int>>, int);
    void render(SDL_Renderer *);

private:
    int top_x, top_y;
    int min_coord, max_coord, const_coord;
    int direction;
    int width, height;
    int block_size;
    int left_offset, top_offset;
    int id;
    int dir;
    int duration;
    int end_time;
    int explosion_offset;
};