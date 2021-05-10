#include "Renderer.h"

Renderer::Renderer(int w, int h, vector<vector<SDL_Surface *>> player_surfaces, vector<SDL_Surface *> block_surfaces, vector<SDL_Surface *> bomb_surfaces, vector<SDL_Surface *> explosion_surfaces, SDL_Surface *heart)
{
    Renderer::player_surfaces = player_surfaces;
    Renderer::block_surfaces = block_surfaces;
    Renderer::bomb_surfaces = bomb_surfaces;
    Renderer::explosion_surfaces = explosion_surfaces;
    Renderer::heart = heart;
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    //cout << "HELLO" << endl;
    winner_screen = false;
}

void Renderer::update(vector<int> obj)
{

    if (obj.empty())
        return;
    switch (obj[0])
    {
    case 0:
        if (map.getSize() < 4)
            map = Map(obj[3], 1, WINDOW_WIDTH, WINDOW_HEIGHT, obj[2]);
        break;
    case 1:
        players.clear();
        bombs.clear();
        explosions.clear();
        for (int i = 1; i < obj.size(); i += 6)
        {
            int x = obj[i + 1];
            int y = obj[i + 2];
            int w = obj[i + 3];
            int h = obj[i + 4];
            if (x >= 0 && y >= 0 && w >= 0 && h >= 0)
            {
                if (obj[i] <= 4)
                {
                    players.push_back({{obj[i], obj[i + 5]}, SDL_Rect({x, y, w, h})});
                    if (lives.size() < obj[i])
                    {
                        lives.push_back(obj[i + 6]);
                    }
                    else
                    {
                        lives[obj[i] - 1] = obj[i + 6];
                    }
                    i += 2;
                }
                else if (obj[i] == 5)
                {
                    bombs.push_back(SDL_Rect({x, y, w, h}));
                }
                else if (obj[i] == 6)
                {
                    explosions.push_back(SDL_Rect({x, y, w, h}));
                }
            }
        }
        break;
    case 2:
        for (int i = 1; i < obj.size(); i += 3)
        {
            int x = obj[i];
            int y = obj[i + 1];
            int t = obj[i + 2];
            // cout << x << " " << y << " " << t << endl;
            if (x >= 0 && y >= 0 && x < map.getSize() && y < map.getSize() && t >= 0 && t <= 6)
                map.update(x, y, t);
        }

        break;
    case 3:
        map.close(SDL_GetTicks(), 100);
        if (winner == 0)
        {
            rem_lives = obj[3];
            number_of_players = obj[1];
            winner = obj[2];
        }
        break;
    }
}

void Renderer::render_all(SDL_Renderer *renderer, SDL_Surface *surface)
{
    if (winner == 0)
    {
        map.render(renderer, surface, block_surfaces);
        for (int i = 0; i < players.size(); i++)
        {
            render_player(renderer, surface, players[i].first.first, players[i].first.second, players[i].second);
        }

        for (int i = 0; i < bombs.size(); i++)
        {
            render_bomb(renderer, surface, bombs[i]);
        }

        for (int i = 0; i < explosions.size(); i++)
        {
            render_explosion(renderer, surface, explosions[i]);
        }
    }
    else if (!map.closed)
    {
        map.update(SDL_GetTicks());
        map.render(renderer, surface, block_surfaces);
    }
    else if (!winner_screen)
    {
        map.update(SDL_GetTicks());
        map.render(renderer, surface, block_surfaces);
        SDL_Delay(1000);
        winner_screen = true;
    }
    else
    {
        for (int i = 0; i < number_of_players; i++)
        {
            if (i + 1 == winner)
            {
                surface = (player_surfaces[i][0]);
            }
            else
            {
                surface = (player_surfaces[i][2]);
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

void Renderer::render_player(SDL_Renderer *renderer, SDL_Surface *surface, int player_id, int player_state, SDL_Rect rect)
{
    surface = (player_surfaces[player_id - 1][player_state - 2]);
    if (!surface)
    {
        cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);

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
void Renderer::render_bomb(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Rect rect)
{
    surface = (bomb_surfaces[0]);
    if (!surface)
    {
        cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!curr_texture)
    {
        cout << "Failed to create texture" << endl;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderCopy(renderer, curr_texture, nullptr, &rect);
    }
    SDL_DestroyTexture(curr_texture);
}
void Renderer::render_explosion(SDL_Renderer *renderer, SDL_Surface *surface, SDL_Rect rect)
{
    if (rect.w > rect.h)
        surface = explosion_surfaces[0];
    else
        surface = explosion_surfaces[1];

    if (!surface)
    {
        cout << "Failed to create surface" << endl;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
        SDL_DestroyTexture(curr_text);
    }
}