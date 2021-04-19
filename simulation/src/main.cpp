#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Maze.h"
#include "Player.h"
#include "Bot.h"

using namespace std;
int main()
{
    int n, fl;
    cout << "Enter maze dimension: ";
    cin >> n;
    cout << "Enter 0 for continuous walls, 1 for discrete:";
    cin >> fl;
    cout << "Enter game name: ";
    //PACMAN OR BOMBERMAN. In case of Pacman, we will use the same generated maze, but with some loops
    Maze maze(n, 1280, 720, fl);
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
        SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0); //-1 denotes its the first renderer
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        if (!win)
        {
            cout << "Error" << endl;
            //SDL_Quit();
            success = false;
        }
        if (!success)
        {
            return 1;
        }
        bool quit = false;
        SDL_Event e;

        Bot bot(2);
        bot.updateDimensions(maze, 1280, 720);
        bot.setDestination(maze, 2 * n - 1, 2 * n - 1);
        Uint32 startTicks = SDL_GetTicks();
        int slow_factor = 10;
        int updates = 0;
        while (!quit)
        {
            Uint32 curTicks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            int val = (curTicks - startTicks) / slow_factor;
            for (; updates < val; updates++)
            {
                bot.updateLocation(maze);
            }

            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_z)
                    {
                        slow_factor += slow_factor / 2;
                        if (slow_factor > 100)
                            slow_factor = 100;
                        startTicks = curTicks;
                        updates = 0;
                        cout << slow_factor << endl;
                    }
                    if (e.key.keysym.sym == SDLK_x)
                    {
                        slow_factor -= slow_factor / 3;
                        startTicks = curTicks;
                        updates = 0;
                        cout << slow_factor << endl;
                    }
                }
            }
            maze.render(renderer);
            bot.render(renderer);
            SDL_RenderPresent(renderer);
        }
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 0;
    }
}
