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
    vector<vector<int> > player_info;
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

void Game::render(SDL_Renderer *renderer)
{
    maze.render(renderer);
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
            players[i].render(renderer);
    }
    for (int i = 0; i < bombs.size(); i++)
    {
        bombs[i].render(renderer);
    }
    for (int i = 0; i < explosions.size(); i++)
    {
        explosions[i].render(renderer);
    }
}