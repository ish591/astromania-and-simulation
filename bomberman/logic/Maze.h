#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stack>
#include <vector>

#include "Box.h"
#include "PRG.h"
using namespace std;

class Maze
{
public:
    Maze(int, bool, int, int, int);
    void update(int);
    void update(int, int, int, int);
    int getSize();
    void print();
    void render(SDL_Renderer *, SDL_Surface *, vector<SDL_Surface *> &);
    int getBlockSize();
    void update_power_ups(int);
    vector<Box> get_power_ups();
    vector<vector<Box>> getMaze();
    void add_power_up(int, int);
    bool discrete_walls;
    int left_offset, top_offset;
    void close(int, int);
    string getBoxUpdates();
    int close_radius;
    bool closed;

private:
    PRG randomiser = PRG(0);
    vector<vector<int>> maze_matrix;
    void generate(int, int);
    int N;
    int M;
    int block_size;
    vector<vector<Box>> maze;
    vector<Box> power_ups;
    pair<int, int> last_close;
    int close_direction;
    int last_close_time;
    //each powee up box has a type,coordinate, and ending time
};
