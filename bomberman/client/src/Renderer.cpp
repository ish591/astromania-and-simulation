#include "Renderer.h"

Renderer::Renderer()
{
    cout << "HELLO" << endl;
    loadTextures();
}
void Renderer::loadTextures()
{
    string assets_dir = "../assets/";
    string pref_players = assets_dir + "images/player";
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
    string pref_bombs = assets_dir + "images/bomb";
    for (int i = 0; i < 1; i++)
    {
        string image_path = pref_bombs + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        bomb_surfaces.push_back(new_surface);
    }
    string pref_power_ups = assets_dir + "images/power_ups";
    for (int i = 0; i < 4; i++)
    {
        string image_path = pref_power_ups + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        block_surfaces.push_back(new_surface);
    }
    string image_explosion = assets_dir + "images/explosion.png";
    explosion_surface = IMG_Load(image_explosion.c_str());
}

void Renderer::update(vector<int> obj)
{
    if (obj.empty())
        return;
    switch (obj[0])
    {
    case 0:
        if (maze.getSize() < 4)
            maze = Maze(obj[3], 1, 1280, 720, obj[2]);
        break;
    case 1:
        players.clear();
        bombs.clear();
        explosions.clear();
        for (int i = 1; i < obj.size(); i += 5)
        {
            int x = obj[i + 1];
            int y = obj[i + 2];
            int w = obj[i + 3];
            int h = obj[i + 4];
            if (x >= 0 && y >= 0 && w >= 0 && h >= 0)
            {
                if (obj[i] < 4)
                {
                    players.push_back({obj[i], SDL_Rect({x, y, w, h})});
                }
                if (obj[i] == 5)
                {
                    bombs.push_back(SDL_Rect({x, y, w, h}));
                }
                if (obj[i] == 6)
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
            if (x >= 0 && y >= 0 && x < maze.getSize() && y < maze.getSize() && t >= 0 && t <= 6)
                maze.update(x, y, t);
        }

        break;
    }
}

void Renderer::render_all(SDL_Renderer *renderer, SDL_Surface *surface)
{

    maze.render(renderer, surface, block_surfaces);
    for (int i = 0; i < players.size(); i++)
    {
        render_player(renderer, surface, players[i].first, players[i].second);
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

void Renderer::render_player(SDL_Renderer *renderer, SDL_Surface *surface, int player_id, SDL_Rect rect)
{
    surface = (player_surfaces[0][0]);
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
    surface = explosion_surface;
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