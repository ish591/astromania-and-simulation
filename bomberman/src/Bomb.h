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
    Bomb(Maze &, int, int, int, int, int, int); //initialise bomb with a type, direction, and coordinates, and x,y offsets
    int get_speed();
    int get_x();
    int get_y();
    int get_rad();
    int get_dist_exp();
    int get_type();
    bool update_location(Maze &);
    int get_dist_travelled();
    bool collision(Maze &);
    int get_direction();
    int get_size();
    void render(SDL_Renderer *);
    //will have to check collision with other player or maze walls. So need coordinates of other players also.

private:
    int speed;
    int x;
    int y;
    int radius;
    int dist_explode;
    int type;
    int dist_travelled;
    int direction; //can be 4 values, 1 for left, 2 for up, 3 for right, 4 for down
    int bomb_size;
    int block_size;
    int x_offset, y_offset;
    int left_offset, top_offset;
};
