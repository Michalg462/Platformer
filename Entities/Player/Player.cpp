#include "Player.h"
#include <cstdio>
#include <utility>
#include <SDL.h>
#include <iostream>

#define PLAYER_WIDTH 32     // In pixels
#define PLAYER_HEIGHT 32    // In pixels..
#define OTHER_SIZE 16       // In pixels...
#define GRAVITY_ACC_TIME 40 // How often will the gravity affect the player (in milliseconds)
#define GRAVITY_MOD 0.1     // How much will gravity affect the player (in directions scaling)
#define MAX_JUMPS 2         // Maximal number of jumps between landings

Player::Player(double x, double y, double speed, double health, std::vector<double> directions)
    : Entity(x, y, PLAYER_WIDTH, PLAYER_HEIGHT, speed, true), health(health), directions(std::move(directions))
{
    key_hold = 0;   // See Player.h for more details on the variables
    jumps = 0;
    sprite_sheet = SDL_LoadBMP("src/player_idle.bmp");
}

void Player::update(std::vector<std::vector<int>> static_elements, std::vector<Entity*> entities)
{
    /*
     *  This function updates player every frame
     *  It keeps rack of time between frames, calls position updates and collision checking functions
     *  static_elements - vector of all static elements on stage to check the collisions
     */

    // Those static variables are used to hold the time between frames
    static double prev_time = 0;
    static double gravity_time = 0;

    // Updating the time passed between frames
    double current_time = SDL_GetTicks();
    if (prev_time == 0){prev_time = current_time; return;}  // Will not calculate anything in the first frame, just pass the time
    double dt = current_time - prev_time;

    // Handling internal timers
    gravity_time += dt;
    prev_time = current_time;

    // This is the art where gravity is applied
    if (directions[1] < 1 && gravity_time >= GRAVITY_ACC_TIME)
    {
        directions[1] += GRAVITY_MOD;
        gravity_time -= GRAVITY_ACC_TIME;
    }

    // Going through whole vector of static elements and checking for collisions.
    for (auto & static_element : static_elements)
    {
        check_collision(static_element[0],static_element[1],OTHER_SIZE,OTHER_SIZE);
    }

    // The same check for all entities
    for (auto & entitie : entities)
    {
        if (dynamic_cast<Player*>(entitie) == nullptr)
        {
            check_collision(entitie->get_x(), entitie->get_y(), entitie->get_width(), entitie->get_height());
        }

    }

    // When all checked, update position
    move(dt);
}

void Player::move(double delta_time)
{
    /*
     *  Updates position of the player on the board according to time passed between frames
     *  delta_time - time in milliseconds that has passed between frames
     */
    this->x += this->directions[0] * delta_time/1000 * speed;
    this->y += this->directions[1] * delta_time/1000 * speed * 2;
}

void Player::controls(const SDL_Event& event)
{
    /*
     *  Handles all controls for the player. For now:
     *  [A] - move left
     *  [D] - move right
     *  [SPACE] - jump (if possible)
     *
     *  event - SDL_event object
     */
    if (event.type == SDL_KEYDOWN)
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_SPACE:
                if (jumps < MAX_JUMPS)
                {
                    jumps++;  // Increment number of jumps from the last landing
                    this->directions[1] = -1;
                }
                break;
            case SDLK_a:
                this->directions[0] = -1;
                key_hold = -1;
                break;
            case SDLK_d:
                this->directions[0] = 1;
                key_hold = 1;
                break;
            default: ;
        }
    }
    if (event.type == SDL_KEYUP)
    {
        switch( event.key.keysym.sym )
        {
        case SDLK_a:
            this->directions[0] = 0;
            key_hold = 0;
            break;
        case SDLK_d:
            this->directions[0] = 0;
            key_hold = 0;
            break;
        default: ;
        }
    }
}


void Player::check_collision(int other_x, int other_y, int other_width, int other_height)
{
    /*
     *  Checks for collision between player and given static obstacle. Fixes overlap if it exists.
     *  Collision with bottom of the player resets number of jumps.
     *
     *  All coordinates are of top-left corner, both of player and obstacle
     *
     *  other_x - X coordinate of obstacle
     *  other_y - Y coordinate of obstacle
     *  other_width - width of obstacle
     *  other_height - height of obstacle
     */

    // Calculates middle point of player
    int this_mid_x = this->x + PLAYER_WIDTH/2;
    int this_mid_y = this->y + PLAYER_HEIGHT/2;

    // Calculates middle point of obstacle
    int other_mid_x = other_x + other_width/2;
    int other_mid_y = other_y + other_height/2;

    // Calculates the difference between X and Y coordinates of both pints
    int x_diff = abs(this_mid_x - other_mid_x);
    int y_diff = abs(this_mid_y - other_mid_y);


    // First we check if the collision itself have happened. First check if X coordinates overlap, then the same for Y
    // If both overlap it means we have a collision
    if (this->x >= other_x - PLAYER_WIDTH && this->x <= other_x + other_width)
    {
        if (this->y >= other_y - PLAYER_HEIGHT && other_y + other_height >= this->y)
        {
            // Now we check in which direction have the collision occurred. First we check if t was on X or Y
            // We can get this from comparison of differences calculated before.
            // Larger diff is tells in which axis collision occurred
            if (x_diff >= y_diff)
            {
                // First calculate overlap and move the player outside the texture
                // (add or subtract from player x based on direction)
                int overlap_x = (PLAYER_WIDTH + other_width)/2 - x_diff;

                // Then we check the exact direction of collision and if player still tries to move there, we don't allow it
                if (this->x > other_x && directions[0] < 0)
                {
                    this->x += overlap_x;
                    directions[0] = 0;
                }
                if (this->x < other_x && directions[0] > 0)
                {
                    this->x -= overlap_x;
                    directions[0] = 0;
                }

            }
            else if (x_diff < y_diff)
            {
                // Same overlap as for X
                int overlap_y = (PLAYER_HEIGHT + other_height)/2 - y_diff;

                // Same logic as for X, but in Y axis
                if (this->y > other_y && directions[1] < 0)
                {
                    this->y += overlap_y;
                    directions[1] = 0;
                }
                if (this->y < other_y && directions[1] > 0)
                {
                    jumps = 0;
                    this->y -= overlap_y;
                    directions[1] = 0;
                }
            }
        }
    }
    else
    {
        // If no collision in X (we check it first), movement in this axis is revived
        directions[0] = key_hold;
    }
}

SDL_Surface* Player::get_sprite() const
{
    SDL_Surface *tmp = SDL_CreateRGBSurface(0, PLAYER_WIDTH, PLAYER_HEIGHT, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Rect src_rect = {0,0,PLAYER_WIDTH,PLAYER_HEIGHT};
    SDL_BlitSurface(sprite_sheet, &src_rect, tmp, nullptr);
    return tmp;
}

int Player::get_x() const
{
    return this->x;
}

int Player::get_y() const
{
    return this->y;
}

int Player::get_width() const
{
    return width;
}

int Player::get_height() const
{
    return height;
}

bool Player::is_Alive() const
{
    return isAlive;
}

Player::~Player()
{
    SDL_FreeSurface(sprite_sheet);
}
