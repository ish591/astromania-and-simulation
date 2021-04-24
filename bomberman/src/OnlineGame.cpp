#include "OnlineGame.h"

OnlineGame::OnlineGame(int num_players, int maze_size, int width, int height)
{
    OnlineGame::maze_size = maze_size;
    OnlineGame::width = width;
    OnlineGame::height = height;
    loadTextures();
    enemies.push_back(Enemy(2));
}
void OnlineGame::loadTextures()
{
    string pref_players = "./Images/player";
    for (int i = 0; i < 2; i++)
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

    start_time = SDL_GetTicks();
}
void OnlineGame::newLevel(int seed)
{
    maze = Maze(maze_size, 1, width, height, seed);
}

void OnlineGame::control(SDL_Event e, int current_time)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
            players[i].takeAction(e, maze, bombs, current_time);
    }
}

void OnlineGame::update(int current_time)
{
    if (current_time > start_time + 100)
    {
        //maze.close(current_time, maze.close_radius + 1);
        start_time = current_time;
    }
    int alive_count = 0;

    maze.update(current_time);

    for (int i = 0; i < players.size(); i++)
    {
        network.send(players[i].getId(), players[i].get_x(), players[i].get_y(), players[i].get_x_offset(), players[i].get_y_offset());
    }
    vector<int> v = network.recv();
    if (v.size() >= 6)
    {
        switch (v[0])
        {
        case 0:
            //v[2] id, v[3] has seed
            newLevel(v[3]);

            players.push_back(Player(v[2], maze));
            players[0].updateDimensions(maze, width, height);

            enemies[0].updateDimensions(maze, width, height);
            break;
        case 1:
            if (enemies.size() > 0)
            {
                enemies[0].x = v[2];
                enemies[0].y = v[3];
                enemies[0].x_offset = v[4];
                enemies[0].y_offset = v[5];
                break;
            }
        default:
            break;
        }
    }

    vector<vector<int>> player_info;
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
        {
            player_info.push_back({players[i].getId(), players[i].get_x(), players[i].get_y(), players[i].get_x_offset(), players[i].get_y_offset(), players[i].get_size()});
            alive_count++;
        }
    }
    alive_count = 2;

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

void OnlineGame::render(SDL_Renderer *renderer, SDL_Surface *surface)
{

    maze.render(renderer, surface, block_surfaces);

    for (int i = 0; i < explosions.size(); i++)
    {
        explosions[i].render(renderer, surface, explosion_surface);
    }
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].isAlive())
            players[i].render(renderer, surface, player_surfaces);
    }
    if (enemies.size())
    {
        enemies[0].render(renderer, surface, player_surfaces);
    }
    for (int i = 0; i < bombs.size(); i++)
    {
        bombs[i].render(renderer, surface, bomb_surfaces);
    }
}