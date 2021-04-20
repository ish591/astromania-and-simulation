#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>

#include "Maze.h"
class Bomb
{
public:
    //each bomb has a movement speed, x and y coordinates, radius of impact, distance of explosion,
    //a type, and a skin associated with that type, and an audio associated with that type.
    //each player has an object of type bomb associated with it
    Bomb(Maze &, int, int, int, int, int, int, int, int); //initialise bomb with a type, and coordinates, and x,y offsets
    int get_speed();
    int get_x();
    int get_y();
    int get_rad();
    int get_type();
    int get_direction();
    int get_size();
    int get_time_exp();
    int get_id();
    pair<bool, int> update_state(int, Maze &, vector<pair<int, int>>, vector<Bomb> &);
    void update_location(Maze &, vector<pair<int, int>>);
    void explode(Maze &, int);
    void render(SDL_Renderer *);
    void set_direction(int);
    void set_moving();
    //will have to check collision with other player or Maze walls. So need coordinates of other players also.

private:
    int speed;
    int time_beg;
    int time_explode;
    int id;
    int count;
    int x;
    int y;
    int radius;
    int type;
    int direction; //can be 4 values, 1 for left, 2 for up, 3 for right, 4 for down
    int bomb_size;
    int block_size;
    int x_offset, y_offset;
    int left_offset, top_offset;
    bool moving_bomb;
};
