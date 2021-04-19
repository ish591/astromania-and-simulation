#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Maze.h"
#include "Player.h"
#include "Bot.h"
#include "Coin.h"

using namespace std;

//The window we'll be rendering to
SDL_Window *win = NULL;

//The window renderer
SDL_Renderer *renderer = NULL;

// //Scene texture
// LTexture gPromptTexture;

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
// Mix_Chunk *collect_coin = NULL;
Mix_Chunk *Coin::collect_coin = NULL;

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    // Load music
    gMusic = Mix_LoadMUS("sounds/beat.wav");
    if (gMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Load sound effects
    Coin::collect_coin = Mix_LoadWAV("sounds/coin.wav");
    if (Coin::collect_coin == NULL)
    {
        printf("Failed to load coin sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    return success;
}

void close()
{
    // //Free loaded images
    // gPromptTexture.free();

    //Free the sound effects
    // Mix_FreeChunk(Coin::collect_coin);
    // Coin::collect_coin = NULL;
    // Free the music
    Mix_FreeMusic(gMusic);
    gMusic = NULL;

    //Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    win = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    // IMG_Quit();
    SDL_Quit();
}

int main()
{
    int n, fl;
    cout << "Enter maze dimension: ";
    cin >> n;
    // cout << "Enter 0 for continuous walls, 1 for discrete:";
    // cin >> fl;
    Maze maze(n, 1280, 720, fl);
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
        success = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else
    {
        win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
        renderer = SDL_CreateRenderer(win, -1, 0); //-1 denotes its the first renderer
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

        vector<Coin> coins;
        for (int i = 0; i < 6; i++)
        {
            coins.push_back(Coin(n, maze.getBlockSize(), maze.left_offset, maze.top_offset));
            coins[i].setLocation(coins);
        }

        Bot bot(2);
        bot.updateDimensions(maze, 1280, 720);
        bot.setDestination(maze, 2 * n - 1, 2 * n - 1);

        Uint32 startTicks = SDL_GetTicks();
        int slow_factor = 10;
        int updates = 0;
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        while (!quit)
        {
            Uint32 curTicks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            int val = (curTicks - startTicks) / slow_factor;
            for (; updates < val; updates++)
            {
                bot.updateLocation(maze);
                bot.checkCoinCollection(coins);
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
                        // Mix_PlayChannel(-1, Coin::collect_coin, 0);
                    }
                }
            }
            for (int i = 0; i < coins.size(); i++)
            {
                coins[i].render(renderer);
            }
            maze.render(renderer);
            bot.render(renderer);

            SDL_RenderPresent(renderer);
        }

        close();
    }
    return 0;
}
