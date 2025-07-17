#include "Box.h"
#include <iostream>

#include "../Player/Player.h"

#define BOX_WIDTH 28
#define BOX_HEIGHT 24

Box::Box(int x, int y, int hp) : Entity(x, y, BOX_WIDTH, BOX_HEIGHT, 0, true), hp(hp)
{
    collided = false;
    play_hit_animation = false;
    //sprite_sheet = SDL_LoadBMP("src/box_one.bmp");
    sprite_sheet = SDL_LoadBMP("src/box_one.bmp");
    sprite = SDL_CreateRGBSurface(0, BOX_WIDTH, BOX_HEIGHT, 32,
    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Rect src_rect = {0,0,BOX_WIDTH,BOX_HEIGHT};
    SDL_BlitSurface(sprite_sheet, &src_rect, sprite, nullptr);
}

void Box::check_collision(Entity* other)
{
    if (this->x >= other->get_x() - this->width && this->x <= other->get_x() + other->get_width())
    {
        if (this->y >= other->get_y() - this->height && other->get_y() + other->get_height() >= this->y)
        {
            if (!collided)
            {
                collided = true;
                play_hit_animation = true;
                this->hp -= 1;
                if (this->hp <= 0)
                {
                    isAlive = false;
                }
            }
            return;
        }
    }
    collided = false;
}

void Box::update(std::vector<std::vector<int>> static_elements, std::vector<Entity*> entities)
{
    static double prev_time = 0;

    // Updating the time passed between frames
    double current_time = SDL_GetTicks();
    if (prev_time == 0){prev_time = current_time; return;}  // Will not calculate anything in the first frame, just pass the time
    double dt = current_time - prev_time;

    prev_time = current_time;
    for (auto & entitie : entities)
    {
        if (dynamic_cast<Player*>(entitie) != nullptr)
        {
            check_collision(entitie);
        }

    }
    if (play_hit_animation){animation_hit(dt);}
}

SDL_Surface* Box::get_sprite() const
{
    return sprite;
}

int Box::get_x() const
{
    return x;
}

int Box::get_y() const
{
    return y;
}

int Box::get_width() const
{
    return width;
}

int Box::get_height() const
{
    return height;
}

bool Box::is_Alive() const
{
    return isAlive;
}

void Box::animation_hit(double delta_time)
{
    static int frame = 1;
    static double animation_timer = 50;
    animation_timer += delta_time;
    if (animation_timer >= 50 && frame < 4)
    {
        SDL_Rect src_rect{frame * BOX_WIDTH,0,BOX_WIDTH,BOX_HEIGHT};
        SDL_FillRect(sprite, nullptr, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
        SDL_BlitSurface(sprite_sheet, &src_rect, sprite, nullptr);
        frame++;
        animation_timer -= 50;
    }
    if (frame >= 4)
    {
        SDL_Rect src_rect{0,0,BOX_WIDTH,BOX_HEIGHT};
        SDL_FillRect(sprite, nullptr, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
        SDL_BlitSurface(sprite_sheet, &src_rect, sprite, nullptr);
        frame = 0;
        animation_timer = 50;
        play_hit_animation = false;
    }
}

Box::~Box()
{
    SDL_FreeSurface(sprite_sheet);
}

