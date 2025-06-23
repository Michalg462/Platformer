#include "Player.h"
#include <cstdio>
#include <utility>
#include <SDL.h>
#include <iostream>

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

Player::Player(double x, double y, double speed, double health, std::vector<double> directions)
    : Entity(x, y, speed), health(health), directions(std::move(directions))
{
    sprite_sheet = SDL_LoadBMP("src/player_idle.bmp");
}

void Player::update()
{
    static double prev_time = 0;
    double current_time = SDL_GetTicks();
    if (prev_time == 0){prev_time = current_time; return;}
    double dt = current_time - prev_time;
    prev_time = current_time;
    move(dt);
    check_collision(64,64,16,16);
}

void Player::move(double delta_time)
{
    this->x += this->directions[0] * delta_time/1000 * speed;
    this->y += this->directions[1] * delta_time /1000 * speed;
}

void Player::controls(SDL_Event event)
{
    /*
     * Will be remade better, just for testing the collisions with normal movement (x and y modifiers)
     */
    if (event.type == SDL_KEYDOWN)
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_w:
                this->directions[1] = -1;
                break;
            case SDLK_s:
                this->directions[1] = 1;
                break;
            case SDLK_a:
                this->directions[0] = -1;
                break;
            case SDLK_d:
                this->directions[0] = 1;
                break;
            default: ;
        }
    }
    if (event.type == SDL_KEYUP)
    {
        switch( event.key.keysym.sym )
        {
        case SDLK_w:
            this->directions[1] = 0;
            break;
        case SDLK_s:
            this->directions[1] = 0;
            break;
        case SDLK_a:
            this->directions[0] = 0;
            break;
        case SDLK_d:
            this->directions[0] = 0;
            break;
        default: ;
        }
    }
}


void Player::check_collision(int other_x, int other_y, int other_width, int other_height)
{
    int this_mid_x = this->x + PLAYER_WIDTH/2;
    int this_mid_y = this->y + PLAYER_HEIGHT/2;

    int other_mid_x = other_x + other_width/2;
    int other_mid_y = other_y + other_height/2;

    int x_diff = abs(this_mid_x - other_mid_x);
    int y_diff = abs(this_mid_y - other_mid_y);

    if (this->x > other_x - PLAYER_WIDTH && this->x < other_x + other_width)
    {
        if (this->y > other_y - PLAYER_HEIGHT && other_x + other_height > this->y)
        {
            if (x_diff > y_diff)
            {
                directions[0] = 0;
            }
            else if (x_diff < y_diff)
            {
                directions[1] = 0;
            }
        }
    }
    // if (this->x > other_x - PLAYER_WIDTH && this->x < other_x + other_width)
    // {
    //     if (this->y > other_y - PLAYER_HEIGHT && other_x + other_height > this->y)
    //     {
    //         if (directions[0] == 0)
    //         {
    //             directions[1] = 0;
    //         }
    //         else if (directions[1] == 0)
    //         {
    //             directions[0] = 0;
    //         }
    //         else
    //         {
    //             int player_center_x = x + PLAYER_WIDTH / 2;
    //             int player_center_y = y + PLAYER_HEIGHT / 2;
    //             int other_center_x = other_x + other_width / 2;
    //             int other_center_y = other_y + other_height / 2;
    //
    //             int dx = player_center_x - other_center_x;
    //             int dy = player_center_y - other_center_y;
    //
    //             if (directions[0] < 0)
    //             {
    //                 if (directions[1] < 0)
    //                 {
    //
    //                 }
    //                 else
    //                 {
    //
    //                 }
    //             }
    //             else
    //             {
    //                 if (directions[1] < 0)
    //                 {
    //
    //                 }
    //                 else
    //                 {
    //
    //                 }
    //             }
    //         }
    //     }
    // }
}

SDL_Surface* Player::get_sprite()
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

Player::~Player()
{
    SDL_FreeSurface(sprite_sheet);
}
