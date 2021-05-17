#pragma once

#include <SDL2/SDL.h>
#include <queue>
#include <stack>
#include <vector>
#include <utility>

#include "Player.h"
#include "Coin.h"

class Bot : public Player
{
public:
    Bot(int);
    void setDestination(Maze, int, int, vector<Coin>);
    void render(SDL_Renderer *, SDL_Surface *, SDL_Surface *);
    void updateLocation(Maze);
    vector<vector<int>> bfs(Maze, int, int);
    vector<vector<int>> createMatrix(Maze, vector<Coin>);
    vector<int> findOptimalPermutation(Maze, vector<Coin>);

private:
    int x_dest, y_dest;
    void calculatePath(vector<vector<Box>>, int, int);
    void center();
    stack<pair<int, int>> path;
    queue<pair<int, int>> destinations;
};
