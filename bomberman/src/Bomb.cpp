#include "Bomb.h"

Bomb::Bomb(Maze &maze, int t, int x1, int y1, int x_off, int y_off, int start_time, int pl_id, int total_released)
{
    type = t;
    block_size = maze.getBlockSize();
    time_beg = start_time;
    direction = -1;
    moving_bomb = false;
    if (type == 1) //default type
    {
        speed = 2;
        radius = 5;
        x = 0;
        y = 0;
        time_explode = 3000;
    }
    bomb_size = block_size - 20;
    x_offset = x_off;
    y_offset = y_off;
    left_offset = maze.left_offset;
    top_offset = maze.top_offset;
    x = x1;
    y = y1;
    id = pl_id;
    count = total_released;
}
void Bomb::set_direction(int d)
{
    direction = d;
}
void Bomb::set_moving()
{
    moving_bomb = true;
}
int Bomb::get_type()
{
    return type;
}

int Bomb::get_x()
{
    return x;
}
int Bomb::get_y()
{
    return y;
}
int Bomb::get_rad()
{
    return radius;
}
int Bomb::get_speed()
{
    return speed;
}
int Bomb::get_time_exp()
{
    return time_explode;
}

int Bomb::get_size()
{
    return bomb_size;
}

pair<bool, int> Bomb::update_state(int current_time, Maze &maze, vector<pair<int, int>> locations, vector<Bomb> &bombs)
{
    //check if moving or stationary presently
    if (moving_bomb)
    {
        cout << "l";
        if (current_time >= time_explode + time_beg)
        {
            //bomb will explode now

            explode(maze);
            int ok = 0;
            for (int i = 0; i < bombs.size(); i++)
            {
                if (bombs[i].id == id && bombs[i].count == count)
                {
                    bombs.erase(bombs.begin() + i);
                    break;
                }
            }
            return {true, id};
        }
        else
        {
            update_location(maze, locations);
        }
    }
    else
    {
        if (current_time >= time_explode + time_beg)
        {
            //bomb will explode now
            explode(maze);
            int ok = 0;
            for (int i = 0; i < bombs.size(); i++)
            {
                if (bombs[i].id == id && bombs[i].count == count)
                {
                    bombs.erase(bombs.begin() + i);
                    break;
                }
            }
            cout << bombs.size() << endl;
            return {true, id};
        }
        //else, bomb will remain at the same position, so do nothing
    }
    return {false, 0};
}

