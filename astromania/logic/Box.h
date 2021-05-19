#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <vector>
using namespace std;
//describes each box of a maze
class Box
{
public:
    Box(int, int, int);    //ex, 0 for path, 1 for breakable, 2 for unbreakable. type defines the color of the box
    void update(int, int); //update the type of the box
    void render(SDL_Renderer *, int, int, int, int, SDL_Surface *, vector<SDL_Surface *> &);
    int get_block_type();
    int get_start_time();
    int get_duration();
    int get_x();
    int get_y();

private:
    int type;
    int start_time; //required for power up blocks
    int duration;
    int x, y;
};
