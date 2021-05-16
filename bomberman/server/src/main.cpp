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
    int total_players = 0;
    int joined_so_far = 0;
    //sending ready/start signals only once
    Network network_init;
    while (!quit)
    {
        vector<vector<int>> actions = network_init.recv();
        //cout << joined_so_far << endl;
        for (int i = 0; i < actions.size(); i++)
        {
            //cout << actions[i][0] << endl;
            if (actions[i][1] == 5)
            {
                total_players++; //added players
                //cout << "ok1" << endl;
            }
            else if (actions[i][1] == 6)
            {
                joined_so_far++; //player is ready
                //cout << "ok2" << endl;
            }
            else if (actions[i][1] == 7)
            {
                //a player has left
                total_players--;
                joined_so_far--;
            }
            else if (actions[i][1] == 8)
            {
                joined_so_far--; //player has unreadied
            }
        }
        if (total_players == joined_so_far && total_players > 0)
        {
            //cout << total_players << endl;
            quit = true;
        }
    }

    OnlineGame game(total_players, 7, 1280, 720, network_init);
    cout << "Running on port 1234" << endl;
    quit = false;
    while (!quit)
    {
        quit = game.run();
    }

    return 0;
}
