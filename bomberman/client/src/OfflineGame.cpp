#include "OfflineGame.h"

OfflineGame::OfflineGame(int num_players, int maze_size, int width, int height, vector<vector<SDL_Surface *>> player_surfaces, vector<SDL_Surface *> block_surfaces, vector<SDL_Surface *> bomb_surfaces, vector<SDL_Surface *> explosion_surfaces)
{
    OfflineGame::player_surfaces = player_surfaces;
    OfflineGame::block_surfaces = block_surfaces;
    OfflineGame::bomb_surfaces = bomb_surfaces;
    OfflineGame::explosion_surfaces = explosion_surfaces;
    OfflineGame::maze_size = maze_size;
    OfflineGame::width = width;
    OfflineGame::height = height;
    newLevel();
    updates = 0;
    for (int i = 1; i <= num_players; i++)
    {
        Player player(i, maze);
        player.updateDimensions(maze, width, height);
        players.push_back(player);
    }
    start_time = SDL_GetTicks();
    maze_update_time = start_time;
}

OfflineGame::~OfflineGame()
{
    // Destroy object
}

void OfflineGame::newLevel()
{
    maze = Maze(maze_size, 1, width, height, time(0));
}

void OfflineGame::control(SDL_Event e, int current_time)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
        {
            if (e.type == SDL_KEYDOWN)
            {
                // cout << "hello" << endl;
                players[i].takeAction(0, e.key.keysym.sym, maze, bombs, current_time);
            }
            else if (e.type == SDL_KEYUP)
            {
                players[i].takeAction(1, e.key.keysym.sym, maze, bombs, current_time);
                // cout << "bye" << endl;
            }
        }
    }
}

void OfflineGame::update(int current_time)
{
    if (current_time > maze_update_time + 20000)
    {
        maze.close(current_time, maze.close_radius + 1);
        maze_update_time = current_time;
    }
    int alive_count = 0;
    maze.update(current_time);
    vector<vector<int>> player_info;
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
        {
            player_info.push_back({players[i].getId(), players[i].get_x(), players[i].get_y(), players[i].get_x_offset(), players[i].get_y_offset(), players[i].get_size()});
            alive_count++;
        }
    }

    if (alive_count <= 1 && maze.close_radius != 100)
        maze.close(current_time, 100);

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

void OfflineGame::render(SDL_Renderer *renderer, SDL_Surface *surface)
{
    maze.render(renderer, surface, block_surfaces);
    for (int i = 0; i < players.size(); i++)
    {
        // if (players[i].isAlive())
        players[i].render(renderer, surface, player_surfaces);
    }
    for (int i = 0; i < bombs.size(); i++)
    {
        bombs[i].render(renderer, surface, bomb_surfaces);
    }
    for (int i = 0; i < explosions.size(); i++)
    {
        explosions[i].render(renderer, surface, explosion_surfaces);
    }
}