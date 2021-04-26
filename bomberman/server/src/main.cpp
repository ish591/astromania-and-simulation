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
    bool online = true;
    // cout << "Enter Mode(0 for offline, 1 for online): " << endl;
    // cin >> online;

    // if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    //     {
    //     cout << "Error: " << SDL_GetError() << endl;
    //     return 1;
    // }
    // if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    // {
    //     cout << " Image failed to be initialised" << SDL_GetError() << endl;
    //     return 1;
    // }
    // SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    // SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0); //-1 denotes its the first renderer
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // if (!win)
    // {
    //     cout << "Error" << endl;
    //     SDL_Quit();
    //     return 1;
    // }

    bool quit = false;
    // SDL_Surface *surface = nullptr;
    Uint32 startTicks = SDL_GetTicks();
    int updates = 0;
    if (online)
    {
        OnlineGame game(1, 7, 1280, 720);
        while (!quit)
            quit = game.run();
    }

    else
    {
        OfflineGame game(1, 7, 1280, 720);
        while (!quit)
            quit = game.run();
    }
    return 0;
}
