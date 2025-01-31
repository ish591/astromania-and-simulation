#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include <SDL2/SDL_image.h>
#include "Bomb.h"
using namespace std;

class Player
{
public:
    Player(int, Maze &);
    int getId();
    // pair<int, int> getLocation();
    void updateLocation(Maze &, vector<Player> &, vector<Bomb> &, int, vector<Explosion> &);
    void takeAction(int, SDL_Keycode, Maze &, vector<Bomb> &, int);
    void render(SDL_Renderer *, SDL_Surface *, vector<vector<SDL_Surface *>> &);
    string render();
    void updateDimensions(Maze &, int, int);
    void set_bomb_type(int);
    void update_bombs(Maze &, vector<Player> &, vector<Bomb> &, int, vector<Explosion> &);
    void reset_bombs(Maze &, int, int);
    int get_x();
    int get_y();
    int get_x_offset();
    int get_y_offset();
    int get_size();
    int get_bomb_count();
    void update_bomb_count(int);
    void update_power_ups(int);
    void kill(int);
    bool isAlive();
    int lives;
    SDL_Keycode UP, DOWN, RIGHT, LEFT, DROP_BOMB;

protected:
    int player_id;
    int player_state; //direction in which player is looking, 0 front, 1 back, 2 left, 3 right
    int power_up_duration;
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
    int last_life_loss_time;
    int color_r, color_g, color_b;
    vector<pair<int, int>> power_ups;

    //each player has some power ups. for now, 1 corresponds to the throwing power_up
};