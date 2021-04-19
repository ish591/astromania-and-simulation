#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Game.h"

using namespace std;
int main()
{
    int n, fl;
    cout << "Enter maze dimension: ";
    cin >> n;
    cout << "Enter 0 for continuous walls, 1 for discrete:";
    cin >> fl;
    cout << "Enter game name: ";
    Game current_game = Game(1, 7, 1280, 720);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0); //-1 denotes its the first renderer
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!win)
    {
        cout << "Error" << endl;
        SDL_Quit();
        return 1;
    }
    bool quit = false;
    SDL_Event e;

    Uint32 startTicks = SDL_GetTicks();
    int updates = 0;
    while (!quit)
    {
        Uint32 curTicks = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        current_game.render(renderer);
        int val = (curTicks - startTicks) / 5;
        for (; updates < val; updates++)
        {
            current_game.update(curTicks - (val - updates + 1) * 5);
        }
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            current_game.control(e, curTicks);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
