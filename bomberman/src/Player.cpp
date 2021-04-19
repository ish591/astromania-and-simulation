#include "Player.h"

Player::Player(int id, Maze &maze)
{
    player_id = id;
    x = 1;
    y = 1;
    x_offset = 0, y_offset = 0;
    score = 0;
    UP_PRESSED = 0;
    LEFT_PRESSED = 0;
    DOWN_PRESSED = 0;
    RIGHT_PRESSED = 0;
    SPACE_PRESSED = 0;
    bomb_count = 0;
    total_released = 0;
    for (int i = 0; i < 5; i++)
    {
        power_ups.push_back(false);
    }
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
int Player::get_bomb_count()
{
    return bomb_count;
}
void Player::update_bomb_count(int new_count)
{
    bomb_count = new_count;
}

// void Player::reset_bombs(Maze &maze, int x1, int y1)
// {
//     released = false;
//     bombs.clear();
//     collision_status.clear();
//     for (int i = 0; i < 4; i++)
//     {
//         collision_status.push_back(false);
//         Bomb new_bomb = Bomb(maze, bomb_type, i + 1, x1, y1, x_offset, y_offset); //i+1 denotes the direction
//         bombs.push_back(new_bomb);
//     }
// }

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
    move_size = 1;
}

void Player::takeAction(SDL_Event event, Maze &maze, vector<Bomb> &bombs, int current_time)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            LEFT_PRESSED = 1;
            break;
        case SDLK_RIGHT:
            RIGHT_PRESSED = 1;
            break;
        case SDLK_UP:
            UP_PRESSED = 1;
            break;
        case SDLK_DOWN:
            DOWN_PRESSED = 1;
            break;
        case SDLK_SPACE:
            if (bomb_count == 0) //then only release
            {
                //currently setting the type as 1 only
                total_released++;
                Bomb new_bomb = Bomb(maze, 1, x, y, block_size / 2, block_size / 2, current_time, player_id, total_released);
                bombs.push_back(new_bomb);
            }
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            LEFT_PRESSED = 0;
            break;
        case SDLK_RIGHT:
            RIGHT_PRESSED = 0;
            break;
        case SDLK_UP:
            UP_PRESSED = 0;
            break;
        case SDLK_DOWN:
            DOWN_PRESSED = 0;
            break;
        default:
            break;
        }
        break;
    }
}

void Player::updateLocation(Maze &maze, vector<Player> &players, vector<Bomb> &bombs, int current_time)
{
    update_bombs(maze, players, bombs, current_time);
    //new bombs have been released, and collision with bombs have been checked.
    //now, handle the sliding feature. if next cell has a slidable bomb, slide it
    int hmove = RIGHT_PRESSED - LEFT_PRESSED;
    int vmove = DOWN_PRESSED - UP_PRESSED;
    vector<vector<Box> > a = maze.getMaze();
    // cout << hmove << vmove << endl;
    switch (hmove)
    {
    case -1:
        if (x_offset - move_size - player_size / 2 > 0)
        {
            x_offset -= move_size;
        }
        else if (a[y][x - 1].get_block_type() == 0)
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
                            if (x_offset - move_size < 0)
                                x--;
                            x_offset = (x_offset + block_size - move_size) % block_size;
                        }
                        else
                        {
                            x_offset = player_size / 2 + 1;
                        }
                    }
                    else
                    {
                        if (x_offset - move_size < 0)
                            x--;
                        x_offset = (x_offset + block_size - move_size) % block_size;
                    }
                }
                else
                {
                    x_offset = player_size / 2 + 1;
                }
            }
            else
            {
                if (x_offset - move_size < 0)
                    x--;
                x_offset = (x_offset + block_size - move_size) % block_size;
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
        if (x_offset + move_size + player_size / 2 < block_size)
        {
            x_offset += move_size;
        }
        else if (a[y][x + 1].get_block_type() == 0)
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
                            if (x_offset + move_size >= block_size)
                                x++;
                            x_offset = (x_offset + move_size) % block_size;
                        }
                        else
                        {
                            x_offset = block_size - 1 - player_size / 2;
                        }
                    }
                    else
                    {
                        if (x_offset + move_size >= block_size)
                            x++;
                        x_offset = (x_offset + move_size) % block_size;
                    }
                }
                else
                {
                    x_offset = block_size - 1 - player_size / 2;
                }
            }
            else
            {
                if (x_offset + move_size >= block_size)
                    x++;
                x_offset = (x_offset + move_size) % block_size;
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
        if (y_offset - move_size - player_size / 2 > 0)
        {
            y_offset -= move_size;
        }
        else if (a[y - 1][x].get_block_type() == 0)
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
                            if (y_offset - move_size < 0)
                                y--;
                            y_offset = (y_offset + block_size - move_size) % block_size;
                        }
                        else
                        {
                            y_offset = player_size / 2 + 1;
                        }
                    }
                    else
                    {
                        if (y_offset - move_size < 0)
                            y--;
                        y_offset = (y_offset + block_size - move_size) % block_size;
                    }
                }
                else
                {
                    y_offset = player_size / 2 + 1;
                }
            }
            else
            {
                if (y_offset - move_size < 0)
                    y--;
                y_offset = (y_offset + block_size - move_size) % block_size;
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
        if (y_offset + move_size + player_size / 2 < block_size)
        {
            y_offset += move_size;
        }
        else if (a[y + 1][x].get_block_type() == 0)
        {
            if (a[y - 1][x + 1].get_block_type() >= 1)
            {
                if (x_offset <= block_size - player_size / 2)
                {
                    if (a[y - 1][x - 1].get_block_type() >= 1)
                    {
                        if (x_offset >= player_size / 2)
                        {
                            if (y_offset + move_size >= block_size)
                                y++;
                            y_offset = (y_offset + move_size) % block_size;
                        }
                        else
                        {
                            y_offset = block_size - 1 - player_size / 2;
                        }
                    }
                    else
                    {
                        if (y_offset + move_size >= block_size)
                            y++;
                        y_offset = (y_offset + move_size) % block_size;
                    }
                }
                else
                {
                    y_offset = block_size - 1 - player_size / 2;
                }
            }
            else
            {
                if (y_offset + move_size >= block_size)
                    y++;
                y_offset = (y_offset + move_size) % block_size;
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
void Player::update_bombs(Maze &maze, vector<Player> &players, vector<Bomb> &bombs, int current_time)
{
    //first of all update all the bombs
    // if a bomb explodes, remove it from this vector.
    // also, decrement count of player
    vector<Bomb> new_bombs;
    vector<pair<int, int> > locations;
    for (Player u : players)
    {
        locations.push_back({u.get_x(), u.get_y()});
    }
    for (Bomb u : bombs)
    {
        pair<bool, int> res = u.update_state(current_time, maze, locations, bombs);
        if (!res.first) //bomb has exploded
        {
            new_bombs.push_back(u);
        }
        else
        {
            for (Player u : players)
            {
                if (u.getId() == res.second)
                {
                    u.update_bomb_count(u.get_bomb_count() - 1);
                }
            }
        }
    }
    bombs = new_bombs;
    //all old bombs have been updated and explosions, collissions checked.
    // now time for releasing a new bomb!
}

void Player::render(SDL_Renderer *renderer)
{
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
