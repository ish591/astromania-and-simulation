#include "Enemy.h"

Enemy::Enemy(int id)
{
    player_id = id;
    x_offset = 0, y_offset = 0;
    x = 0, y = 0;
    color_r = 255, color_g = 255, color_b = 255;
}

void Enemy::updateDimensions(Maze &maze, int w, int h)
{
    block_size = maze.getBlockSize();
    player_size = block_size - 15;
    if (player_size % 2 == 1)
        player_size--;

    left_offset = maze.left_offset;
    top_offset = maze.top_offset;
    x_offset = block_size / 2;
    y_offset = block_size / 2;
}

void Enemy::render(SDL_Renderer *renderer, SDL_Surface *surface, vector<vector<SDL_Surface *>> &player_surfaces)
{
    surface = (player_surfaces[0][0]);
    if (!surface)
    {
        cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};

    if (!curr_text)
    {
        cout << "Failed to create texture" << endl;
        SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
    }

    SDL_DestroyTexture(curr_text);
}