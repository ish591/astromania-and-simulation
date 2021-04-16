#include "Bomb.h"

Bomb::Bomb(Maze &maze, int t, int dir, int x1, int y1, int x_off, int y_off)
{
    type = t;
    direction = dir;
    block_size = maze.getBlockSize();
    if (type == 1)
    {
        speed = 2;
        radius = 5;
        x = 0; //default value
        y = 0; //default value
        dist_explode = 300;
        dist_travelled = 0;
    }
    bomb_size = block_size - 40;
    x_offset = x_off;
    y_offset = y_off;
    left_offset = maze.left_offset;
    top_offset = maze.top_offset;
    x = x1;
    y = y1;
}

int Bomb::get_speed()
{
    return speed;
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

int Bomb::get_dist_exp()
{
    return dist_explode;
}

int Bomb::get_dist_travelled()
{
    return dist_travelled;
}

int Bomb::get_direction()
{
    return direction;
}

int Bomb::get_size()
{
    return bomb_size;
}
bool Bomb::update_location(Maze &maze)
{
    //update the location of bomb. need to check for collisions in this case
    vector<vector<Box>> a = maze.getMaze();
    switch (direction)
    {
    case 1:
        if (x_offset - speed - bomb_size / 2 > 0)
        {
            x_offset -= speed;
            dist_travelled += speed;
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
                            dist_travelled += speed;
                        }
                        else
                        {
                            int old_x_off = x_offset;
                            x_offset = bomb_size / 2 + 1;
                            dist_travelled += abs(old_x_off - x_offset);
                        }
                    }
                    else
                    {
                        if (x_offset - speed < 0)
                            x--;
                        x_offset = (x_offset + block_size - speed) % block_size;
                        dist_travelled += speed;
                    }
                }
                else
                {
                    int old_x_off = x_offset;
                    x_offset = bomb_size / 2 + 1;
                    dist_travelled += abs(old_x_off - x_offset);
                }
            }
            else
            {
                if (x_offset - speed < 0)
                    x--;
                x_offset = (x_offset + block_size - speed) % block_size;
                dist_travelled += speed;
            }
        }
        else
        {
            int old_x_off = x_offset;
            x_offset = bomb_size / 2 + 1;
            dist_travelled += abs(old_x_off - x_offset);
        }
        break;
    case 3:
        if (x_offset + speed + bomb_size / 2 < block_size)
        {
            x_offset += speed;
            dist_travelled += speed;
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
                            dist_travelled += speed;
                        }
                        else
                        {
                            int old_x_off = x_offset;
                            x_offset = block_size - 1 - bomb_size / 2;
                            dist_travelled += abs(x_offset - old_x_off);
                        }
                    }
                    else
                    {
                        if (x_offset + speed >= block_size)
                            x++;
                        x_offset = (x_offset + speed) % block_size;
                        dist_travelled += speed;
                    }
                }
                else
                {
                    int old_x_off = x_offset;
                    x_offset = block_size - 1 - bomb_size / 2;
                    dist_travelled += abs(x_offset - old_x_off);
                }
            }
            else
            {
                if (x_offset + speed >= block_size)
                    x++;
                x_offset = (x_offset + speed) % block_size;
                dist_travelled += speed;
            }
        }
        else
        {
            int old_x_off = x_offset;
            x_offset = block_size - 1 - bomb_size / 2;
            dist_travelled += abs(x_offset - old_x_off);
        }
        break;
    case 2:
        if (y_offset - speed - bomb_size / 2 > 0)
        {
            y_offset -= speed;
            dist_travelled += speed;
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
                            dist_travelled += speed;
                        }
                        else
                        {
                            int old_y_off = y_offset;
                            y_offset = bomb_size / 2 + 1;
                            dist_travelled += abs(old_y_off - y_offset);
                        }
                    }
                    else
                    {
                        if (y_offset - speed < 0)
                            y--;
                        y_offset = (y_offset + block_size - speed) % block_size;
                        dist_travelled += speed;
                    }
                }
                else
                {
                    int old_y_off = y_offset;
                    y_offset = bomb_size / 2 + 1;
                    dist_travelled += abs(old_y_off - y_offset);
                }
            }
            else
            {
                if (y_offset - speed < 0)
                    y--;
                y_offset = (y_offset + block_size - speed) % block_size;
                dist_travelled += speed;
            }
        }
        else
        {
            int old_y_off = y_offset;
            y_offset = bomb_size / 2 + 1;
            dist_travelled += abs(old_y_off - y_offset);
        }
        break;
    case 4:
        if (y_offset + speed + bomb_size / 2 < block_size)
        {
            y_offset += speed;
            dist_travelled += speed;
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
                            dist_travelled += speed;
                        }
                        else
                        {
                            int old_y_off = y_offset;
                            y_offset = block_size - 1 - bomb_size / 2;
                            dist_travelled += abs(old_y_off - y_offset);
                        }
                    }
                    else
                    {
                        if (y_offset + speed >= block_size)
                            y++;
                        y_offset = (y_offset + speed) % block_size;
                        dist_travelled += speed;
                    }
                }
                else
                {
                    int old_y_off = y_offset;
                    y_offset = block_size - 1 - bomb_size / 2;
                    dist_travelled += abs(old_y_off - y_offset);
                }
            }
            else
            {
                if (y_offset + speed >= block_size)
                    y++;
                y_offset = (y_offset + speed) % block_size;
                dist_travelled += speed;
            }
        }
        else
        {
            int old_y_off = y_offset;
            y_offset = block_size - 1 - bomb_size / 2;
            dist_travelled += abs(old_y_off - y_offset);
        }
        break;
    }
    if (dist_travelled == dist_explode)
    {
        //this is the case of self explosion
        return true;
    }
    return false;
}

