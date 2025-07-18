#include "Box.h"
#include <iostream>

#include "../Player/Player.h"

#define BOX_WIDTH 28
#define BOX_HEIGHT 24

Box::Box(int x, int y, int hp) : Entity(x, y, BOX_WIDTH, BOX_HEIGHT, 0, true), hp(hp)
{
    collided = false;       // Holds the info if the collision was already read
    play_hit_animation = false;     // Holds the info if the hit animation was already played
    sprite_sheet = SDL_LoadBMP("src/box_one.bmp");  // Reading Box spreadsheet into memory
    sprite = SDL_CreateRGBSurface(0, BOX_WIDTH, BOX_HEIGHT, 32,
    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);    // Preparing surface that will be returned as sprite (current frame)
    SDL_Rect src_rect = {0,0,BOX_WIDTH,BOX_HEIGHT};
    SDL_BlitSurface(sprite_sheet, &src_rect, sprite, nullptr);  // Drawing default image.
}

void Box::check_collision(Entity* other)
{
    /*
     * checks for collision with other entity
     * other - entity against which the collision is checked
     *
     * collision check is checked the same as in Player
     */
    if (this->x >= other->get_x() - this->width && this->x <= other->get_x() + other->get_width())
    {
        if (this->y >= other->get_y() - this->height && other->get_y() + other->get_height() >= this->y)
        {
            if (!collided)
            {
                // If collision flag wasn't already raised, it gets raised, animation is allowed, and hit points are lowered.
                collided = true;
                play_hit_animation = true;
                this->hp -= 1;
                if (this->hp <= 0)
                {
                    // if HP dropped to 0, the box is marked as dead
                    isAlive = false;
                }
            }
            return;
        }
    }
    // If no collision occurred lower the collision flag
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
            check_collision(entitie);   // Checks collision against all other entities # For testing just Player #
        }

    }
    // If the animation allowed it gets played.
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
    static int frame = 1; // Holds the info about the frame that is now displayed
    static double animation_timer = 50; // time that has passed from the last frame change
    animation_timer += delta_time;
    if (animation_timer >= 50 && frame < 4)
    {
        // If the time between frames has passed and the animation should be continued, the sprite gets updated as a
        // current animation frame and the frame changed to the next
        SDL_Rect src_rect{frame * BOX_WIDTH,0,BOX_WIDTH,BOX_HEIGHT};
        SDL_FillRect(sprite, nullptr, SDL_MapRGBA(sprite->format, 0, 0, 0, 0));
        SDL_BlitSurface(sprite_sheet, &src_rect, sprite, nullptr);
        frame++;
        animation_timer -= 50;
    }
    if (frame >= 4)
    {
        // If the animation has finished, go back to default sprite, and lower animation flag
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

