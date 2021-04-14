#include <iostream>
#include <SDL2/SDL.h>

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
    Maze maze(n, 1280, 720, fl);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!win)
    {
        cout << "Error" << endl;
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    // Player player(1);
    // player.updateDimensions(maze, 1280, 720);
    Bot bot(2);
    bot.updateDimensions(maze, 1280, 720);
    bot.setDestination(maze, 2 * n - 1, 2 * n - 1);
    while (!quit)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // bot.updateLocation(maze);
        maze.render(renderer);
        bot.render(renderer, maze);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            // player.takeAction(e);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
