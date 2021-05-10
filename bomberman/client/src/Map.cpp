#include "Map.h"

Map::Map(int N, bool discrete_walls, int width, int height, int seed)
{
    generate(N, seed);
    int x = min(width, height);
    Map::discrete_walls = discrete_walls;
    block_size = x / M - discrete_walls;
    left_offset = (width - block_size * M) / 2;
    top_offset = (height - block_size * M) / 2;
    close_direction = 0;
    last_close = {1, 0};
    close_radius = 0;
    last_close_time = 0;
}

// void Map::generate(int N)
// {
// Using DFS to generate a line boundary map and then convert it to block walls
// Map::N = N;
// M = 2 * N + 1;
// map.clear();
// map = vector<vector<Block> >();
// for (int i = 0; i < M; i++)
// {
//     vector<Block> temp;
//     for (int j = 0; j < M; j++)
//     {
//         Block new_box = Block(0, i, j);
//         temp.push_back(new_box);
//     }
//     map.push_back(temp);
// }
// int a[N][N][5];

// srand(time(0));

// for (int i = 0; i < N; i++)
// {
//     for (int j = 0; j < N; j++)
//     {
//         for (int k = 0; k < 5; k++)
//             a[i][j][k] = 0;
//     }
// }
// int par = 50;
// stack<pair<int, int> > st;
// st.push({0, 0});

// while (!st.empty())
// {
//     int i = st.top().first;
//     int j = st.top().second;
//     int x[] = {0, 0, 0, 0};
//     int ct = 0;
//     a[i][j][0] = 1;
//     if (i - 1 >= 0 && a[i - 1][j][0] == 0)
//     {
//         x[ct++] = 1;
//     }
//     if (j - 1 >= 0 && a[i][j - 1][0] == 0)
//     {
//         x[ct++] = 2;
//     }
//     if (i + 1 < N && a[i + 1][j][0] == 0)
//     {
//         x[ct++] = 3;
//     }
//     if (j + 1 < N && a[i][j + 1][0] == 0)
//     {
//         x[ct++] = 4;
//     }

//     if (ct == 0)
//     {
//         st.pop();
//         continue;
//     }

//     int r = rand() % ct;
//     int l = x[r];
//     a[i][j][l] = 1;

//     if (l == 1)
//     {
//         st.push({i - 1, j});
//         a[i - 1][j][3] = 1;
//     }
//     else if (l == 2)
//     {
//         st.push({i, j - 1});
//         a[i][j - 1][4] = 1;
//     }
//     else if (l == 3)
//     {
//         st.push({i + 1, j});
//         a[i + 1][j][1] = 1;
//     }
//     else
//     {
//         st.push({i, j + 1});
//         a[i][j + 1][2] = 1;
//     }
// }

// // Make block walls
// for (int i = 0; i < N; i++)
// {
//     for (int j = 0; j < N; j++)
//     {
//         int x = 2 * i + 1, y = 2 * j + 1;
//         map[x][y].update(0, -1);
//         map[x - 1][y].update(1 - a[i][j][1], -1);
//         map[x][y - 1].update(1 - a[i][j][2], -1);
//         map[x + 1][y].update(1 - a[i][j][3], -1);
//         map[x][y + 1].update(1 - a[i][j][4], -1);
//         map[x - 1][y - 1].update(2, -1);
//         map[x + 1][y - 1].update(2, -1);
//         map[x - 1][y + 1].update(2, -1);
//         map[x + 1][y + 1].update(2, -1);
//     }
// }
// for (int i = 0; i < M; i++)
// {
//     for (int j = 0; j < M; j++)
//     {
//         if (i == 0 || j == 0 || i == M - 1 || j == M - 1)
//         {
//             map[i][j].update(2, -1);
//         }
//     }
// }
// // for (int i = 0; i < N; i++)
// // {
// //     for (int j = 0; j < N; j++)
// //     {
// //         cout << a[i][j][0] << a[i][j][1] << a[i][j][2] << a[i][j][3] << a[i][j][4] << " ";
// //     }
// //     cout << endl;
// // }
// }
void Map::generate(int N, int seed)
{
    Map::N = N;
    M = 2 * N + 1;
    map.clear();
    map = vector<vector<Block>>();
    srand(seed);
    randomiser = PRG(seed);
    for (int i = 0; i < M; i++)
    {
        vector<Block> temp;
        for (int j = 0; j < M; j++)
        {
            Block new_box = Block(0, i, j);
            temp.push_back(new_box);
        }
        map.push_back(temp);
    }

    for (int i = 0; i < M; i++)
    {
        map[i][M - 1].update(2);
        map[0][i].update(2);
        map[i][0].update(2);
        map[M - 1][i].update(2);
    }
    for (int i = 1; i < M - 1; i++)
    {
        vector<Block> temp;

        for (int j = 1; j < M - 1; j++)
        {
            if (i % 2 == 0 && j % 2 == 0)
                map[i][j].update(2);
            else
                map[i][j].update(randomiser.generate() % 2);
        }
    }
    map[1][1].update(0);
    map[1][2].update(0);
    map[2][1].update(0);
    map[1][M - 2].update(0);
    map[1][M - 3].update(0);
    map[2][M - 2].update(0);
    map[M - 2][1].update(0);
    map[M - 2][2].update(0);
    map[M - 3][1].update(0);
    map[M - 2][M - 3].update(0);
    map[M - 2][M - 2].update(0);
    map[M - 3][M - 2].update(0);
}

void Map::update(int i, int j, int new_type)
{
    if (map[i][j].get_block_type() != 2)
        map[i][j].update(new_type);
}

vector<vector<Block>> Map::getMap()
{
    return map;
}

int Map::getSize()
{
    return M;
}

void Map::print()
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {

            cout << map[i][j].get_block_type();
        }
        cout << endl;
    }
}

void Map::render(SDL_Renderer *renderer, SDL_Surface *surface, vector<SDL_Surface *> &block_surfaces)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            map[i][j].render(renderer, j * (block_size) + left_offset, i * (block_size) + top_offset, block_size - discrete_walls, block_size - discrete_walls, surface, block_surfaces);
        }
    }
}

int Map::getBlockSize()
{
    return block_size;
}

void Map::update(int cur_time)
{
    int delay = 20;

    for (int i = last_close_time; i < cur_time; i += delay)
    {
        if (close_direction / 4 < close_radius && close_direction < 2 * M - 5)
        {
            if (close_direction % 4 == 0)
            {
                map[last_close.first][++last_close.second].update(2);
                if (last_close.first + last_close.second == M - 1)
                    close_direction++;
            }
            if (close_direction % 4 == 1)
            {
                map[++last_close.first][last_close.second].update(2);
                if (last_close.first == last_close.second)
                    close_direction++;
            }
            if (close_direction % 4 == 2)
            {
                map[last_close.first][--last_close.second].update(2);
                if (last_close.first + last_close.second == M - 1)
                    close_direction++;
            }
            if (close_direction % 4 == 3)
            {
                map[--last_close.first][last_close.second].update(2);
                if (last_close.first - last_close.second == 1)
                {
                    close_direction++;
                    close_radius--;
                }
            }
        }
    }
    last_close_time = cur_time;
    if (close_direction == 2 * M - 5)
    {
        closed = true;
    }
}
void Map::close(int cur_time, int r)
{
    last_close_time = cur_time;
    close_radius = r;
}