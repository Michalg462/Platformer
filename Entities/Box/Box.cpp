#include "Box.h"
#include <iostream>

#include "../Player/Player.h"

#define BOX_WIDTH 28
#define BOX_HEIGHT 24

Box::Box(int x, int y, int hp) : Entity(x, y, BOX_WIDTH, BOX_HEIGHT, 0, true), hp(hp)
{
    collided = false;
    sprite_sheet = SDL_LoadBMP("src/box_idle.bmp");
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
    for (auto & entitie : entities)
    {
        if (dynamic_cast<Player*>(entitie) != nullptr)
        {
            check_collision(entitie);
        }

    }
}

SDL_Surface* Box::get_sprite() const
{
    return sprite_sheet;
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

Box::~Box()
{
    SDL_FreeSurface(sprite_sheet);
}

