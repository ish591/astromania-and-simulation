#include "Network.h"
using namespace std;
Network::Network(const char *ipchar)
{
    SDLNet_Init();
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, ipchar, 1234) == -1)
        cout << "ERROR!";
    connection = SDLNet_TCP_Open(&ip);
    if (connection == NULL)
        cout << "Bad IP" << endl;
    server = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(server, connection);
}
Network::~Network()
{
    SDLNet_TCP_Close(connection);
    SDLNet_FreeSocketSet(server);
    SDLNet_Quit();
}
void Network::send(int id, int x, int y, int xoff, int yoff)
{
    sprintf(tmp, "1 %d %d %d %d %d \n", id, x, y, xoff, yoff);
    // cout << printf("%s", tmp) << endl;
    int size = 0;
    int len = strlen(tmp) + 1;
    while (size < len)
    {
        size += SDLNet_TCP_Send(connection, tmp + size, len - size);
    }
}
void Network::sendBomb()
{
    // sprintf(tmp, "1 %d \n", 6);
    // int size = 0;
    // int len = strlen(tmp) + 1;
    // while (size < len)
    // {
    //     size += SDLNet_TCP_Send(connection, tmp + size, len - size);
    // }
}
vector<int> Network::recv()
{
    while (SDLNet_CheckSockets(server, 0) > 0 && SDLNet_SocketReady(connection))
    {
        int offset = 0;
        do
        {
            offset += SDLNet_TCP_Recv(connection, tmp + offset, 1400);
            if (offset <= 0)
                return vector<int>({});
        } while (tmp[strlen(tmp) - 1] != '\n');
        vector<int> v(6, 0);
        sscanf(tmp, "%d %d %d %d %d %d", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
        // cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << " " << v[5] << endl;
        return v;
    }
    return vector<int>({});
}
