#include "OnlineGame.h"

OnlineGame::OnlineGame(int num_players, int maze_size, int width, int height)
{
    if (SDL_Init(SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        exit(3);
    }
    OnlineGame::maze_size = maze_size;
    OnlineGame::width = width;
    OnlineGame::height = height;
    network.seed = (time(0) % 100000);
    network.maze_size = maze_size;
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
    maze_update_send_time = 0;
}

OnlineGame::~OnlineGame()
{
    SDL_Quit();
}

bool OnlineGame::run()
{
    Uint32 curTicks = SDL_GetTicks();
    vector<vector<int>> actions = network.recv();
    control(actions, curTicks);
    render(curTicks);

    int val = (curTicks - start_time) / 5;
    for (; updates < val; updates++)
    {
        update(curTicks - (val - updates + 1) * 5);
    }
    SDL_Delay(1);
    return false;
}

void OnlineGame::newLevel()
{
    maze = Maze(maze_size, 1, width, height, network.seed);
}

void OnlineGame::control(vector<vector<int>> actions, int current_time)
{
    for (int i = 0; i < actions.size(); i++)
    {
        switch (actions[i][0])
        {
        case 1:
            SDL_Keycode key_press;
            switch (actions[i][3])
            {
            case 0:
                key_press = players[actions[i][1] - 1].LEFT;
                break;
            case 1:
                key_press = players[actions[i][1] - 1].UP;
                break;
            case 2:
                key_press = players[actions[i][1] - 1].RIGHT;
                break;
            case 3:
                key_press = players[actions[i][1] - 1].DOWN;
                break;
            case 4:
                key_press = players[actions[i][1] - 1].DROP_BOMB;
            }
            if (players[actions[i][1] - 1].isAlive())
            {
                players[actions[i][1] - 1].takeAction(actions[i][2], key_press, maze, bombs, current_time);
            }
            break;
        default:
            break;
        }
    }
}

void OnlineGame::update(int current_time)
{
    // if (current_time > maze_update_time + 20000)
    // {
    //     maze.close(current_time, maze.close_radius + 1);
    //     maze_update_time = current_time;
    // }
    if (current_time > maze_update_send_time + 200)
    {
        string maze_updates = maze.getBoxUpdates();
        for (int i = 0; i < 2; i++)
        {
            // cout << maze_updates << endl;
            if (maze_updates.length() > 4)
            {
                // cout << maze_updates << endl;
                network.sendState(maze_updates);
                SDL_Delay(10);
            }
        }
        maze_update_send_time = current_time;
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

void OnlineGame::render(int current_time)
{
    // maze.render(renderer, surface, block_surfaces);

    if (current_time > render_state_send_time + 30)
    {
        string s = "1 ";
        for (int i = 0; i < players.size(); i++)
        {
            // if (players[i].isAlive())
            s += players[i].render();
        }
        for (int i = 0; i < bombs.size(); i++)
        {
            s += bombs[i].render();
        }
        for (int i = 0; i < explosions.size(); i++)
        {
            s += explosions[i].render();
        }
        network.sendState(s);
        render_state_send_time = current_time;
    }
}