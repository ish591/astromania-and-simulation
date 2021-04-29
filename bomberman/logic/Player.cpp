#include "Player.h"

Player::Player(int id, Maze &maze)
{
    player_id = id;
    player_state = 2;
    if (id == 1)
    {
        x = 1;
        y = 1;
        color_r = 255;
        color_g = 255;
        color_b = 0;
        UP = SDLK_UP;
        DOWN = SDLK_DOWN;
        RIGHT = SDLK_RIGHT;
        LEFT = SDLK_LEFT;
        DROP_BOMB = SDLK_RETURN;
    }
    if (id == 2)
    {
        x = maze.getSize() - 2;
        y = x;
        color_r = 0;
        color_g = 255;
        color_b = 0;
        UP = SDLK_w;
        LEFT = SDLK_a;
        DOWN = SDLK_s;
        RIGHT = SDLK_d;
        DROP_BOMB = SDLK_SPACE;
    }
    if (id == 3)
    {
        x = 1;
        y = maze.getSize() - 2;
        color_r = 0;
        color_g = 255;
        color_b = 0;
        UP = SDLK_y;
        LEFT = SDLK_g;
        DOWN = SDLK_h;
        RIGHT = SDLK_j;
        DROP_BOMB = SDLK_u;
    }
    if (id == 4)
    {
        x = maze.getSize() - 2;
        y = 1;
        color_r = 0;
        color_g = 255;
        color_b = 0;
        UP = SDLK_1;
        LEFT = SDLK_2;
        DOWN = SDLK_3;
        RIGHT = SDLK_4;
        DROP_BOMB = SDLK_5;
    }

    x_offset = 0, y_offset = 0;
    score = 0;
    UP_PRESSED = 0;
    LEFT_PRESSED = 0;
    DOWN_PRESSED = 0;
    RIGHT_PRESSED = 0;
    SPACE_PRESSED = 0;
    bomb_count = 0;
    total_released = 0;
    power_up_duration = 8000;
    lives = 3;
    last_life_loss_time = -5000;
    power_ups.push_back({0, -1}); //slidable bomb
    power_ups.push_back({1, -1}); //speed increased
    power_ups.push_back({2, -1}); //radius increased
    power_ups.push_back({1, -1}); //more number of bombs
}
int Player::get_x()
{
    return x;
}
int Player::getId()
{
    return player_id;
}
int Player::get_y()
{
    return y;
}
int Player::get_x_offset()
{
    return x_offset;
}
int Player::get_y_offset()
{
    return y_offset;
}
int Player::get_size()
{
    return player_size;
}
int Player::get_bomb_count()
{
    return bomb_count;
}
void Player::update_bomb_count(int new_count)
{
    bomb_count = new_count;
}
void Player::updateDimensions(Maze &maze, int w, int h)
{
    block_size = maze.getBlockSize();
    player_size = block_size - 15;
    if (player_size % 2 == 1)
        player_size--;

    left_offset = maze.left_offset;
    top_offset = maze.top_offset;
    x_offset = block_size / 2;
    y_offset = block_size / 2;
    power_ups[1].first = 1;
}

void Player::takeAction(int down_press, SDL_Keycode key_press, Maze &maze, vector<Bomb> &bombs, int current_time)
{

    if (down_press == 0)
    {
        if (key_press == LEFT)
        {
            LEFT_PRESSED = 1;
        }
        else if (key_press == RIGHT)
        {
            RIGHT_PRESSED = 1;
        }
        else if (key_press == UP)
        {
            UP_PRESSED = 1;
        }
        else if (key_press == DOWN)
        {
            DOWN_PRESSED = 1;
        }
        else if (key_press == DROP_BOMB)
        {
            if (bomb_count < power_ups[3].first) //then only release
            {
                //currently setting the type as 1 only
                total_released++;
                bomb_count++;
                Bomb new_bomb = Bomb(maze, min(5, power_ups[2].first), x, y, block_size / 2, block_size / 2, current_time, player_id, total_released, color_r, color_g, color_b);
                bombs.push_back(new_bomb);
            }
        }
    }
    else
    {
        if (key_press == LEFT)
            LEFT_PRESSED = 0;
        else if (key_press == RIGHT)
            RIGHT_PRESSED = 0;
        else if (key_press == UP)
            UP_PRESSED = 0;
        else if (key_press == DOWN)
            DOWN_PRESSED = 0;
    }
}

