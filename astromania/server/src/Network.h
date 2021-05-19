#pragma once

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

class Network
{
public:
    Network();
    ~Network();
    void sendState(string);
    vector<vector<int>> recv();
    int seed, maze_size;
    vector<data> socketvector;
    char tmp[8000];

private:
    SDLNet_SocketSet sockets;
    TCPsocket server;
    int playernum;
    int cur_id;
    IPaddress ip;
};