bool Bomb::collision(Maze &maze)
{
    //first check collision with a maze wall.
    //considered only of x_offset = limiting value and blocked on left
    vector<vector<Box>> a = maze.getMaze();
    bool res = false;
    switch (direction)
    {
    case 1:
        if (x_offset == bomb_size / 2 + 1)
        {
            if (a[y][x - 1].get_block_type() != 0)
            {

                if (a[y][x - 1].get_block_type() == 1)
                {
                    maze.update(y, x - 1, 0);
                }
                res = true;
            }
            //else it is also possible it has collided with a vertical cell on its path
            if (a[y - 1][x - 1].get_block_type() != 0 && y_offset < bomb_size / 2)
            {
                if (a[y - 1][x - 1].get_block_type() == 1)
                {
                    maze.update(y - 1, x - 1, 0);
                }
                res = true;
            }
            if (a[y + 1][x - 1].get_block_type() != 0 && y_offset > block_size - bomb_size / 2)
            {
                if (a[y + 1][x - 1].get_block_type() == 1)
                {
                    maze.update(y + 1, x - 1, 0);
                }
                res = true;
            }
        }
        break;
    case 3:
        if (x_offset == block_size - 1 - bomb_size / 2)
        {
            if (a[y][x + 1].get_block_type() != 0)
            {
                if (a[y][x + 1].get_block_type() == 1)
                {
                    maze.update(y, x + 1, 0);
                }
                res = true;
            }
            if (a[y - 1][x + 1].get_block_type() != 0 && y_offset < bomb_size / 2)
            {
                if (a[y - 1][x + 1].get_block_type() == 1)
                {
                    maze.update(y - 1, x + 1, 0);
                }
                res = true;
            }
            if (a[y + 1][x + 1].get_block_type() != 0 && y_offset > block_size - bomb_size / 2)
            {
                if (a[y + 1][x + 1].get_block_type() == 1)
                {
                    maze.update(y + 1, x + 1, 0);
                }
                res = true;
            }
        }
        break;
    case 2:
        if (y_offset == bomb_size / 2 + 1)
        {
            if (a[y - 1][x].get_block_type() != 0)
            {
                if (a[y - 1][x].get_block_type() == 1)
                {
                    maze.update(y - 1, x, 0);
                }
                res = true;
            }
            if (a[y - 1][x + 1].get_block_type() != 0 && x_offset > block_size - bomb_size / 2)
            {
                if (a[y - 1][x + 1].get_block_type() == 1)
                {
                    maze.update(y - 1, x + 1, 0);
                }
                res = true;
            }
            if (a[y - 1][x - 1].get_block_type() != 0 && x_offset < bomb_size / 2)
            {
                if (a[y - 1][x - 1].get_block_type() == 1)
                {
                    maze.update(y - 1, x - 1, 0);
                }
                res = true;
            }
        }
        break;
    case 4:
        if (y_offset == block_size - 1 - bomb_size / 2)
        {
            if (a[y + 1][x].get_block_type() != 0)
            {
                if (a[y + 1][x].get_block_type() == 1)
                {
                    maze.update(y + 1, x, 0);
                }
                res = true;
            }
            if (a[y + 1][x + 1].get_block_type() != 0 && x_offset > block_size - bomb_size / 2)
            {
                if (a[y + 1][x + 1].get_block_type() == 1)
                {
                    maze.update(y + 1, x + 1, 0);
                }
                res = true;
            }
            if (a[y + 1][x - 1].get_block_type() != 0 && x_offset < bomb_size / 2)
            {
                if (a[y + 1][x - 1].get_block_type() == 1)
                {
                    maze.update(y + 1, x - 1, 0);
                }
                res = true;
            }
        }
        break;
    }
    return res;
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