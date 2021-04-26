#include "OfflineGame.h"

OfflineGame::OfflineGame(int num_players, int maze_size, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        exit(3);
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        cout << " Image failed to be initialised" << SDL_GetError() << endl;
        exit(3);
    }
    win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    renderer = SDL_CreateRenderer(win, -1, 0); //-1 denotes its the first renderer
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!win)
    {
        cout << "Error" << endl;
        SDL_Quit();
    }

    OfflineGame::maze_size = maze_size;
    OfflineGame::width = width;
    OfflineGame::height = height;
    newLevel();
    updates = 0;
    for (int i = 1; i <= 2; i++)
    {
        Player player(i, maze);
        player.updateDimensions(maze, width, height);
        players.push_back(player);
    }
    loadTextures();
    start_time = SDL_GetTicks();
    maze_update_time = start_time;
}

OfflineGame::~OfflineGame()
{
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

bool OfflineGame::run()
{
    Uint32 curTicks = SDL_GetTicks();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    render(renderer, surface);
    int val = (curTicks - start_time) / 5;
    for (; updates < val; updates++)
    {
        update(curTicks - (val - updates + 1) * 5);
    }

    while (SDL_PollEvent(&e))
    {

        if (e.type == SDL_QUIT)
        {
            return true;
        }

        control(e, curTicks);
    }
    SDL_RenderPresent(renderer);
    return false;
}

void OfflineGame::loadTextures()
{
    string pref_players = "../Images/player";
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
    string pref_bombs = "../Images/bomb";
    for (int i = 0; i < 1; i++)
    {
        string image_path = pref_bombs + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        bomb_surfaces.push_back(new_surface);
    }
    string pref_power_ups = "../Images/power_ups";
    for (int i = 0; i < 4; i++)
    {
        string image_path = pref_power_ups + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        block_surfaces.push_back(new_surface);
    }
    string image_explosion = "../Images/explosion.png";
    explosion_surface = IMG_Load(image_explosion.c_str());
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