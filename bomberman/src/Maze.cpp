#include "Maze.h"

Maze::Maze(int N, bool discrete_walls, int width, int height)
{
    generate(N);
    int x = min(width, height);
    Maze::discrete_walls = discrete_walls;
    block_size = x / M - discrete_walls;
    left_offset = (width - block_size * M) / 2;
    top_offset = (height - block_size * M) / 2;
}

void Maze::generate(int N)
{
    // Using DFS to generate a line boundary maze and then convert it to block walls
    Maze::N = N;
    M = 2 * N + 1;
    maze.clear();
    maze = vector<vector<Box>>();
    for (int i = 0; i < M; i++)
    {
        vector<Box> temp;
        for (int j = 0; j < M; j++)
        {
            Box new_box = Box(0);
            temp.push_back(new_box);
        }
        maze.push_back(temp);
    }
    int a[N][N][5];

    srand(time(0));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < 5; k++)
                a[i][j][k] = 0;
        }
    }
    int par = 50;
    stack<pair<int, int>> st;
    st.push({0, 0});

    while (!st.empty())
    {
        int i = st.top().first;
        int j = st.top().second;
        int x[] = {0, 0, 0, 0};
        int ct = 0;
        a[i][j][0] = 1;
        if (i - 1 >= 0 && a[i - 1][j][0] == 0)
        {
            x[ct++] = 1;
        }
        if (j - 1 >= 0 && a[i][j - 1][0] == 0)
        {
            x[ct++] = 2;
        }
        if (i + 1 < N && a[i + 1][j][0] == 0)
        {
            x[ct++] = 3;
        }
        if (j + 1 < N && a[i][j + 1][0] == 0)
        {
            x[ct++] = 4;
        }

        if (ct == 0)
        {
            st.pop();
            continue;
        }

        int r = rand() % ct;
        int l = x[r];
        a[i][j][l] = 1;

        if (l == 1)
        {
            st.push({i - 1, j});
            a[i - 1][j][3] = 1;
        }
        else if (l == 2)
        {
            st.push({i, j - 1});
            a[i][j - 1][4] = 1;
        }
        else if (l == 3)
        {
            st.push({i + 1, j});
            a[i + 1][j][1] = 1;
        }
        else
        {
            st.push({i, j + 1});
            a[i][j + 1][2] = 1;
        }
    }

    // Make block walls
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int x = 2 * i + 1, y = 2 * j + 1;
            maze[x][y].update(0);
            maze[x - 1][y].update(1 - a[i][j][1]);
            maze[x][y - 1].update(1 - a[i][j][2]);
            maze[x + 1][y].update(1 - a[i][j][3]);
            maze[x][y + 1].update(1 - a[i][j][4]);
            maze[x - 1][y - 1].update(2);
            maze[x + 1][y - 1].update(2);
            maze[x - 1][y + 1].update(2);
            maze[x + 1][y + 1].update(2);
        }
    }
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (i == 0 || j == 0 || i == M - 1 || j == M - 1)
            {
                maze[i][j].update(2);
            }
        }
    }
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         cout << a[i][j][0] << a[i][j][1] << a[i][j][2] << a[i][j][3] << a[i][j][4] << " ";
    //     }
    //     cout << endl;
    // }
}

void Maze::update(int i, int j, int new_type)
{
    maze[i][j].update(new_type);
}

vector<vector<Box>> Maze::getMaze()
{
    return maze;
}

int Maze::getSize()
{
    return M;
}

void Maze::print()
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {

            cout << maze[i][j].get_block_type();
        }
        cout << endl;
    }
}

void Maze::render(SDL_Renderer *renderer)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            maze[i][j].render(renderer, j * (block_size) + left_offset, i * (block_size) + top_offset, block_size - discrete_walls, block_size - discrete_walls);
        }
    }
}

int Maze::getBlockSize()
{
    return block_size;
}