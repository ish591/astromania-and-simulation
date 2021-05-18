#include "OfflineGame.h"

OfflineGame::OfflineGame(int num_players, int maze_size, int width, int height, vector<vector<SDL_Surface *>> player_surfaces, vector<SDL_Surface *> block_surfaces, vector<SDL_Surface *> bomb_surfaces, vector<SDL_Surface *> explosion_surfaces, SDL_Surface *heart, Mix_Chunk *win_sound, Mix_Chunk *explosion_sound, TTF_Font *curr_font, int mute_state, vector<SDL_Surface *> &mute_unmute)
{
    OfflineGame::player_surfaces = player_surfaces;
    OfflineGame::block_surfaces = block_surfaces;
    OfflineGame::bomb_surfaces = bomb_surfaces;
    OfflineGame::explosion_surfaces = explosion_surfaces;
    OfflineGame::maze_size = maze_size;
    OfflineGame::width = width;
    OfflineGame::height = height;
    OfflineGame::heart = heart;
    OfflineGame::win_sound = win_sound;
    OfflineGame::explosion_sound = explosion_sound;
    OfflineGame::game_font = curr_font;
    OfflineGame::mute_unmute = mute_unmute;
    mute_rect = {width - (width * 100) / 1280, (height * 8) / 10, (width * 70) / 1280, (height * 50) / 720};
    num_bombs = 0;
    newLevel();
    updates = 0;
    for (int i = 1; i <= num_players; i++)
    {
        Player player(i, maze);
        player.updateDimensions(maze, width, height);
        players.push_back(player);
    }
    start_time = SDL_GetTicks();
    maze_update_time = start_time + 40000;
    win_screen = false;
    mute_hover = 0;
    OfflineGame::mute_state = mute_state;
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
    switch (e.type)
    {
    case (SDL_MOUSEBUTTONDOWN):
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (mute_hover == 1)
            {
                if (Mix_PausedMusic() == 1)
                {
                    //Resume the music
                    Mix_ResumeMusic();
                    mute_state = 0;
                }
                //If the music is playing
                else
                {
                    //Pause the music
                    Mix_PauseMusic();
                    mute_state = 1;
                }
            }
        }
        break;
    }
    case (SDL_MOUSEMOTION):
    {
        int x1, y1;
        SDL_GetMouseState(&x1, &y1);
        mute_hover = 0;
        if (x1 > mute_rect.x && x1 < (mute_rect.x + mute_rect.w) &&
            y1 > mute_rect.y && y1 < (mute_rect.y + mute_rect.h))
        {
            mute_hover = 1;
        }
        break;
    }
    case (SDL_KEYDOWN):
    {
        SDL_Keycode key_press = e.key.keysym.sym;
        if (key_press == SDLK_m)
        {
            if (Mix_PausedMusic() == 1)
            {
                //Resume the music
                Mix_ResumeMusic();
                mute_state = 0;
            }
            //If the music is playing
            else
            {
                //Pause the music
                Mix_PauseMusic();
                mute_state = 1;
            }
        }
    }
    }
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

    if (alive_count <= 1 && maze.close_radius <= 50)
    {
        maze.close(current_time + 3000, 100);
        if (winner == 0)
        {
            winner = temp_winner;
            if (winner == 0)
                winner = -1;
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

    if (winner == 0 || SDL_GetTicks() <= maze.last_close_time)
    {
        maze.render(renderer, surface, block_surfaces);
        for (int i = 0; i < players.size(); i++)
        {
            // if (players[i].isAlive())
            players[i].render(renderer, surface, player_surfaces);
        }
        if (bombs.size() < num_bombs)
        {
            Mix_PlayChannel(-1, explosion_sound, 0);
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
        num_bombs = bombs.size();
    }
    else if (!maze.closed)
    {
        maze.render(renderer, surface, block_surfaces);
    }
    else if (!win_screen)
    {
        Mix_HaltMusic();
        Mix_PlayChannel(-1, win_sound, 0);
        maze.render(renderer, surface, block_surfaces);
        win_screen = true;
    }
    else
    {
        vector<int> rgba = {0, 0, 0, 255};
        SDL_Color curr_color = {255, 0, 0};
        string curr_text = "Game Over!";
        SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
        surface = TTF_RenderText_Solid(game_font, curr_text.c_str(), curr_color);
        SDL_Rect curr_rect = {(width - surface->w) / 2, (height - surface->h) / 6, surface->w, surface->h};
        SDL_RenderDrawRect(renderer, &curr_rect);
        SDL_Texture *display_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderFillRect(renderer, &curr_rect);
        if (!display_texture)
        {
            // cout << "Failed to create texture" << endl;
        }
        else
        {
            SDL_RenderCopy(renderer, display_texture, nullptr, &curr_rect);
        }
        SDL_DestroyTexture(display_texture);

        int vertical_offset = (height * 65) / 144;
        int horizontal_offset = (width - players.size() * ((width * 7) / 128) - (players.size() - 1) * (2 * width) / 128) / 2;
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
            SDL_Rect rect = {horizontal_offset + i * (width)*9 / 128, vertical_offset, (width * 7) / 128, (height * 7) / 72};
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
    //SDL_RenderDrawRect(renderer, &curr_rect);
    if (mute_state == 0)
    {
        surface = mute_unmute[1];
    }
    else
    {
        surface = mute_unmute[0];
    }
    SDL_Texture *display_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!display_texture)
    {
        //cout << "Failed to create texture" << endl;
        SDL_RenderFillRect(renderer, &mute_rect);
    }
    else
    {
        SDL_RenderCopy(renderer, display_texture, nullptr, &mute_rect);
    }
    SDL_DestroyTexture(display_texture);
    //SDL_Rect curr_rect = {width - (width * 100) / 1280, (height * 8) / 10, (width * 70) / 1280, (height * 50) / 720};
    if (mute_hover == 1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &mute_rect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &mute_rect);
    }
    SDL_DestroyTexture(display_texture);
}