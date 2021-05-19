#include "Network.h"

Network::Network()
{
    playernum = 0;
    cur_id = 0;
    SDLNet_Init();
    SDLNet_ResolveHost(&ip, NULL, 1234);
    server = SDLNet_TCP_Open(&ip);
    sockets = SDLNet_AllocSocketSet(4);
    cout << "Running on port 1234" << endl;
}

Network::~Network()
{
    for (int i = 0; i < socketvector.size(); i++)
        SDLNet_TCP_Close(socketvector[i].socket);
    SDLNet_FreeSocketSet(sockets);
    SDLNet_TCP_Close(server);
    SDLNet_Quit();
}

vector<vector<int>> Network::recv()
{
    TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
    if (tmpsocket)
    {
        SDLNet_TCP_AddSocket(sockets, tmpsocket);
        socketvector.push_back(data(tmpsocket, SDL_GetTicks(), cur_id));
        //playernum++;
        //cur_id++;
        //sprintf(tmp, "0 %d %d %d \n", cur_id, seed, maze_size);
        cout << "New connection!" << endl;
        //cout << cur_id << " " << seed << endl;
        //SDLNet_TCP_Send(tmpsocket, tmp, strlen(tmp) + 1);
        //currently don't send the player any initial info
    }

    vector<vector<int>> player_states;
    while (SDLNet_CheckSockets(sockets, 0) > 0)
    {
        for (int i = 0; i < socketvector.size(); i++)
        {
            if (SDLNet_SocketReady(socketvector[i].socket))
            {
                vector<int> v(4, 0);
                socketvector[i].timeout = SDL_GetTicks();
                int offset = 0;
                do
                {
                    offset += SDLNet_TCP_Recv(socketvector[i].socket, tmp + offset, 8000);
                    if (offset <= 0)
                        break;
                } while (tmp[strlen(tmp) - 1] != '\n');
                sscanf(tmp, "%d %d %d %d", &v[0], &v[1], &v[2], &v[3]);
                player_states.push_back(v);
                //cout << "Received: " << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << endl;
            }
        }
    }

    for (int k = 0; k < socketvector.size(); k++)
    {
        if (SDL_GetTicks() - socketvector[k].timeout > 100000)
        {
            cout << "Disconnected!" << endl;
            SDLNet_TCP_DelSocket(sockets, socketvector[k].socket);
            SDLNet_TCP_Close(socketvector[k].socket);
            socketvector.erase(socketvector.begin() + k);
            playernum--;
        }
    }
    return player_states;
}
// Client to server
// key presses, duratio

// <id> <Up or Down> <direction 0-4> <newline>

// server to client
// <type>
// 0 <id> <seed> <maze dimension> //initialization
// // info of updated maze blocks, coordinates of all players, bombs
// 1 <id> rect ..... //for each id
//     5 rect .....// for all bombs
//     6 <rect> ..... // for all exp
//since tmp is a char array
//explosion rectangles

// 2 <x> <y> <new_type> //updated block coordinates, and their type

// 3 <Kill/Life>
// 4 <game end?>

void Network::sendState(string passed)
{
    passed += "\n";
    // cout << passed << endl;
    strcpy(tmp, passed.c_str());
    //cout << socketvector.size() << endl;
    for (int k = 0; k < socketvector.size(); k++)
    {
        int size = 0;
        //cout << "Sending to " << k << passed << endl;
        int len = strlen(tmp) + 1;
        while (size < len)
        {
            size += SDLNet_TCP_Send(socketvector[k].socket, tmp, strlen(tmp) + 1);
        }
    }
    //cout << "Sending to " << 0 << passed << endl;
}
