#include "Game.h"

Game::Game(int num_players, int maze_size, int width, int height)
{
    Game::maze_size = maze_size;
    Game::width = width;
    Game::height = height;
    newLevel();

    for (int i = 1; i <= 2; i++)
    {
        Player player(i, maze);
        player.updateDimensions(maze, width, height);
        players.push_back(player);
    }
    loadTextures();
}
void Game::loadTextures()
{
    string pref_players = "./Images/player";
    for (int i = 0; i < players.size(); i++)
    {
        vector<SDL_Surface *> curr;
        string image_path = pref_players + to_string(i + 1);
        string front_im = image_path + "front.png";
        SDL_Surface *new_surface = IMG_Load(front_im.c_str());
        curr.push_back(new_surface);
        string back_im = image_path + "back.png";
        SDL_Surface *new_surface1 = IMG_Load(back_im.c_str());
        curr.push_back(new_surface1);
        string left_im = image_path + "left.png";
        SDL_Surface *new_surface2 = IMG_Load(left_im.c_str());
        curr.push_back(new_surface2);
        string right_im = image_path + "right.png";
        SDL_Surface *new_surface3 = IMG_Load(right_im.c_str());
        curr.push_back(new_surface3);
        player_surfaces.push_back(curr);
    }
    string pref_bombs = "./Images/bomb";
    for (int i = 0; i < 1; i++)
    {
        string image_path = pref_bombs + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        bomb_surfaces.push_back(new_surface);
    }
    string pref_power_ups = "./Images/power_ups";
    for (int i = 0; i < 4; i++)
    {
        string image_path = pref_power_ups + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        block_surfaces.push_back(new_surface);
    }
    string image_explosion = "./Images/explosion.png";
    explosion_surface = IMG_Load(image_explosion.c_str());
}
void Game::newLevel()
{
    maze = Maze(maze_size, 1, width, height);
}

void Game::control(SDL_Event e, int current_time)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
            players[i].takeAction(e, maze, bombs, current_time);
    }
}

void Game::update(int current_time)
{
    vector<vector<int>> player_info;
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
        {
            player_info.push_back({players[i].getId(), players[i].get_x(), players[i].get_y(), players[i].get_x_offset(), players[i].get_y_offset(), players[i].get_size()});
        }
    }
    for (int i = 0; i < explosions.size(); i++)
    {
        vector<int> final_ids = explosions[i].update(current_time, explosions, maze, player_info);

        for (int i = 0; i < players.size(); i++)
        {
            for (int j = 0; j < final_ids.size(); j++)
            {
                if (players[i].getId() == final_ids[j])
                {
                    players[i].kill(current_time);
                    // new_players.push_back(players[i]);
                    // break;
                }
            }
        }
    }
    maze.update_power_ups(current_time);
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
        {
            players[i].updateLocation(maze, players, bombs, current_time, explosions);
        }
    }
}

void Game::render(SDL_Renderer *renderer, SDL_Surface *surface)
{
    maze.render(renderer, surface, block_surfaces);
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
            players[i].render(renderer, surface, player_surfaces);
    }
    for (int i = 0; i < bombs.size(); i++)
    {
        bombs[i].render(renderer, surface, bomb_surfaces);
    }
    for (int i = 0; i < explosions.size(); i++)
    {
        explosions[i].render(renderer, surface, explosion_surface);
    }
}