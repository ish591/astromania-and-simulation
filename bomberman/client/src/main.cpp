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

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace std;

vector<vector<SDL_Surface *>> player_surfaces;
vector<SDL_Surface *> block_surfaces;
vector<SDL_Surface *> bomb_surfaces;
vector<SDL_Surface *> explosion_surfaces;

int player_id = -1;
void loadTextures()
{
    string assets_dir = "../assets/";
    string pref_players = assets_dir + "images/characters/";
    string colors[] = {"red", "green", "blue", "pink", "yellow", "purple"};
    for (int i = 0; i < 6; i++)
    {
        vector<SDL_Surface *> curr;
        string image_path = pref_players + colors[i];
        // string front_im = image_path + "_front.png";
        // SDL_Surface *new_surface = IMG_Load(front_im.c_str());
        // curr.push_back(new_surface);
        // string back_im = image_path + "_back.png";
        // SDL_Surface *new_surface1 = IMG_Load(back_im.c_str());
        // curr.push_back(new_surface1);
        string left_im = image_path + "_left.png";
        SDL_Surface *new_surface2 = IMG_Load(left_im.c_str());
        curr.push_back(new_surface2);
        string right_im = image_path + "_right.png";
        SDL_Surface *new_surface3 = IMG_Load(right_im.c_str());
        curr.push_back(new_surface3);
        string dead_im = image_path + "_dead.png";
        SDL_Surface *new_surface4 = IMG_Load(dead_im.c_str());
        curr.push_back(new_surface4);
        player_surfaces.push_back(curr);
    }
    string pref_bombs = assets_dir + "images/bomb";
    for (int i = 0; i < 1; i++)
    {
        string image_path = pref_bombs + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        bomb_surfaces.push_back(new_surface);
    }
    string pref_power_ups = assets_dir + "images/powerups/";
    for (int i = 0; i < 4; i++)
    {
        string image_path = pref_power_ups + to_string(i + 1) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        block_surfaces.push_back(new_surface);
    }
    string image_explosion = assets_dir + "images/explosion_horizontal.png";
    explosion_surfaces.push_back(IMG_Load(image_explosion.c_str()));
    image_explosion = assets_dir + "images/explosion_vertical.png";
    explosion_surfaces.push_back(IMG_Load(image_explosion.c_str()));
}

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
    loadTextures();
    int offline;
    cout << "offline?";
    cin >> offline;
    Menu menu = Menu();
    SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    if (offline)
    {
        int players;
        cout << "Enter number of players: ";
        cin >> players;
        OfflineGame game(players, 7, WINDOW_WIDTH, WINDOW_HEIGHT, player_surfaces, block_surfaces, bomb_surfaces, explosion_surfaces);
        int start_time = SDL_GetTicks();
        while (!quit)
        {
            Uint32 curTicks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            game.render(renderer, surface);
            int val = (curTicks - start_time) / 5;
            for (; updates < val; updates++)
            {
                game.update(curTicks - (val - updates + 1) * 5);
            }

            while (SDL_PollEvent(&e))
            {

                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }

                game.control(e, curTicks);
            }
            SDL_RenderPresent(renderer);
        }
    }
    else
    {
        Client client("127.0.0.1");
        Renderer renderIt = Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, player_surfaces, block_surfaces, bomb_surfaces, explosion_surfaces);
        while (!quit)
        {
            Uint32 curTicks = SDL_GetTicks();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if (curTicks - prevTicks > 1000)
            {
                prevTicks = curTicks;
                // cout << client.sent << endl;
                client.sent = 0;
            }

            SDL_RenderClear(renderer);
            vector<int> v = client.recv();
            if (v.size() && v[0] == 0)
            {
                player_id = v[1];
            }
            renderIt.update(v);
            renderIt.render_all(renderer, surface);

            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    break;
                }
                //send packet to server regarding the key press
                vector<int> sending_info = get_send_info(e);

                if (sending_info[0] != -1 && sending_info[1] != -1)
                {
                    event_queue.push({player_id, sending_info[0], sending_info[1]});
                }
            }
            if (curTicks > last_event_send_time + 50 && !(event_queue.empty()))
            {
                last_event_send_time = curTicks;
                client.send(event_queue.front()[0], event_queue.front()[1], event_queue.front()[2]);
                event_queue.pop();
            }
            SDL_RenderPresent(renderer);
        }
    }

    // SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
