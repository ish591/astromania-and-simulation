#include "Game.h"

Game::Game(int num_players, int maze_size, int width, int height)
{
    Game::maze_size = maze_size;
    Game::width = width;
    Game::height = height;
    newLevel();
    Player player(1, maze);
    player.updateDimensions(maze, width, height);
    players.push_back(player);
}

void Game::newLevel()
{
    maze = Maze(maze_size, 1, width, height);
}

void Game::control(SDL_Event e, int current_time)
{
    players[0].takeAction(e, maze, bombs, current_time);
}

void Game::update(int current_time)
{
    maze.update_power_ups(current_time);
    players[0].updateLocation(maze, players, bombs, current_time);
    //each power up has a certain time, so update the powe up blocks also
}

void Game::render(SDL_Renderer *renderer)
{
    maze.render(renderer);
    players[0].render(renderer);
    for (auto u : bombs)
    {
        u.render(renderer);
    }
}