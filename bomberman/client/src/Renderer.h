#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Map.h"

using namespace std;

class Renderer
{
public:
    Renderer(int, int);
    void render_all(SDL_Renderer *, SDL_Surface *);
    void update(vector<int>);
    void loadTextures(int);
    Map map = Map(1, 1, 100, 100, 0);

private:
    vector<pair<pair<int, int>, SDL_Rect>> players;
    vector<SDL_Rect> bombs;
    vector<SDL_Rect> explosions;
    vector<vector<SDL_Surface *>> player_surfaces;
    vector<SDL_Surface *> bomb_surfaces;
    vector<SDL_Surface *> block_surfaces;
    SDL_Surface *explosion_surface;
    void render_player(SDL_Renderer *, SDL_Surface *, int, int, SDL_Rect);
    void render_bomb(SDL_Renderer *, SDL_Surface *, SDL_Rect);
    void render_explosion(SDL_Renderer *, SDL_Surface *, SDL_Rect);
    int WINDOW_HEIGHT, WINDOW_WIDTH;
};