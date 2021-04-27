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
    cout << "Enter Mode(0 for offline, 1 for online): " << endl;
    cin >> online;

    bool quit = false;

    Uint32 startTicks = SDL_GetTicks();
    int updates = 0;
    if (online)
    {
        OnlineGame game(4, 7, 640, 480);
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
