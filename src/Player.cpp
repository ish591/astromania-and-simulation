#include "Player.h"

Player::Player(int id)
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
}

void Player::updateDimensions(Maze maze, int w, int h)
{
    block_size = maze.getBlockSize();
    player_size = block_size - 5;
    if (player_size % 2 == 1)
        player_size--;

    left_offset = maze.left_offset;
    top_offset = maze.top_offset;
    x_offset = block_size / 2;
    y_offset = block_size / 2;
    move_size = 1;
}

void Player::takeAction(SDL_Event event)
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

void Player::updateLocation(Maze maze)
{
    int hmove = RIGHT_PRESSED - LEFT_PRESSED;
    int vmove = DOWN_PRESSED - UP_PRESSED;
    vector<vector<int>> a = maze.getMaze();
    // cout << hmove << vmove << endl;
    switch (hmove)
    {
    case -1:
        if (x_offset - move_size - player_size / 2 > 0)
        {
            x_offset -= move_size;
        }
        else if (a[y][x - 1] == 0)
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
        else if (a[y][x + 1] == 0)
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
        else if (a[y - 1][x] == 0)
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
        else if (a[y + 1][x] == 0)
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

void Player::render(SDL_Renderer *renderer, Maze maze)
{
    updateLocation(maze);
    SDL_Rect rect = {x * block_size + x_offset + left_offset - (player_size / 2), y * block_size + y_offset + top_offset - (player_size / 2), player_size, player_size};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
