#include "OfflineGame.h"

OfflineGame::OfflineGame(int num_players, int maze_size, int width, int height, vector<vector<SDL_Surface *>> player_surfaces, vector<SDL_Surface *> block_surfaces, vector<SDL_Surface *> bomb_surfaces, vector<SDL_Surface *> explosion_surfaces, SDL_Surface *heart)
{
    OfflineGame::player_surfaces = player_surfaces;
    OfflineGame::block_surfaces = block_surfaces;
    OfflineGame::bomb_surfaces = bomb_surfaces;
    OfflineGame::explosion_surfaces = explosion_surfaces;
    OfflineGame::maze_size = maze_size;
    OfflineGame::width = width;
    OfflineGame::height = height;
    OfflineGame::heart = heart;
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
    maze = Maze(maze_size, 0, width, height, time(0));
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
    int temp_winner = 0;
    maze.update(current_time);
    vector<vector<int>> player_info;
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
        {
            player_info.push_back({players[i].getId(), players[i].get_x(), players[i].get_y(), players[i].get_x_offset(), players[i].get_y_offset(), players[i].get_size()});
            alive_count++;
            temp_winner = i + 1;
        }
    }

    if (alive_count <= 1 && maze.close_radius != 100)
    {
        maze.close(current_time, 100);
        if (winner == 0)
            winner = temp_winner;
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
void OfflineGame::score_render(SDL_Renderer *renderer, SDL_Surface *surface)
{
    int vertical_offset = (height - height * (10 * players.size() - 3) / 72) / 2;
    for (int i = 0; i < players.size(); i++)
    {
        surface = (player_surfaces[i][0]);
        if (!surface)
        {
            cout << "Failed to create surface" << endl;
        }
        SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {(width * 103) / 128, height * (i)*5 / 36 + vertical_offset, (width * 7) / 128, (height * 7) / 72};
        if (!curr_text)
        {
            cout << "Failed to create texture" << endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
        }
        SDL_DestroyTexture(curr_text);
        for (int j = 0; j < players[i].lives; j++)
        {
            surface = heart;
            if (!surface)
            {
                cout << "Failed to create surface" << endl;
            }
            SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = {(width * 7) / 8 + (j * width * 9) / 256, height * 5 * (i) / 36 + (vertical_offset * 51) / 48, width / 32, height / 18};
            if (!curr_text)
            {
                cout << "Failed to create texture" << endl;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
            }
            SDL_DestroyTexture(curr_text);
        }
    }
}
void OfflineGame::render(SDL_Renderer *renderer, SDL_Surface *surface)
{
    if (winner == 0)
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
        score_render(renderer, surface);
    }
    else if (!maze.closed)
    {
        maze.render(renderer, surface, block_surfaces);
    }
    else
    {
        //maze.render(renderer, surface, block_surfaces);
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].getId() == winner)
            {
                surface = (player_surfaces[players[i].getId() - 1][0]);
            }
            else
            {
                surface = (player_surfaces[players[i].getId() - 1][2]);
            }
            if (!surface)
            {
                cout << "Failed to create surface" << endl;
            }
            SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = {100 * (i + 1), 200, 70, 70};
            if (!curr_text)
            {
                cout << "Failed to create texture" << endl;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
            }

            SDL_DestroyTexture(curr_text);
        }
    }
}