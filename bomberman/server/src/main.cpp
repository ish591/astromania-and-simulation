#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include "OnlineGame.h"

using namespace std;
int main()
{
    bool quit = false;

    OnlineGame game(4, 7, 1280, 720);
    cout << "Running on port 1234" << endl;
    while (!quit)
        quit = game.run();

    return 0;
}
