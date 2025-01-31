#include "Maze.h"

Maze::Maze(int N, bool discrete_walls, int width, int height, int seed)
{
    generate(N, seed);
    int x = min(width, height);
    Maze::discrete_walls = discrete_walls;
    block_size = x / M - discrete_walls;
    left_offset = (width - block_size * M) / 2;
    top_offset = (height - block_size * M) / 2;
    close_direction = 0;
    last_close = {1, 0};
    close_radius = 0;
    last_close_time = 0;
    maze_matrix = vector<vector<int>>(M, vector<int>(M, 0));
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            maze_matrix[i][j] = maze[i][j].get_block_type();
        }
    }
    closed = false;
}

// void Maze::generate(int N)
// {
// Using DFS to generate a line boundary maze and then convert it to block walls
// Maze::N = N;
// M = 2 * N + 1;
// maze.clear();
// maze = vector<vector<Box> >();
// for (int i = 0; i < M; i++)
// {
//     vector<Box> temp;
//     for (int j = 0; j < M; j++)
//     {
//         Box new_box = Box(0, i, j);
//         temp.push_back(new_box);
//     }
//     maze.push_back(temp);
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

//     int r =  % ct;
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
//         maze[x][y].update(0, -1);
//         maze[x - 1][y].update(1 - a[i][j][1], -1);
//         maze[x][y - 1].update(1 - a[i][j][2], -1);
//         maze[x + 1][y].update(1 - a[i][j][3], -1);
//         maze[x][y + 1].update(1 - a[i][j][4], -1);
//         maze[x - 1][y - 1].update(2, -1);
//         maze[x + 1][y - 1].update(2, -1);
//         maze[x - 1][y + 1].update(2, -1);
//         maze[x + 1][y + 1].update(2, -1);
//     }
// }
// for (int i = 0; i < M; i++)
// {
//     for (int j = 0; j < M; j++)
//     {
//         if (i == 0 || j == 0 || i == M - 1 || j == M - 1)
//         {
//             maze[i][j].update(2, -1);
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
void Maze::generate(int N, int seed)
{
    Maze::N = N;
    M = 2 * N + 1;
    maze.clear();
    maze = vector<vector<Box>>();
    srand(seed);
    randomiser = PRG(seed);
    for (int i = 0; i < M; i++)
    {
        vector<Box> temp;
        for (int j = 0; j < M; j++)
        {
            Box new_box = Box(0, i, j);
            temp.push_back(new_box);
        }
        maze.push_back(temp);
    }

    for (int i = 0; i < M; i++)
    {
        maze[i][M - 1].update(2, -1);
        maze[0][i].update(2, -1);
        maze[i][0].update(2, -1);
        maze[M - 1][i].update(2, -1);
    }
    for (int i = 1; i < M - 1; i++)
    {
        vector<Box> temp;

        for (int j = 1; j < M - 1; j++)
        {
            if (i % 2 == 0 && j % 2 == 0)
                maze[i][j].update(2, -1);
            else
                maze[i][j].update(randomiser.generate() % 2, -1);
        }
    }
    maze[1][1].update(0, -1);
    maze[1][2].update(0, -1);
    maze[2][1].update(0, -1);
    maze[1][M - 2].update(0, -1);
    maze[1][M - 3].update(0, -1);
    maze[2][M - 2].update(0, -1);
    maze[M - 2][1].update(0, -1);
    maze[M - 2][2].update(0, -1);
    maze[M - 3][1].update(0, -1);
    maze[M - 2][M - 3].update(0, -1);
    maze[M - 2][M - 2].update(0, -1);
    maze[M - 3][M - 2].update(0, -1);
}

void Maze::update(int i, int j, int new_type, int current_time)
{
    if (maze[i][j].get_block_type() != 2)
        maze[i][j].update(new_type, current_time);
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

void Maze::render(SDL_Renderer *renderer, SDL_Surface *surface, vector<SDL_Surface *> &block_surfaces)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            maze[i][j].render(renderer, j * (block_size) + left_offset, i * (block_size) + top_offset, block_size - discrete_walls, block_size - discrete_walls, surface, block_surfaces);
        }
    }
}

int Maze::getBlockSize()
{
    return block_size;
}
void Maze::add_power_up(int i, int j)
{
    power_ups.push_back(maze[i][j]);
}
void Maze::update_power_ups(int current_time)
{
    vector<Box> new_power_ups;
    for (int i = 0; i < power_ups.size(); i++)
    {
        if (power_ups[i].get_start_time() + power_ups[i].get_duration() <= current_time)
        {
            this->update(power_ups[i].get_x(), power_ups[i].get_y(), 0, current_time);
        }
        else
        {
            new_power_ups.push_back(power_ups[i]);
        }
    }
    power_ups = new_power_ups;
}
void Maze::update(int cur_time)
{
    // cout << last_close.first << " " << last_close.second << " " << close_radius << " " << close_direction << endl;
    // cout << last_close_time << " " << cur_time << " ";
    int delay;
    if (close_radius >= 50)
        delay = 10;
    else
        delay = 1000;
    // cout << close_radius << " " << close_direction << " " << delay << endl;

    if (cur_time > last_close_time + delay && close_direction / 4 < close_radius && close_direction < 2 * M - 5)
    {
        if (close_direction % 4 == 0)
        {
            maze[last_close.first][++last_close.second].update(2, -1);
            if (last_close.first + last_close.second == M - 1)
                close_direction++;
        }
        if (close_direction % 4 == 1)
        {
            maze[++last_close.first][last_close.second].update(2, -1);
            if (last_close.first == last_close.second)
                close_direction++;
        }
        if (close_direction % 4 == 2)
        {
            maze[last_close.first][--last_close.second].update(2, -1);
            if (last_close.first + last_close.second == M - 1)
                close_direction++;
        }
        if (close_direction % 4 == 3)
        {
            maze[--last_close.first][last_close.second].update(2, -1);
            if (last_close.first - last_close.second == 1)
            {
                close_direction++;
                close_radius--;
            }
        }
        last_close_time = cur_time;
    }
    if (close_direction == 2 * M - 5)
        closed = true;
}
void Maze::close(int cur_time, int r)
{
    last_close_time = cur_time;
    close_radius = r;
}

string Maze::getBoxUpdates()
{
    string s = "2 ";
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (maze_matrix[i][j] != maze[i][j].get_block_type())
            {
                // maze_matrix[i][j] = maze[i][j].get_block_type();
                s += (to_string(i) + " " + to_string(j) + " " + to_string(maze[i][j].get_block_type()) + " ");
            }
        }
    }
    return s;
}