void Player::update_power_ups(int current_time)
{
    if (power_ups[1].first > 1)
    {
        if (power_ups[1].second + power_up_duration < current_time)
        {
            power_ups[1] = {1, -1};
        }
    }
}
void Player::updateLocation(Maze &maze, vector<Player> &players, vector<Bomb> &bombs, int current_time, vector<Explosion> &explosions)
{
    //first of all remove power ups of the user if time has elapsed
    update_power_ups(current_time);
    update_bombs(maze, players, bombs, current_time, explosions);
    //new bombs have been released, and collision with bombs have been checked.
    //now, handle the sliding feature. if next cell has a slidable bomb, slide it
    //else the player will get blocked by it
    //player can also aquire a power up now
    //cell type 3 -> sets 0 (throwable)
    //cell type 4 -> sets 1 (speed)
    //cell type 5 -> sets 1 (radius increased)
    int hmove = RIGHT_PRESSED - LEFT_PRESSED;
    int vmove = DOWN_PRESSED - UP_PRESSED;
    vector<vector<Box>> a = maze.getMaze();
    // cout << hmove << vmove << endl;
    if (a[y][x].get_block_type() == 2)
    {
        kill(current_time);
    }
    switch (hmove)
    {
    case -1:
        player_state = 2;
        if (x_offset - power_ups[1].first - player_size / 2 > 0)
        {
            x_offset -= power_ups[1].first;
        }
        else if (a[y][x - 1].get_block_type() == 0 || a[y][x - 1].get_block_type() >= 3)
        {
            //check if blocked by a vertical cell
            if (a[y - 1][x - 1].get_block_type() >= 1)
            {
                if (y_offset >= player_size / 2)
                {
                    //now, the upper block does not pose a constraint
                    if (a[y + 1][x - 1].get_block_type() >= 1)
                    {
                        if (y_offset <= block_size - player_size / 2)
                        {
                            //lower block also does not pose a constraint
                            int next_x = x - 1;
                            int next_y = y;
                            bool found = false;
                            for (int b = 0; b < bombs.size(); b++)
                            {
                                if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                                {
                                    if (power_ups[0].first && bombs[b].get_id() == player_id)
                                    {
                                        bombs[b].set_direction(1);
                                        bombs[b].set_moving();
                                        //cout << "ok";
                                    }
                                    found = true;
                                }
                            }
                            if (found)
                            {
                                x_offset = player_size / 2 + 1;
                            }
                            else
                            {
                                //check for powerup
                                int num = a[y][x - 1].get_block_type();
                                maze.update(y, x - 1, 0, current_time);
                                switch (num)
                                {
                                case 3:
                                    power_ups[0] = {1, current_time};
                                    //throwable bomb powerup
                                    break;
                                case 4:
                                    power_ups[1] = {2, current_time};
                                    power_ups[1].first = 2;
                                    //increased speed
                                    break;
                                case 5:
                                    power_ups[2].first++;
                                    //increased bomb radius
                                    break;
                                case 6:
                                    //more bombs
                                    power_ups[3].first++;
                                    break;
                                }
                                if (x_offset - power_ups[1].first < 0)
                                    x--;
                                x_offset = (x_offset + block_size - power_ups[1].first) % block_size;
                            }
                        }
                        else
                        {
                            x_offset = player_size / 2 + 1;
                        }
                    }
                    else
                    {
                        int next_x = x - 1;
                        int next_y = y;
                        bool found = false;
                        for (int b = 0; b < bombs.size(); b++)
                        {
                            if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                            {
                                if (power_ups[0].first && bombs[b].get_id() == player_id)
                                {
                                    bombs[b].set_direction(1);
                                    bombs[b].set_moving();

                                    //cout << "ok";
                                }
                                found = true;
                            }
                        }
                        if (found)
                        {
                            x_offset = player_size / 2 + 1;
                        }
                        else
                        {
                            int num = a[y][x - 1].get_block_type();
                            maze.update(y, x - 1, 0, current_time);
                            switch (num)
                            {
                            case 3:
                                power_ups[0] = {1, current_time};
                                //throwable bomb powerup, stays with the user for
                                //a particular time
                                break;
                            case 4:
                                power_ups[1] = {2, current_time};
                                power_ups[1].first = 2;
                                break;
                            case 5:
                                power_ups[2].first++;
                                //increased bomb radius
                                break;
                            case 6:
                                power_ups[3].first++;
                                break;
                            }
                            if (x_offset - power_ups[1].first < 0)
                                x--;
                            x_offset = (x_offset + block_size - power_ups[1].first) % block_size;
                        }
                    }
                }
                else
                {
                    x_offset = player_size / 2 + 1;
                }
            }
            else
            {
                int next_x = x - 1;
                int next_y = y;
                bool found = false;
                for (int b = 0; b < bombs.size(); b++)
                {
                    if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                    {
                        if (power_ups[0].first && bombs[b].get_id() == player_id)
                        {
                            bombs[b].set_direction(1);
                            bombs[b].set_moving();
                            // cout << "ok";
                        }
                        found = true;
                    }
                }
                if (found)
                {
                    x_offset = player_size / 2 + 1;
                }
                else
                {
                    int num = a[y][x - 1].get_block_type();
                    maze.update(y, x - 1, 0, current_time);
                    switch (num)
                    {
                    case 3:
                        power_ups[0] = {1, current_time};
                        //throwable bomb powerup, stays with the user for
                        //a particular time
                        break;
                    case 4:
                        power_ups[1] = {2, current_time};
                        power_ups[1].first = 2;
                        break;
                    case 5:
                        power_ups[2].first++;
                        //increased bomb radius
                        break;
                    case 6:
                        power_ups[3].first++;
                        break;
                    }
                    if (x_offset - power_ups[1].first < 0)
                        x--;
                    x_offset = (x_offset + block_size - power_ups[1].first) % block_size;
                }
            }
        }
        else
        {
            x_offset = player_size / 2 + 1;
        }
        break;

    case 0:
        break;

    case 1:
        player_state = 3;
        if (x_offset + power_ups[1].first + player_size / 2 < block_size)
        {
            x_offset += power_ups[1].first;
        }
        else if (a[y][x + 1].get_block_type() == 0 || a[y][x + 1].get_block_type() >= 3)
        {
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (y_offset >= player_size / 2)
                {
                    //now, the upper block does not pose a constraint
                    if (a[y + 1][x + 1].get_block_type() >= 1)
                    {
                        if (y_offset <= block_size - player_size / 2)
                        {
                            //lower block also does not pose a constraint
                            int next_x = x + 1;
                            int next_y = y;
                            bool found = false;
                            for (int b = 0; b < bombs.size(); b++)
                            {
                                if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                                {
                                    if (power_ups[0].first && bombs[b].get_id() == player_id)
                                    {
                                        bombs[b].set_direction(3);
                                        bombs[b].set_moving();
                                    }
                                    found = true;
                                }
                            }
                            if (found)
                            {
                                x_offset = block_size - 1 - player_size / 2;
                            }
                            else
                            {
                                int num = a[y][x + 1].get_block_type();
                                maze.update(y, x + 1, 0, current_time);
                                switch (num)
                                {
                                case 3:
                                    power_ups[0] = {1, current_time};
                                    //throwable bomb powerup, stays with the user for
                                    //a particular time
                                    break;
                                case 4:
                                    power_ups[1] = {2, current_time};
                                    power_ups[1].first = 2;
                                    break;
                                case 5:
                                    power_ups[2].first++;
                                    //increased bomb radius
                                    break;
                                case 6:
                                    power_ups[3].first++;
                                    break;
                                }
                                if (x_offset + power_ups[1].first >= block_size)
                                    x++;
                                x_offset = (x_offset + power_ups[1].first) % block_size;
                            }
                        }
                        else
                        {
                            x_offset = block_size - 1 - player_size / 2;
                        }
                    }
                    else
                    {
                        int next_x = x + 1;
                        int next_y = y;
                        bool found = false;
                        for (int b = 0; b < bombs.size(); b++)
                        {
                            if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                            {
                                if (power_ups[0].first && bombs[b].get_id() == player_id)
                                {
                                    bombs[b].set_direction(3);
                                    bombs[b].set_moving();
                                }
                                found = true;
                            }
                        }
                        if (found)
                        {
                            x_offset = block_size - 1 - player_size / 2;
                        }
                        else
                        {
                            int num = a[y][x + 1].get_block_type();
                            maze.update(y, x + 1, 0, current_time);
                            switch (num)
                            {
                            case 3:
                                power_ups[0] = {1, current_time};
                                //throwable bomb powerup, stays with the user for
                                //a particular time
                                break;
                            case 4:
                                power_ups[1] = {2, current_time};
                                power_ups[1].first = 2;
                                break;
                            case 5:
                                power_ups[2].first++;
                                //increased bomb radius
                                break;
                            case 6:
                                power_ups[3].first++;
                                break;
                            }
                            if (x_offset + power_ups[1].first >= block_size)
                                x++;
                            x_offset = (x_offset + power_ups[1].first) % block_size;
                        }
                    }
                }
                else
                {
                    x_offset = block_size - 1 - player_size / 2;
                }
            }
            else
            {
                int next_x = x + 1;
                int next_y = y;
                bool found = false;
                for (int b = 0; b < bombs.size(); b++)
                {
                    if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                    {
                        if (power_ups[0].first && bombs[b].get_id() == player_id)
                        {
                            bombs[b].set_direction(3);
                            bombs[b].set_moving();
                        }
                        found = true;
                    }
                }
                if (found)
                {
                    x_offset = block_size - 1 - player_size / 2;
                }
                else
                {
                    int num = a[y][x + 1].get_block_type();
                    maze.update(y, x + 1, 0, current_time);
                    switch (num)
                    {
                    case 3:
                        power_ups[0] = {1, current_time};
                        //throwable bomb powerup, stays with the user for
                        //a particular time
                        break;
                    case 4:
                        power_ups[1] = {2, current_time};
                        power_ups[1].first = 2;
                        break;
                    case 5:
                        power_ups[2].first++;
                        //increased bomb radius
                        break;
                    case 6:
                        power_ups[3].first++;
                        break;
                    }
                    if (x_offset + power_ups[1].first >= block_size)
                        x++;
                    x_offset = (x_offset + power_ups[1].first) % block_size;
                }
            }
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
        // player_state = 1;
        if (y_offset - power_ups[1].first - player_size / 2 > 0)
        {
            y_offset -= power_ups[1].first;
        }
        else if (a[y - 1][x].get_block_type() == 0 || a[y - 1][x].get_block_type() >= 3)
        {
            //check if blocked by a right block
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (x_offset <= block_size - player_size / 2)
                {
                    if (a[y - 1][x - 1].get_block_type() >= 1)
                    {
                        if (x_offset >= player_size / 2)
                        {
                            int next_x = x;
                            int next_y = y - 1;
                            bool found = false;
                            for (int b = 0; b < bombs.size(); b++)
                            {
                                if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                                {
                                    if (power_ups[0].first && bombs[b].get_id() == player_id)
                                    {
                                        bombs[b].set_direction(2);
                                        bombs[b].set_moving();
                                    }
                                    found = true;
                                }
                            }
                            if (found)
                            {
                                y_offset = player_size / 2 + 1;
                            }
                            else
                            {
                                int num = a[y - 1][x].get_block_type();
                                maze.update(y - 1, x, 0, current_time);
                                switch (num)
                                {
                                case 3:
                                    power_ups[0] = {1, current_time};
                                    //throwable bomb powerup, stays with the user for
                                    //a particular time
                                    break;
                                case 4:
                                    power_ups[1] = {2, current_time};
                                    power_ups[1].first = 2;
                                    break;
                                case 5:
                                    power_ups[2].first++;
                                    //increased bomb radius
                                    break;
                                case 6:
                                    power_ups[3].first++;
                                    break;
                                }
                                if (y_offset - power_ups[1].first < 0)
                                    y--;
                                y_offset = (y_offset + block_size - power_ups[1].first) % block_size;
                            }
                        }
                        else
                        {
                            y_offset = player_size / 2 + 1;
                        }
                    }
                    else
                    {
                        int next_x = x;
                        int next_y = y - 1;
                        bool found = false;
                        for (int b = 0; b < bombs.size(); b++)
                        {
                            if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                            {
                                if (power_ups[0].first && bombs[b].get_id() == player_id)
                                {
                                    bombs[b].set_direction(2);
                                    bombs[b].set_moving();
                                }
                                found = true;
                            }
                        }
                        if (found)
                        {
                            y_offset = player_size / 2 + 1;
                        }
                        else
                        {
                            int num = a[y - 1][x].get_block_type();
                            maze.update(y - 1, x, 0, current_time);
                            switch (num)
                            {
                            case 3:
                                power_ups[0] = {1, current_time};
                                //throwable bomb powerup, stays with the user for
                                //a particular time
                                break;
                            case 4:
                                power_ups[1] = {2, current_time};
                                power_ups[1].first = 2;
                                break;
                            case 5:
                                power_ups[2].first++;
                                //increased bomb radius
                                break;
                            case 6:
                                power_ups[3].first++;
                                break;
                            }
                            if (y_offset - power_ups[1].first < 0)
                                y--;
                            y_offset = (y_offset + block_size - power_ups[1].first) % block_size;
                        }
                    }
                }
                else
                {
                    y_offset = player_size / 2 + 1;
                }
            }
            else
            {
                int next_x = x;
                int next_y = y - 1;
                bool found = false;
                for (int b = 0; b < bombs.size(); b++)
                {
                    if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                    {
                        if (power_ups[0].first && bombs[b].get_id() == player_id)
                        {
                            bombs[b].set_direction(2);
                            bombs[b].set_moving();
                        }
                        found = true;
                    }
                }
                if (found)
                {
                    y_offset = player_size / 2 + 1;
                }
                else
                {
                    int num = a[y - 1][x].get_block_type();
                    maze.update(y - 1, x, 0, current_time);
                    switch (num)
                    {
                    case 3:
                        power_ups[0] = {1, current_time};
                        //throwable bomb powerup, stays with the user for
                        //a particular time
                        break;
                    case 4:
                        power_ups[1] = {2, current_time};
                        power_ups[1].first = 2;
                        break;
                    case 5:
                        power_ups[2].first++;
                        //increased bomb radius
                        break;
                    case 6:
                        power_ups[3].first++;
                        break;
                    }
                    if (y_offset - power_ups[1].first < 0)
                        y--;
                    y_offset = (y_offset + block_size - power_ups[1].first) % block_size;
                }
            }
        }
        else
        {
            y_offset = player_size / 2 + 1;
        }
        break;

    case 0:
        break;

    case 1:
        // player_state = 0;
        if (y_offset + power_ups[1].first + player_size / 2 < block_size)
        {
            y_offset += power_ups[1].first;
        }
        else if (a[y + 1][x].get_block_type() == 0 || a[y + 1][x].get_block_type() >= 3)
        {
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (x_offset <= block_size - player_size / 2)
                {
                    if (a[y - 1][x - 1].get_block_type() >= 1)
                    {
                        if (x_offset >= player_size / 2)
                        {
                            int next_x = x;
                            int next_y = y + 1;
                            bool found = false;
                            for (int b = 0; b < bombs.size(); b++)
                            {
                                if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                                {
                                    if (power_ups[0].first && bombs[b].get_id() == player_id)
                                    {
                                        bombs[b].set_direction(4);
                                        bombs[b].set_moving();
                                    }
                                    found = true;
                                }
                            }
                            if (found)
                            {
                                y_offset = block_size - 1 - player_size / 2;
                            }
                            else
                            {
                                int num = a[y + 1][x].get_block_type();
                                maze.update(y + 1, x, 0, current_time);
                                switch (num)
                                {
                                case 3:
                                    power_ups[0] = {1, current_time};
                                    //throwable bomb powerup, stays with the user for
                                    //a particular time
                                    break;
                                case 4:
                                    power_ups[1] = {2, current_time};
                                    power_ups[1].first = 2;
                                    break;
                                case 5:
                                    power_ups[2].first++;
                                    //increased bomb radius
                                    break;
                                case 6:
                                    power_ups[3].first++;
                                    break;
                                }
                                if (y_offset + power_ups[1].first >= block_size)
                                    y++;
                                y_offset = (y_offset + power_ups[1].first) % block_size;
                            }
                        }
                        else
                        {
                            y_offset = block_size - 1 - player_size / 2;
                        }
                    }
                    else
                    {
                        int next_x = x;
                        int next_y = y + 1;
                        bool found = false;
                        for (int b = 0; b < bombs.size(); b++)
                        {
                            if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                            {
                                if (power_ups[0].first && bombs[b].get_id() == player_id)
                                {
                                    bombs[b].set_direction(4);
                                    bombs[b].set_moving();
                                }
                                found = true;
                            }
                        }
                        if (found)
                        {
                            y_offset = block_size - 1 - player_size / 2;
                        }
                        else
                        {
                            int num = a[y + 1][x].get_block_type();
                            maze.update(y + 1, x, 0, current_time);
                            switch (num)
                            {
                            case 3:
                                power_ups[0] = {1, current_time};
                                //throwable bomb powerup, stays with the user for
                                //a particular time
                                break;
                            case 4:
                                power_ups[1] = {2, current_time};
                                power_ups[1].first = 2;
                                break;
                            case 5:
                                power_ups[2].first++;
                                //increased bomb radius
                                break;
                            case 6:
                                power_ups[3].first++;
                                break;
                            }
                            if (y_offset + power_ups[1].first >= block_size)
                                y++;
                            y_offset = (y_offset + power_ups[1].first) % block_size;
                        }
                    }
                }
                else
                {
                    y_offset = block_size - 1 - player_size / 2;
                }
            }
            else
            {
                int next_x = x;
                int next_y = y + 1;
                bool found = false;
                for (int b = 0; b < bombs.size(); b++)
                {
                    if (bombs[b].get_x() == next_x & bombs[b].get_y() == next_y)
                    {
                        if (power_ups[0].first && bombs[b].get_id() == player_id)
                        {
                            bombs[b].set_direction(4);
                            bombs[b].set_moving();
                        }
                        found = true;
                    }
                }
                if (found)
                {
                    y_offset = block_size - 1 - player_size / 2;
                }
                else
                {
                    int num = a[y + 1][x].get_block_type();
                    maze.update(y + 1, x, 0, current_time);
                    switch (num)
                    {
                    case 3:
                        power_ups[0] = {1, current_time};
                        //throwable bomb powerup, stays with the user for
                        //a particular time
                        break;
                    case 4:
                        power_ups[1] = {2, current_time};
                        power_ups[1].first = 2;
                        break;
                    case 5:
                        power_ups[2].first++;
                        //increased bomb radius
                        break;
                    case 6:
                        power_ups[3].first++;
                        break;
                    }
                    if (y_offset + power_ups[1].first >= block_size)
                        y++;
                    y_offset = (y_offset + power_ups[1].first) % block_size;
                }
            }
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
void Player::update_bombs(Maze &maze, vector<Player> &players, vector<Bomb> &bombs, int current_time, vector<Explosion> &explosions)
{
    //first of all update all the bombs
    // if a bomb explodes, remove it from this vector.
    // also, decrement count of player
    vector<Bomb> new_bombs;
    vector<pair<int, int>> locations;
    for (int i = 0; i < players.size(); i++)
    {
        locations.push_back({players[i].get_x(), players[i].get_y()});
    }
    for (Bomb u : bombs)
    {
        pair<bool, int> res = u.update_state(current_time, maze, locations, bombs, explosions);
        if (!res.first) //bomb has not  exploded
        {
            new_bombs.push_back(u);
        }
        else
        {
            for (int i = 0; i < players.size(); i++)
            {
                if (players[i].getId() == res.second)
                {
                    players[i].update_bomb_count(players[i].get_bomb_count() - 1);
                }
            }
        }
    }
    bombs = new_bombs;
    //all old bombs have been updated and explosions, collissions checked.
    // now time for releasing a new bomb!
}

void Player::kill(int cur_time)
{

    // cout << cur_time << endl;
    if (last_life_loss_time + 1500 < cur_time)
    {
        lives--;
        // cout << lives << endl;
        last_life_loss_time = cur_time;
    }
}

bool Player::isAlive()
{
    if (lives > 0)
        return true;
    return false;
}

void Player::render(SDL_Renderer *renderer, SDL_Surface *surface, vector<vector<SDL_Surface *>> &player_surfaces)
{
    if (!isAlive())
        player_state = 4;
    surface = (player_surfaces[player_id - 1][player_state - 2]);
    if (!surface)
    {
        cout << "Failed to create surface" << endl;
    }
    SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};
    int gap = SDL_GetTicks() - last_life_loss_time;
    if (gap > 2000 || (gap / 100) % 5 < 4)
    {
        if (!curr_text)
        {
            // cout << "Failed to create texture" << endl;
            SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
        }
    }
    SDL_DestroyTexture(curr_text);
}

string Player::render()
{
    if (!isAlive())
        player_state = 4;
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};
    int gap = SDL_GetTicks() - last_life_loss_time;
    if (gap > 2000 || (gap / 100) % 5 < 4)
    {
        return (to_string(player_id) + " " + to_string(rect.x) + " " + to_string(rect.y) + " " + to_string(rect.w) + " " + to_string(rect.h) + " " + to_string(player_state) + " ");
    }
    return "";
}