#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include "Maze.h"
#include "Bomb.h"

using namespace std;

class Player
{
public:
    Player(int, Maze &);
    int getId();
    // pair<int, int> getLocation();
    void updateLocation(Maze &, vector<Player> &, vector<Bomb> &, int);
    void takeAction(SDL_Event, Maze &, vector<Bomb> &, int);
    void render(SDL_Renderer *);
    void updateDimensions(Maze &, int, int);
    void set_bomb_type(int);
    void update_bombs(Maze &, vector<Player> &, vector<Bomb> &, int);
    void reset_bombs(Maze &, int, int);
    int get_x();
    int get_y();
    int get_bomb_count();
    void update_bomb_count(int);

protected:
    int player_id;
    int x_offset, y_offset;
    int x, y;
    int score;
    int reset_values;
    int UP_PRESSED;
    int LEFT_PRESSED;
    int DOWN_PRESSED;
    int RIGHT_PRESSED;
    int SPACE_PRESSED;
    int block_size;
    int player_size;
    int left_offset, top_offset;
    int move_size;
    int bomb_count;
    int total_released;
    vector<bool> power_ups;
    //each player has some power ups. for now, 1 corresponds to the throwing power_up
};