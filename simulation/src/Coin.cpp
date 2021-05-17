#include "Coin.h"

Coin::Coin(int N, int block_size, int left_offset, int top_offset, SDL_Surface *coin_surface)
{
    srand(time(0));
    Coin::left_offset = left_offset;
    Coin::top_offset = top_offset;
    Coin::block_size = block_size;
    Coin::N = N;
    collected = false;
    x = -1;
    y = -1;
    coin_size = max(2, block_size - 5);
    Coin::coin_surface = coin_surface;
}

void Coin::setLocation(vector<Coin> coins)
{

    bool repeat = true;
    while (repeat)
    {
        x = 2 * (rand() % N) + 1;
        y = 2 * (rand() % N) + 1;
        repeat = false;
        for (int i = 0; i < coins.size(); i++)
        {
            if (coins[i].getCoordinates().first == x && coins[i].getCoordinates().second == y)
                repeat = true;
        }
    }
}

pair<int, int> Coin::getCoordinates()
{
    return {x, y};
}

void Coin::collect()
{
    if (!collected)
    {
        collected = true;
        //cout << "COLLECTED" << endl;
        Mix_PlayChannel(-1, Coin::collect_coin, 0);
    }
}

void Coin::render(SDL_Renderer *renderer, SDL_Surface *surface)
{
    SDL_Rect rect;
    rect = {left_offset + x * block_size + (block_size - coin_size) / 2, top_offset + y * block_size + (block_size - coin_size) / 2, coin_size, coin_size};

    surface = (coin_surface);
    if (!surface)
    {
        // cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!curr_texture)
    {
        // cout << "Failed to create texture" << endl;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        if (!collected)
        {
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
    else
    {
        if (!collected)
            SDL_RenderCopy(renderer, curr_texture, nullptr, &rect);
        else
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
    SDL_DestroyTexture(curr_texture);
}
