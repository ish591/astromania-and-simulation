#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
using namespace std;
//describes each box of a maze
class Box
{
public:
    Box(int);         //ex, 0 for path, 1 for breakable, 2 for unbreakable. type defines the color of the box
    void update(int); //update the type of the box
    void render(SDL_Renderer *, int, int, int, int);
    int get_block_type();

private:
    int type;
};