#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
vector<SDL_Surface *> mute_unmute;
vector<SDL_Surface *> control_buttons;
SDL_Surface *main_screen;
SDL_Surface *background;
SDL_Surface *heart;
SDL_Surface *back_button;
Mix_Music *menu_music = NULL;
Mix_Music *game_music = NULL;
Mix_Chunk *explosion_sound = NULL;
Mix_Chunk *win_sound = NULL;
TTF_Font *curr_font;
int mute_hover = 0;
int mute_state;
int player_id = -1;
void loadTextures()
{
    string assets_dir = "../assets/";
    string pref_players = assets_dir + "images/characters/";
    string colors[] = {"red", "green", "blue", "pink", "yellow", "purple"};
    heart = IMG_Load((assets_dir + "images/heart.png").c_str());
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
    string pref_power_ups = assets_dir + "images/blocks/";
    for (int i = 0; i < 7; i++)
    {
        string image_path = pref_power_ups + to_string(i) + ".png";
        SDL_Surface *new_surface = IMG_Load(image_path.c_str());
        block_surfaces.push_back(new_surface);
    }
    string image_explosion = assets_dir + "images/explosion_horizontal.png";
    explosion_surfaces.push_back(IMG_Load(image_explosion.c_str()));
    image_explosion = assets_dir + "images/explosion_vertical.png";
    explosion_surfaces.push_back(IMG_Load(image_explosion.c_str()));

    string main_surf = assets_dir + "images/main_screen.png";
    main_screen = IMG_Load(main_surf.c_str());

    string backg = assets_dir + "images/background.png";
    background = IMG_Load(backg.c_str());

    string mute = assets_dir + "images/music_off.png";
    SDL_Surface *temp = IMG_Load(mute.c_str());
    mute_unmute.push_back(temp);

    string unmute = assets_dir + "images/music_on.png";
    temp = IMG_Load(unmute.c_str());
    mute_unmute.push_back(temp);

    string backb = assets_dir + "images/back_button.png";
    back_button = IMG_Load(backb.c_str());

    string prefOff = assets_dir + "images/instructions/offline_";
    for (int i = 0; i < 4; i++)
    {
        SDL_Surface *temp = IMG_Load((prefOff + to_string(i + 1) + ".png").c_str());
        control_buttons.push_back(temp);
    }
    temp = IMG_Load((assets_dir + "images/instructions/online.png").c_str());
    control_buttons.push_back(temp);
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

        else if (key_press == SDLK_SPACE)
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

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    menu_music = Mix_LoadMUS("../assets/sounds/menu.wav");
    if (menu_music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    // Load music
    game_music = Mix_LoadMUS("../assets/sounds/background_music.wav");
    if (game_music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    win_sound = Mix_LoadWAV("../assets/sounds/win.wav");
    if (win_sound == NULL)
    {
        printf("Failed to load win sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    explosion_sound = Mix_LoadWAV("../assets/sounds/explosion.wav");
    if (explosion_sound == NULL)
    {
        printf("Failed to load explosion sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    return success;
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
    bool res = loadMedia();
    TTF_Init();
    curr_font = TTF_OpenFont("../assets/fonts/m5x7.ttf", 200);
    return 1;
}

//The sound effects that will be used
// Mix_Chunk *collect_coin = NULL;
//Mix_Chunk *Coin::collect_coin = NULL;
void close()
{
    // //Free loaded images
    // gPromptTexture.free();

    //Free the sound effects
    // Mix_FreeChunk(Coin::collect_coin);
    // Coin::collect_coin = NULL;
    // Free the music
    Mix_FreeMusic(menu_music);
    Mix_FreeMusic(game_music);
    menu_music = NULL;
    game_music = NULL;

    //Destroy window
    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(win);
    //win = NULL;
    //renderer = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    // IMG_Quit();
    SDL_Quit();
}
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        cout << "Error: " << SDL_GetError() << endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
    Init();
    loadTextures();
    bool offline = false;
    bool online = false;
    bool game_started = false;
    bool not_ready_yet = true;
    //cout << "offline?";
    //cin >> offline;
    Menu menu = Menu(WINDOW_WIDTH, WINDOW_HEIGHT, player_surfaces, main_screen, background, mute_unmute, back_button, control_buttons, block_surfaces);
    SDL_Window *win = SDL_CreateWindow("Astromania", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
    int players;
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(menu_music, -1);
    }
    while (!quit)
    {
        Uint32 curTicks = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else
            {
                menu.HandleEvents(e);
            }
        }
        if (menu.ended())
        {
            quit = true;
            if (menu.offline_selected)
            {
                offline = true;
                players = menu.players_selected;
            }
            if (menu.online_selected)
            {
                online = true;
            }
        }
        menu.display(renderer, surface);
    }
    quit = false;
    SDL_FreeSurface(surface);
    if (offline)
    {
        //cout << "Enter number of players: ";
        //cin >> players;
        OfflineGame game(players, 7, WINDOW_WIDTH, WINDOW_HEIGHT, player_surfaces, block_surfaces, bomb_surfaces, explosion_surfaces, heart, win_sound, explosion_sound, curr_font, menu.mute_state, mute_unmute);
        int start_time = SDL_GetTicks();
        // if (Mix_PlayingMusic() == 0)
        // {
        //Play the music
        Mix_PlayMusic(game_music, -1);
        if (menu.mute_state == 1)
        {
            Mix_PauseMusic();
        }
        // }
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
    else if (online)
    {
        SDL_Rect mute_rect = {WINDOW_WIDTH - (WINDOW_WIDTH * 100) / 1280, (WINDOW_HEIGHT * 8) / 10, (WINDOW_WIDTH * 70) / 1280, (WINDOW_HEIGHT * 50) / 720};
        int mute_state = menu.mute_state;
        Client client(menu.IP_address.c_str());
        Renderer renderIt = Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, player_surfaces, block_surfaces, bomb_surfaces, explosion_surfaces, heart, win_sound, explosion_sound, curr_font);
        //this denotes that a player has joined
        client.send(5, 0, 0);
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
            if (game_started)
            {
                vector<int> v = client.recv();
                //cout << v.size() << endl;
                renderIt.update(v);
                renderIt.render_all(renderer, surface);

                while (SDL_PollEvent(&e))
                {
                    switch (e.type)
                    {
                    case (SDL_MOUSEBUTTONDOWN):
                    {
                        if (e.button.button == SDL_BUTTON_LEFT)
                        {
                            if (mute_hover == 1)
                            {
                                if (Mix_PausedMusic() == 1)
                                {
                                    //Resume the music
                                    Mix_ResumeMusic();
                                    mute_state = 0;
                                }
                                //If the music is playing
                                else
                                {
                                    //Pause the music
                                    Mix_PauseMusic();
                                    mute_state = 1;
                                }
                            }
                        }
                        break;
                    }
                    case (SDL_MOUSEMOTION):
                    {
                        int x1, y1;
                        SDL_GetMouseState(&x1, &y1);
                        mute_hover = 0;
                        if (x1 > mute_rect.x && x1 < (mute_rect.x + mute_rect.w) &&
                            y1 > mute_rect.y && y1 < (mute_rect.y + mute_rect.h))
                        {
                            mute_hover = 1;
                        }
                        break;
                    }
                    case (SDL_KEYDOWN):
                    {
                        SDL_Keycode key_press = e.key.keysym.sym;
                        if (key_press == SDLK_m)
                        {
                            if (Mix_PausedMusic() == 1)
                            {
                                //Resume the music
                                Mix_ResumeMusic();
                                mute_state = 0;
                            }
                            //If the music is playing
                            else
                            {
                                //Pause the music
                                Mix_PauseMusic();
                                mute_state = 1;
                            }
                        }
                    }
                    }
                    if (e.type == SDL_QUIT)
                    {
                        client.send(7, 0, 0);
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
                    //cout << player_id << endl;
                    //cout << "here" << endl;
                    last_event_send_time = curTicks;
                    client.send(event_queue.front()[0], event_queue.front()[1], event_queue.front()[2]);
                    event_queue.pop();
                }
                if (renderIt.winner == 0 || !renderIt.map.closed || !renderIt.winner_screen)
                {
                    if (mute_state == 0)
                    {
                        surface = mute_unmute[1];
                    }
                    else
                    {
                        surface = mute_unmute[0];
                    }
                    SDL_Texture *display_texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (!display_texture)
                    {
                        //cout << "Failed to create texture" << endl;
                        SDL_RenderFillRect(renderer, &mute_rect);
                    }
                    else
                    {
                        SDL_RenderCopy(renderer, display_texture, nullptr, &mute_rect);
                    }
                    SDL_DestroyTexture(display_texture);
                    //SDL_Rect curr_rect = {width - (width * 100) / 1280, (height * 8) / 10, (width * 70) / 1280, (height * 50) / 720};
                    if (mute_hover == 1)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                        SDL_RenderDrawRect(renderer, &mute_rect);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderDrawRect(renderer, &mute_rect);
                    }
                    SDL_DestroyTexture(display_texture);
                }
                SDL_RenderPresent(renderer);
            }
            else
            {
                //HANDLE RECIEVING PACKETS FROM SERVER AT THIS TIME FOR DISPLAY OF NUMBER OF PLAYERS
                SDL_RenderClear(renderer);
                menu.setOption(5);
                bool quit1 = false;
                while (!quit1)
                {
                    vector<int> v = client.recv();
                    if (v.size() && v[0] == 0)
                    {
                        player_id = v[1];
                        game_started = true;
                        Mix_PlayMusic(game_music, -1);
                        if (mute_state == 1)
                            Mix_PauseMusic();
                        quit1 = true;
                        renderIt.update(v);
                        //renderIt.render_all(renderer, surface);
                        break;
                        //this is player initialisation. Send only when starting
                    }
                    else if (v.size() && v[0] == 4)
                    {
                        //cout << "ok" << endl;
                        menu.total_players = v[1];
                        menu.joined_players = v[2];
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                    while (SDL_PollEvent(&e))
                    {
                        if (e.type == SDL_QUIT)
                        {
                            quit1 = true;
                            break;
                        }
                        else
                        {
                            menu.HandleEvents(e);
                            //add a ready button !!!!
                        }
                    }
                    if (menu.player_ready() && not_ready_yet)
                    {

                        //send a packet to server that I am ready
                        client.send(6, 0, 0);
                        not_ready_yet = false;
                        //only send a packet the first time
                    }
                    if (!menu.player_ready() && !not_ready_yet)
                    {
                        //if pressed ready button again, but was ready earlier. Send a packet that you are not ready.
                        //but haven't left.
                        //type 8.
                        client.send(8, 0, 0);
                        not_ready_yet = true;
                    }

                    menu.display(renderer, surface);
                }
            }
        }
    }
    // SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
