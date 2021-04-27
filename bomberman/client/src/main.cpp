#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include "Renderer.h"
#include "Client.h"
#include "OfflineGame.h"
#include "Menu.h"
using namespace std;

int player_id = 1;

vector<int> get_send_info(SDL_Event event)
{
    int up_down = -1;
    int direction = -1;
    if (event.type == SDL_KEYDOWN)
    {
        up_down = 0;
        SDL_Keycode key_press = event.key.keysym.sym;
        if (key_press == SDLK_LEFT)
            direction = 0;

        else if (key_press == SDLK_RIGHT)
            direction = 2;

        else if (key_press == SDLK_UP)
            direction = 1;

        else if (key_press == SDLK_DOWN)

            direction = 3;

        else if (key_press == SDLK_RETURN)
            direction = 4;
    }
    else if (event.type == SDL_KEYUP)
    {
        up_down = 1;
        SDL_Keycode key_press = event.key.keysym.sym;
        if (key_press == SDLK_LEFT)
            direction = 0;
        else if (key_press == SDLK_RIGHT)
            direction = 2;
        else if (key_press == SDLK_UP)
            direction = 1;
        else if (key_press == SDLK_DOWN)
            direction = 3;
    }
    return vector<int>{up_down, direction};
}
int Init()
{
    Client network("127.0.0.1");
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
    TTF_Init();
    return 1;
}
int main()
{
    Init();
    Menu menu = Menu();
    SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0); //-1 denotes its the first renderer
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!win)
    {
        cout << "Error" << endl;
        SDL_Quit();
        return 1;
    }
    SDL_Event e;
    SDL_Surface *surface = nullptr;
    Uint32 startTicks = SDL_GetTicks();
    int updates = 0;
    Renderer renderIt = Renderer();
    Uint32 prevTicks = startTicks;
    queue<vector<int>> event_queue;
    int last_event_send_time = startTicks;
    // TTF_Font *font = TTF_OpenFont("src/m5x7.ttf", 24); //24 is the font size
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    bool quit = false;
    // while (!quit)
    // {
    //     Uint32 curTicks = SDL_GetTicks();
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //     SDL_RenderClear(renderer);
    //     while (SDL_PollEvent(&e))
    //     {
    //         if (e.type == SDL_QUIT)
    //         {
    //             quit = true;
    //             break;
    //         }
    //     }
    //     menu.display(renderer, surface);
    // }

    quit = false;
    OfflineGame game(1, 7, 1280, 720);
    while (!quit)
        quit = game.run();

    // while (!quit)
    // {
    //     Uint32 curTicks = SDL_GetTicks();
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //     // if (curTicks - prevTicks > 1000)
    //     // {
    //     //     prevTicks = curTicks;
    //     //     cout << network.sent << endl;
    //     //     network.sent = 0;
    //     // }

    //     SDL_RenderClear(renderer);
    //     vector<int> v = network.recv();
    //     if (v.size() && v[0] == 0)
    //     {
    //         player_id = v[1];
    //     }
    //     renderIt.update(v);
    //     renderIt.render_all(renderer, surface);
    //     //int val = (curTicks - startTicks) / 5;

    //     while (SDL_PollEvent(&e))
    //     {
    //         if (e.type == SDL_QUIT)
    //         {
    //             quit = true;
    //             break;
    //         }
    //         //send packet to server regarding the key press
    //         vector<int> sending_info = get_send_info(e);

    //         if (sending_info[0] != -1 && sending_info[1] != -1)
    //         {
    //             event_queue.push({player_id, sending_info[0], sending_info[1]});
    //         }
    //     }
    //     if (curTicks > last_event_send_time + 50 && !(event_queue.empty()))
    //     {
    //         last_event_send_time = curTicks;
    //         network.send(event_queue.front()[0], event_queue.front()[1], event_queue.front()[2]);
    //         event_queue.pop();
    //     }
    //     SDL_RenderPresent(renderer);
    // }

    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