void Bomb::update_location(Maze &maze, vector<pair<int, int>> locations)
{
    vector<vector<Box>> a = maze.getMaze();
    //first check if blocked by any player
    int next_cell_x = x;
    int next_cell_y = y;
    switch (direction)
    {
    //for now, if next cell has a player, stop at the centre of current cell
    //this is done to avoid edge issues
    case 1:
        next_cell_x--;
        break;
    case 3:
        next_cell_x++;
        break;
    case 2:
        next_cell_y--;
        break;
    case 4:
        next_cell_y++;
        break;
    }
    bool found = false;
    for (auto u : locations)
    {
        if (u.first == next_cell_x && u.second == next_cell_y)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        moving_bomb = false;
        direction = -1;
        x_offset = block_size / 2;
        y_offset = block_size / 2;
        return;
    }

    switch (direction)
    {
    case 1:
        if (x_offset - speed - bomb_size / 2 > 0)
        {
            x_offset -= speed;
            //dist_travelled += speed;
        }
        else if (a[y][x - 1].get_block_type() == 0)
        {
            //check if blocked by a vertical cell
            if (a[y - 1][x - 1].get_block_type() >= 1)
            {
                if (y_offset >= bomb_size / 2)
                {
                    //now, the upper block does not pose a constraint
                    if (a[y + 1][x - 1].get_block_type() >= 1)
                    {
                        if (y_offset <= block_size - bomb_size / 2)
                        {
                            //lower block also does not pose a constraint
                            if (x_offset - speed < 0)
                                x--;
                            x_offset = (x_offset + block_size - speed) % block_size;
                            //dist_travelled += speed;
                        }
                        else
                        {
                            moving_bomb = false;
                            direction = -1;
                            x_offset = bomb_size / 2 + 1;
                        }
                    }
                    else
                    {
                        if (x_offset - speed < 0)
                            x--;
                        x_offset = (x_offset + block_size - speed) % block_size;
                        //dist_travelled += speed;
                    }
                }
                else
                {
                    moving_bomb = false;
                    direction = -1;
                    x_offset = bomb_size / 2 + 1;
                    //dist_travelled += abs(old_x_off - x_offset);
                }
            }
            else
            {
                if (x_offset - speed < 0)
                    x--;
                x_offset = (x_offset + block_size - speed) % block_size;
                //dist_travelled += speed;
            }
        }
        else
        {
            moving_bomb = false;
            direction = -1;
            x_offset = bomb_size / 2 + 1;
            //dist_travelled += abs(old_x_off - x_offset);
        }
        break;
    case 3:
        if (x_offset + speed + bomb_size / 2 < block_size)
        {
            x_offset += speed;
            //dist_travelled += speed;
        }
        else if (a[y][x + 1].get_block_type() == 0)
        {
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (y_offset >= bomb_size / 2)
                {
                    //now, the upper block does not pose a constraint
                    if (a[y + 1][x + 1].get_block_type() >= 1)
                    {
                        if (y_offset <= block_size - bomb_size / 2)
                        {
                            //lower block also does not pose a constraint
                            if (x_offset + speed >= block_size)
                                x++;
                            x_offset = (x_offset + speed) % block_size;
                            //dist_travelled += speed;
                        }
                        else
                        {
                            moving_bomb = false;
                            direction = -1;
                            x_offset = block_size - 1 - bomb_size / 2;
                            //dist_travelled += abs(x_offset - old_x_off);
                        }
                    }
                    else
                    {
                        if (x_offset + speed >= block_size)
                            x++;
                        x_offset = (x_offset + speed) % block_size;
                        //dist_travelled += speed;
                    }
                }
                else
                {
                    moving_bomb = false;
                    direction = -1;
                    x_offset = block_size - 1 - bomb_size / 2;
                    //dist_travelled += abs(x_offset - old_x_off);
                }
            }
            else
            {
                if (x_offset + speed >= block_size)
                    x++;
                x_offset = (x_offset + speed) % block_size;
                //dist_travelled += speed;
            }
        }
        else
        {
            moving_bomb = false;
            direction = -1;
            x_offset = block_size - 1 - bomb_size / 2;
            //dist_travelled += abs(x_offset - old_x_off);
        }
        break;
    case 2:
        if (y_offset - speed - bomb_size / 2 > 0)
        {
            y_offset -= speed;
            //dist_travelled += speed;
        }
        else if (a[y - 1][x].get_block_type() == 0)
        {
            //check if blocked by a right block
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (x_offset <= block_size - bomb_size / 2)
                {
                    if (a[y - 1][x - 1].get_block_type() >= 1)
                    {
                        if (x_offset >= bomb_size / 2)
                        {
                            if (y_offset - speed < 0)
                                y--;
                            y_offset = (y_offset + block_size - speed) % block_size;
                            //dist_travelled += speed;
                        }
                        else
                        {
                            moving_bomb = false;
                            direction = -1;
                            y_offset = bomb_size / 2 + 1;
                            //dist_travelled += abs(old_y_off - y_offset);
                        }
                    }
                    else
                    {
                        if (y_offset - speed < 0)
                            y--;
                        y_offset = (y_offset + block_size - speed) % block_size;
                        //dist_travelled += speed;
                    }
                }
                else
                {
                    moving_bomb = false;
                    direction = -1;
                    y_offset = bomb_size / 2 + 1;
                    //dist_travelled += abs(old_y_off - y_offset);
                }
            }
            else
            {
                if (y_offset - speed < 0)
                    y--;
                y_offset = (y_offset + block_size - speed) % block_size;
                //dist_travelled += speed;
            }
        }
        else
        {
            moving_bomb = false;
            direction = -1;
            y_offset = bomb_size / 2 + 1;
            //dist_travelled += abs(old_y_off - y_offset);
        }
        break;
    case 4:
        if (y_offset + speed + bomb_size / 2 < block_size)
        {
            y_offset += speed;
            //dist_travelled += speed;
        }
        else if (a[y + 1][x].get_block_type() == 0)
        {
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (x_offset <= block_size - bomb_size / 2)
                {
                    if (a[y - 1][x - 1].get_block_type() >= 1)
                    {
                        if (x_offset >= bomb_size / 2)
                        {
                            if (y_offset + speed >= block_size)
                                y++;
                            y_offset = (y_offset + speed) % block_size;
                            //dist_travelled += speed;
                        }
                        else
                        {
                            moving_bomb = false;
                            direction = -1;
                            y_offset = block_size - 1 - bomb_size / 2;
                            //dist_travelled += abs(old_y_off - y_offset);
                        }
                    }
                    else
                    {
                        if (y_offset + speed >= block_size)
                            y++;
                        y_offset = (y_offset + speed) % block_size;
                        //dist_travelled += speed;
                    }
                }
                else
                {
                    moving_bomb = false;
                    direction = -1;
                    y_offset = block_size - 1 - bomb_size / 2;
                    //dist_travelled += abs(old_y_off - y_offset);
                }
            }
            else
            {
                if (y_offset + speed >= block_size)
                    y++;
                y_offset = (y_offset + speed) % block_size;
                //dist_travelled += speed;
            }
        }
        else
        {
            moving_bomb = false;
            direction = -1;
            y_offset = block_size - 1 - bomb_size / 2;
            //dist_travelled += abs(old_y_off - y_offset);
        }
        break;
    }
}

