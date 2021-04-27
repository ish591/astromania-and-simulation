#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

class Network
{
public:
    Network(const char *ipchar);
    ~Network();
    void send(int, int, int);
    void sendBomb();
    vector<int> recv();
    int sent = 0;

private:
    SDLNet_SocketSet server;
    TCPsocket connection;
    char tmp[4000];
    int sendctr, recvctr;
};