#include "Bot.h"

Bot::Bot(int id) : Player(id)
{
    x_dest = 1;
    y_dest = 1;
    path = stack<pair<int, int>>();
}

void Bot::setDestination(Maze maze, int x, int y, vector<Coin> coins)
{
    x_dest = x;
    y_dest = y;
    while (!path.empty())
        path.pop();
    center();
    while (!destinations.empty())
    {
        destinations.pop();
    }
    vector<int> optimal_perm = findOptimalPermutation(maze, coins);
    for (int i = 0; i < coins.size(); i++)
    {
        destinations.push({coins[optimal_perm[i] - 1].getCoordinates().first, coins[optimal_perm[i] - 1].getCoordinates().second});
    }
    destinations.push({x_dest, y_dest});
}
vector<vector<int>> Bot::createMatrix(Maze maze, vector<Coin> coins)
{
    int st_x = x;
    int st_y = y;
    int end_x = x_dest;
    int end_y = y_dest;
    vector<vector<int>> mat(coins.size() + 2, vector<int>(coins.size() + 2, 0));
    vector<vector<int>> curr;
    curr = bfs(maze, st_x, st_y);
    for (int i = 0; i < coins.size(); i++)
    {
        mat[0][i + 1] = curr[coins[i].getCoordinates().second][coins[i].getCoordinates().first];
        mat[i + 1][0] = mat[0][i + 1];
    }
    mat[0][coins.size() + 1] = curr[end_y][end_x];
    mat[coins.size() + 1][0] = mat[0][coins.size() + 1];
    for (int j = 1; j <= coins.size(); j++)
    {
        curr = bfs(maze, coins[j - 1].getCoordinates().first, coins[j - 1].getCoordinates().second);
        for (int i = 0; i < coins.size(); i++)
        {
            mat[j][i + 1] = curr[coins[i].getCoordinates().second][coins[i].getCoordinates().first];
            mat[i + 1][j] = mat[j][i + 1];
        }
        mat[j][coins.size() + 1] = curr[end_x][end_y];
        mat[coins.size() + 1][j] = mat[j][coins.size() + 1];
    }
    return mat;
}
vector<int> Bot::findOptimalPermutation(Maze maze, vector<Coin> coins)
{
    vector<vector<int>> mat = createMatrix(maze, coins);
    for (int i = 0; i < coins.size() + 2; i++)
    {

        for (int j = 0; j < coins.size() + 2; j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }
    for (int i = 0; i < coins.size(); i++)
    {
        cout << coins[i].getCoordinates().first << " " << coins[i].getCoordinates().second << endl;
    }
    //this finds the optimal permutation according to indices
    vector<int> indices;
    for (int i = 1; i <= coins.size(); i++)
    {
        indices.push_back(i);
    }
    int minimal_distance = 1e9;
    vector<int> optimal_answer;
    int cur_d = 0;
    cur_d = mat[0][indices[0]];
    for (int i = 0; i < coins.size() - 1; i++)
    {
        cur_d += mat[indices[i]][indices[i + 1]];
    }
    cur_d += mat[indices[coins.size() - 1]][coins.size() + 1];
    minimal_distance = cur_d;
    optimal_answer = indices;
    cout << minimal_distance << endl;
    while (next_permutation(indices.begin(), indices.end()))
    {
        // for (int i = 0; i < coins.size(); i++)
        // {
        //     cout << indices[i] << " ";
        // }
        // cout << endl;
        cur_d = mat[0][indices[0]];
        for (int i = 0; i < coins.size() - 1; i++)
        {
            cur_d += mat[indices[i]][indices[i + 1]];
        }
        cur_d += mat[indices[coins.size() - 1]][coins.size() + 1];
        if (cur_d <= minimal_distance)
        {
            optimal_answer = indices;
            minimal_distance = cur_d;
        }
        // cout << cur_d << " " << minimal_distance << endl;
    }
    cout << minimal_distance << endl;
    return optimal_answer;
}
vector<vector<int>> Bot::bfs(Maze m, int x, int y)
{
    vector<vector<Box>> maze = m.getMaze();
    int M = maze.size();
    vector<vector<int>> dist(M, vector<int>(M, -1));
    dist[y][x] = 0;
    queue<pair<int, int>> q;
    q.push({y, x});
    while (!q.empty())
    {
        int i = q.front().first;
        int j = q.front().second;
        if (i - 1 >= 0 && dist[i - 1][j] == -1 && maze[i - 1][j].get_block_type() == 0)
        {
            q.push({i - 1, j});
            dist[i - 1][j] = dist[i][j] + 1;
        }
        if (i + 1 < M && dist[i + 1][j] == -1 && maze[i + 1][j].get_block_type() == 0)
        {
            q.push({i + 1, j});
            dist[i + 1][j] = dist[i][j] + 1;
        }
        if (j - 1 >= 0 && dist[i][j - 1] == -1 && maze[i][j - 1].get_block_type() == 0)
        {
            q.push({i, j - 1});
            dist[i][j - 1] = dist[i][j] + 1;
        }
        if (j + 1 < M && dist[i][j + 1] == -1 && maze[i][j + 1].get_block_type() == 0)
        {
            q.push({i, j + 1});
            dist[i][j + 1] = dist[i][j] + 1;
        }
        q.pop();
    }
    return dist;
}

void Bot::center()
{
    x_offset = block_size / 2;
    y_offset = block_size / 2;
}

void Bot::calculatePath(vector<vector<Box>> maze, int x_dest, int y_dest)
{
    // cout << x_dest << " " << y_dest << endl;
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
        path.push({parent[i][j], block_size});
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
        int x = path.top().first;
        vmove = (x == 3) - (x == 1);
        hmove = (x == 4) - (x == 2);
        if (path.top().second-- == 1)
            path.pop();
    }
    else if (!destinations.empty())
    {
        calculatePath(maze.getMaze(), destinations.front().first, destinations.front().second);
        destinations.pop();
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

void Bot::render(SDL_Renderer *renderer)
{
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
