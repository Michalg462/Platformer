#include "Player.h"
#include <cstdio>
#include <utility>
#include <SDL.h>

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

Player::Player(int x, int y, double speed, double health, std::vector<double> directions)
    : Entity(x, y, speed), health(health), directions(std::move(directions))
{
    sprite_sheet = SDL_LoadBMP("src/player_idle.bmp");
}

void Player::update()
{
    // For testing of collision, follows the mouse coordinates, will be gone soon
    int x, y;
    SDL_GetMouseState(&x, &y);
    this->x = x;
    this->y = y;
    check_collision(256,256,32,32);
}

void Player::move(double delta_time)
{
    this->x += this->directions[0] * delta_time * speed;
    this->y += this->directions[1] * delta_time * speed;
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
    if (this->x > other_x - PLAYER_WIDTH && this->x < other_x + other_width)
    {
        if (this->y > other_y - PLAYER_HEIGHT && other_x + other_height > this->y)
        {
            printf("Colision!\n");
        }
    }
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
