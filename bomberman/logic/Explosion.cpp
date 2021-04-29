#include "Explosion.h"

Explosion::Explosion(int passed_id, int c1, int c2, int other, int direction, Maze &maze, int cur_time)
{
    block_size = maze.getBlockSize();
    min_coord = c1;
    max_coord = c2;
    const_coord = other;
    left_offset = maze.left_offset;
    top_offset = maze.top_offset;
    Explosion::direction = direction;
    duration = 2000;
    end_time = cur_time + duration;
    explosion_offset = 10;
    id = passed_id;
    create_actual_rectangle();
}
int Explosion::get_id()
{
    return id;
}
void Explosion::create_actual_rectangle()
{
    //creates a pixel rectangle
    if (direction == 0)
    {
        top_x = min_coord * block_size + left_offset + explosion_offset;
        top_y = const_coord * block_size + top_offset + explosion_offset;
        height = block_size - 2 * explosion_offset;
        width = (max_coord - min_coord + 1) * block_size - 2 * explosion_offset;
    }
    else
    {
        top_x = const_coord * block_size + left_offset + explosion_offset;
        top_y = min_coord * block_size + top_offset + explosion_offset;
        width = block_size - 2 * explosion_offset;
        height = (max_coord - min_coord + 1) * block_size - 2 * explosion_offset;
    }
}
vector<int> Explosion::update(int cur_ticks, vector<Explosion> &explosions, Maze &maze, vector<vector<int>> player_info)
{
    //make this return a vector of player id's !
    if (isOver(cur_ticks))
    {
        //remove the current explosion from the vectors.
        for (int i = 0; i < explosions.size(); i++)
        {
            if (explosions[i].get_id() == id)
            {
                explosions.erase(i + explosions.begin());
                break;
            }
        }
        vector<int> new_players;
        // for (int i = 0; i < player_info.size(); i++)
        // {
        //     new_players.push_back(player_info[i][0]);
        // }
        return new_players;
    }
    else
    {
        return kill(maze, player_info, cur_ticks);
    }
}
bool Explosion::isOver(int cur_ticks)
{
    if (cur_ticks > end_time)
        return true;
    else
        return false;
}
void Explosion::kill_blocks(Maze &maze, int x, int y, int current_time)
{
    int number = rand() % 6;
    if (number <= 1)
    {
        maze.update(y, x, 0, current_time);
    }
    else
    {
        maze.update(y, x, number + 1, current_time);
        maze.add_power_up(y, x);
    }
}
bool Explosion::check_player_killed(vector<int> x_coords, vector<int> y_coords)
{
    //for all points see if in region
    for (int i = 0; i < 5; i++)
    {
        if (x_coords[i] >= top_x && x_coords[i] <= top_x + width && y_coords[i] >= top_y && y_coords[i] <= top_y + height)
        {
            return true;
        }
    }
    return false;
}
vector<int> Explosion::kill(Maze &maze, vector<vector<int>> player_info, int current_time)
{
    //player_info contains player id, x,y,xoff, yoff ,playersize
    //destroy blocks and people
    //block will always be there at extreme ends.
    vector<vector<Box>> a = maze.getMaze();
    int x1, y1, x2, y2;
    if (direction == 0)
    {
        y1 = y2 = const_coord;
        x1 = min_coord;
        x2 = max_coord;
    }
    else
    {
        x1 = x2 = const_coord;
        y1 = min_coord;
        y2 = max_coord;
    }
    if (a[y1][x1].get_block_type() == 1)
    {
        kill_blocks(maze, x1, y1, current_time);
    }
    if (a[y2][x2].get_block_type() == 1)
    {
        kill_blocks(maze, x2, y2, current_time);
    }
    vector<int> new_players;
    //presently just removing from the vector
    for (int i = 0; i < player_info.size(); i++)
    {
        //check if player is in current rectangle, see 5 points
        int px1, px2, px3, py1, py2, py3; //clockwise + centre
        px1 = player_info[i][1] * block_size + player_info[i][3] + left_offset - (player_info[i][5]) / 2;
        px2 = px1 + player_info[i][5];
        px3 = (px1 + px2) / 2;
        py1 = player_info[i][2] * block_size + player_info[i][4] + top_offset - (player_info[i][5]) / 2;
        py2 = py1 + player_info[i][5];
        py3 = (py1 + py2) / 2;
        vector<int> x_coords = {px1, px2, px3, px3, px3};
        vector<int> y_coords = {py3, py3, py1, py3, py2};
        bool player_killed = check_player_killed(x_coords, y_coords);
        if (player_killed)
        {
            new_players.push_back(player_info[i][0]);
        }
    }
    return new_players;
}
void Explosion::render(SDL_Renderer *renderer, SDL_Surface *surface, vector<SDL_Surface *> explosion_surface)
{
    SDL_Rect rect = {top_x, top_y, width, height};
    int r, g, b, a;
    r = 150;
    g = 100;
    b = 220;
    a = 150;
    if (width > height)
        surface = explosion_surface[0];
    else
        surface = explosion_surface[1];
    if (!surface)
    {
        cout << "Failed to create surface" << endl;
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_Texture *curr_text = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, curr_text, nullptr, &rect);
        SDL_DestroyTexture(curr_text);
    }
}
string Explosion::render()
{
    SDL_Rect rect = {top_x, top_y, width, height};
    return ("6 " + to_string(rect.x) + " " + to_string(rect.y) + " " + to_string(rect.w) + " " + to_string(rect.h) + " ");
}