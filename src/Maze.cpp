#include "Maze.h"

Maze::Maze()
{
    generate(5);
    N = 5;
    return;
}

void Maze::generate(int N)
{
    // Using DFS to generate a line boundary maze and then convert it to block walls
    Maze::N = N;
    int M = 2 * N + 1;
    maze.clear();
    maze = vector<vector<int>>(M, vector<int>(M, 0));
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
            maze[x][y] = 0;
            maze[x - 1][y] = 1 - a[i][j][1];
            maze[x][y - 1] = 1 - a[i][j][2];
            maze[x + 1][y] = 1 - a[i][j][3];
            maze[x][y + 1] = 1 - a[i][j][4];
            maze[x - 1][y - 1] = 1;
            maze[x + 1][y - 1] = 1;
            maze[x - 1][y + 1] = 1;
            maze[x + 1][y + 1] = 1;
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

void update(vector<pair<int, int>>)
{
    return;
}

vector<vector<int>> Maze::getMaze()
{
    return maze;
}

int Maze::getSize()
{
    return (2 * N + 1);
}

void Maze::print()
{
    int M = 2 * N + 1;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            cout << maze[i][j];
        }
        cout << endl;
    }
}

void Maze::render(SDL_Renderer *renderer, bool discrete_walls)
{
    int M = 2 * N + 1;
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int x = min(w, h);
    int sz = x / M - discrete_walls;

    SDL_Rect rect;

    int left_offset = (w - M * (sz + discrete_walls)) / 2;
    int top_offset = (h - M * (sz + discrete_walls)) / 2;

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (maze[i][j])
            {
                rect = {j * (sz + discrete_walls) + left_offset, i * (sz + discrete_walls) + top_offset, sz, sz};
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}
