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
    Maze(int, int, int, bool);
    void update(int, int, int);
    int getSize();
    void print();
    void render(SDL_Renderer *);
    int getBlockSize();
    vector<vector<Box>> getMaze();
    bool discrete_walls;
    int left_offset, top_offset;

private:
    void generate(int);
    int N;
    int M;
    int block_size;
    vector<vector<Box>> maze;
};
