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

private:
    SDLNet_SocketSet sockets;
    vector<data> socketvector;
    TCPsocket server;
    char tmp[1400];
    int playernum;
    int cur_id;
    IPaddress ip;
};