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
    void updateLocation(Maze &);
    void takeAction(SDL_Event);
    void render(SDL_Renderer *, Maze &);
    void updateDimensions(Maze &, int, int);
    void set_bomb_type(int);
    void update_bombs(Maze &);
    void reset_bombs(Maze &, int, int);

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
    int bomb_type;      //type of bomb with each player, initialised to 1
    vector<Bomb> bombs; //each player releases 4 bombs travelling in all directions. A player can release an new bomb only
    //when all previous ones have collided.
    vector<bool> collision_status; //true for i'th bomb if it has collided
    bool released;                 //true if some of the bombs of player have not yet collided
};