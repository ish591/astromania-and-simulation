#include "Bot.h"

Bot::Bot(int id) : Player(id)
{
    x_dest = 1;
    y_dest = 1;
    path = stack<int>();
}

void Bot::setDestination(Maze maze, int x, int y)
{
    x_dest = x;
    y_dest = y;
    while (!path.empty())
        path.pop();
    calculatePath(maze.getMaze());
    center();
}

void Bot::center()
{
    x_offset = block_size / 2;
    y_offset = block_size / 2;
}

void Bot::calculatePath(vector<vector<Box>> maze)
{
    if (x == x_dest && y == y_dest)
        return;
    int M = maze.size();
    int parent[M][M];
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            parent[i][j] = -1;
        }
    }
    parent[y][x] = 0;
    queue<pair<int, int>> q;
    q.push({y, x});
    while (!q.empty())
    {
        int i = q.front().first;
        int j = q.front().second;
        // cout << i << j << endl;
        if (i == y_dest && j == x_dest)
            break;

        if (i - 1 >= 0 && parent[i - 1][j] == -1 && maze[i - 1][j].get_block_type() == 0)
        {
            q.push({i - 1, j});
            parent[i - 1][j] = 1;
        }
        if (i + 1 < M && parent[i + 1][j] == -1 && maze[i + 1][j].get_block_type() == 0)
        {
            q.push({i + 1, j});
            parent[i + 1][j] = 3;
        }
        if (j - 1 >= 0 && parent[i][j - 1] == -1 && maze[i][j - 1].get_block_type() == 0)
        {
            q.push({i, j - 1});
            parent[i][j - 1] = 2;
        }
        if (j + 1 < M && parent[i][j + 1] == -1 && maze[i][j + 1].get_block_type() == 0)
        {
            q.push({i, j + 1});
            parent[i][j + 1] = 4;
        }
        q.pop();
    }
    int i = y_dest, j = x_dest;
    move_size = 1;
    while (parent[i][j])
    {
        // cout << i << j << endl;
        for (int k = 0; k < block_size; k++)
        {
            path.push(parent[i][j]);
        }
        int v = (parent[i][j] == 1) - (parent[i][j] == 3);
        int h = (parent[i][j] == 2) - (parent[i][j] == 4);
        // cout << parent[i][j] << " " << v << h << i << j << endl;
        i += v;
        j += h;
    }
    // while (!path.empty())
    // {
    //     // cout << path.top();
    //     path.pop();
    // }
}

void Bot::updateLocation(Maze maze)
{
    int hmove = 0, vmove = 0;
    if (!path.empty())
    {
        int x = path.top();
        vmove = (x == 3) - (x == 1);
        hmove = (x == 4) - (x == 2);
        path.pop();
    }
    vector<vector<Box>> a = maze.getMaze();

    switch (hmove)
    {
    case -1:
        if (x_offset - move_size - player_size / 2 > 0)
        {
            x_offset -= move_size;
        }
        else if (a[y][x - 1].get_block_type() == 0)
        {
            if (x_offset - move_size < 0)
                x--;
            x_offset = (x_offset + block_size - move_size) % block_size;
        }
        else
        {
            x_offset = player_size / 2 + 1;
        }
        break;

    case 0:
        break;

    case 1:
        if (x_offset + move_size + player_size / 2 < block_size)
        {
            x_offset += move_size;
        }
        else if (a[y][x + 1].get_block_type() == 0)
        {
            if (x_offset + move_size >= block_size)
                x++;
            x_offset = (x_offset + move_size) % block_size;
        }
        else
        {
            x_offset = block_size - 1 - player_size / 2;
        }
        break;

    default:
        hmove = 0;
        break;
    }

    switch (vmove)
    {
    case -1:
        if (y_offset - move_size - player_size / 2 > 0)
        {
            y_offset -= move_size;
        }
        else if (a[y - 1][x].get_block_type() == 0)
        {
            if (y_offset - move_size < 0)
                y--;
            y_offset = (y_offset + block_size - move_size) % block_size;
        }
        else
        {
            y_offset = player_size / 2 + 1;
        }
        break;

    case 0:
        break;

    case 1:
        if (y_offset + move_size + player_size / 2 < block_size)
        {
            y_offset += move_size;
        }
        else if (a[y + 1][x].get_block_type() == 0)
        {
            if (y_offset + move_size >= block_size)
                y++;
            y_offset = (y_offset + move_size) % block_size;
        }
        else
        {
            y_offset = block_size - 1 - player_size / 2;
        }

        break;

    default:
        vmove = 0;
        break;
    }
}

void Bot::render(SDL_Renderer *renderer, Maze maze)
{
    updateLocation(maze);
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
