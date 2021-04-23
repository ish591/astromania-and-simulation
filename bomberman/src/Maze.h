#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stack>
#include <vector>
#include "Box.h"
//this is the interface file basically
using namespace std;

class Maze
{
public:
    Maze(int, bool, int, int);
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
    void close(int);

private:
    void generate(int);
    int N;
    int M;
    int block_size;
    vector<vector<Box>> maze;
    vector<Box> power_ups;
    //each powee up box has a type,coordinate, and ending time
};
