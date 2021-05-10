#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Map.h"

using namespace std;

class Renderer
{
public:
    Renderer(int, int, vector<vector<SDL_Surface *>>, vector<SDL_Surface *>, vector<SDL_Surface *>, vector<SDL_Surface *>, SDL_Surface *, Mix_Chunk *, Mix_Chunk *);
    void render_all(SDL_Renderer *, SDL_Surface *);
    void update(vector<int>);
    Map map = Map(1, 1, 100, 100, 0);

private:
    vector<pair<pair<int, int>, SDL_Rect>> players;
    vector<SDL_Rect> bombs;
    vector<SDL_Rect> explosions;
    vector<vector<SDL_Surface *>> player_surfaces;
    vector<SDL_Surface *> bomb_surfaces;
    vector<SDL_Surface *> block_surfaces;
    vector<SDL_Surface *> explosion_surfaces;
    SDL_Surface *heart;
    void render_player(SDL_Renderer *, SDL_Surface *, int, int, SDL_Rect);
    void render_bomb(SDL_Renderer *, SDL_Surface *, SDL_Rect);
    void render_explosion(SDL_Renderer *, SDL_Surface *, SDL_Rect);
    void score_render(SDL_Renderer *, SDL_Surface *);
    int WINDOW_HEIGHT, WINDOW_WIDTH;
    int winner = 0;
    int rem_lives = -1;
    int number_of_players = 0;
    bool winner_screen;
    vector<int> lives;
    Mix_Chunk *win_sound;
    Mix_Chunk *explosion_sound;
    int num_bombs;
};