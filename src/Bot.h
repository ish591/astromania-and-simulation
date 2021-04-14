#pragma once

#include <SDL2/SDL.h>
#include <queue>
#include <stack>
#include <vector>

#include "Player.h"

class Bot : public Player
{
public:
    Bot(int);
    void setDestination(Maze, int, int);
    void render(SDL_Renderer *, Maze);
    void updateLocation(Maze);

private:
    int x_dest, y_dest;
    void calculatePath(vector<vector<int>> maze);
    void center();
    stack<int> path;
};
