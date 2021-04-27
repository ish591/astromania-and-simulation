#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#include "Player.h"
#include "Network.h"

using namespace std;

class OnlineGame
{
public:
    OnlineGame(int, int, int, int);
    ~OnlineGame();
    bool run();
    void newLevel();
    void render(int);
    void update(int);
    void control(vector<vector<int>>, int);
    Maze maze = Maze(1, 100, 100, 100, 0);
    int width, height;
    vector<Player> players;
    vector<Bomb> bombs;
    vector<Explosion> explosions;
    int maze_size = 7;
    int level;
    int start_time, maze_update_time, maze_update_send_time;
    int render_state_send_time = 0;
    int updates;
    Network network;
};