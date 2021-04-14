#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stack>
#include <vector>

using namespace std;

class Maze
{
public:
    Maze();
    void generate(int N);
    void update(vector<pair<int, int>>);
    int getSize();
    void print();
    void render(SDL_Renderer *, bool);
    vector<vector<int>> getMaze();

private:
    int N;
    vector<vector<int>> maze;
};