void Bomb::explode(Maze &maze)
{
    vector<vector<Box>> a = maze.getMaze();
    //just explode the bomb, in all four directions up till radius
    for (int x_curr = x; x_curr >= max(0, x - radius); x_curr--)
    {
        if (a[y][x_curr].get_block_type() != 0)
        {
            if (a[y][x_curr].get_block_type() == 1)
            {
                maze.update(y, x_curr, 0);
            }
            break;
        }
    }
    for (int x_curr = x; x_curr <= min(maze.getSize() - 1, x + radius); x_curr++)
    {
        if (a[y][x_curr].get_block_type() != 0)
        {
            if (a[y][x_curr].get_block_type() == 1)
            {
                maze.update(y, x_curr, 0);
            }
            break;
        }
    }
    for (int y_curr = y; y_curr >= max(0, y - radius); y_curr--)
    {
        if (a[y_curr][x].get_block_type() != 0)
        {
            if (a[y_curr][x].get_block_type() == 1)
            {
                maze.update(y_curr, x, 0);
            }
            break;
        }
    }
    for (int y_curr = y; y_curr <= min(maze.getSize() - 1, y + radius); y_curr++)
    {
        if (a[y_curr][x].get_block_type() != 0)
        {
            if (a[y_curr][x].get_block_type() == 1)
            {
                maze.update(y_curr, x, 0);
            }
            break;
        }
    }
    //now, check for any players in the neighbourhood.
}

void Bomb::render(SDL_Renderer *renderer)
{
    int x1 = x * block_size + x_offset + left_offset - (bomb_size / 2);
    int y1 = y * block_size + y_offset + top_offset - (bomb_size / 2);
    int w = bomb_size;
    int h = bomb_size;
    SDL_Rect rect;
    rect = {x1, y1, w, h};
    int r, g, b, a;
    if (type == 1)
    {
        r = 200;
        g = 200;
        b = 0;
        a = 255;
    }
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}
