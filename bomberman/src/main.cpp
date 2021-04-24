#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include "OfflineGame.h"
#include "OnlineGame.h"

using namespace std;
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        cout << " Image failed to be initialised" << SDL_GetError() << endl;
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
    SDL_Surface *surface = nullptr;
    Uint32 startTicks = SDL_GetTicks();
    int updates = 0;

    bool online;
    cout << "Enter Mode(0 for offline, 1 for online): " << endl;
    cin >> online;
    if (online)
    {
        OnlineGame current_game = OnlineGame(1, 7, 1280, 720);
        while (!quit)
        {
            Uint32 curTicks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            current_game.render(renderer, surface);
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
                    break;
                }

                current_game.control(e, curTicks);
            }
            SDL_RenderPresent(renderer);
        }
    }
    else
    {
        OfflineGame current_game = OfflineGame(1, 7, 1280, 720);
        while (!quit)
        {
            Uint32 curTicks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            current_game.render(renderer, surface);
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
                    break;
                }

                current_game.control(e, curTicks);
            }
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
