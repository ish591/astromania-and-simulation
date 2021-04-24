#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct data
{
    TCPsocket socket;
    Uint32 timeout;
    int id;
    data(TCPsocket sock, Uint32 t, int i) : socket(sock), timeout(t), id(i) {}
};

int main()
{
    int seed = time(0);
    int playernum = 0;
    int curid = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    IPaddress ip;
    SDLNet_ResolveHost(&ip, NULL, 1234);
    TCPsocket server = SDLNet_TCP_Open(&ip);
    vector<data> socketvector;
    char tmp[1400];

    SDLNet_SocketSet sockets = SDLNet_AllocSocketSet(4);
    SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 10, 10, 0);
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }
        TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
        if (tmpsocket)
        {
            SDLNet_TCP_AddSocket(sockets, tmpsocket);
            socketvector.push_back(data(tmpsocket, SDL_GetTicks(), curid));
            playernum++;
            curid++;
            sprintf(tmp, "0 %d %d %d %d %d \n", 0, 1, seed, 0, 0);
            cout << "New connection!" << endl;
            SDLNet_TCP_Send(tmpsocket, tmp, strlen(tmp) + 1);
        }

        while (SDLNet_CheckSockets(sockets, 0) > 0)
        {
            for (int i = 0; i < socketvector.size(); i++)
            {
                if (SDLNet_SocketReady(socketvector[i].socket))
                {
                    socketvector[i].timeout = SDL_GetTicks();
                    int offset = 0;
                    do
                    {
                        offset += SDLNet_TCP_Recv(socketvector[i].socket, tmp + offset, 1400);
                        if (offset <= 0)
                            break;
                    } while (tmp[strlen(tmp) - 1] != '\n');

                    int num = tmp[0] - '0';
                    int j = 1;
                    while (tmp[j] >= '0' && tmp[j] <= '9')
                    {
                        num *= 10;
                        num += tmp[j] - '0';
                        j++;
                        cout << num << endl;
                    }
                    if (num == 1)
                    {
                        for (int k = 0; k < socketvector.size(); k++)
                        {
                            if (k == i)
                                continue;
                            SDLNet_TCP_Send(socketvector[k].socket, tmp, strlen(tmp) + 1);
                        }
                    }
                    else
                    {
                        SDLNet_TCP_DelSocket(sockets, socketvector[i].socket);
                        SDLNet_TCP_Close(socketvector[i].socket);
                        socketvector.erase(socketvector.begin() + i);
                        playernum--;
                    }
                }
            }
        }

        for (int k = 0; k < socketvector.size(); k++)
        {
            if (SDL_GetTicks() - socketvector[k].timeout > 5000)
            {
                cout << "Disconnected!" << endl;
                SDLNet_TCP_DelSocket(sockets, socketvector[k].socket);
                SDLNet_TCP_Close(socketvector[k].socket);
                socketvector.erase(socketvector.begin() + k);
                playernum--;
            }
        }
        SDL_Delay(1);
    }
    for (int i = 0; i < socketvector.size(); i++)
        SDLNet_TCP_Close(socketvector[i].socket);
    SDLNet_FreeSocketSet(sockets);
    SDLNet_TCP_Close(server);
    SDLNet_Quit();
}