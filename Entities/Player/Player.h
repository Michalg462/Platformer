#ifndef PLAYER_H
#define PLAYER_H

#include "../Entity.h"
#include <vector>
#include <SDL.h>

class Player : public Entity{
    double health;
    SDL_Surface *sprite_sheet;      // Temporary (for testing) just one texture to display, I want to make it work first, then deal with animation
    std::vector<double> directions; // Value between 0 - 1, works like a speed modifier, the closer it gets to 1 the closer the speed is to max value
public:
    Player(int x, int y, double speed, double health, std::vector<double> directions);
    void check_collision(int other_x, int other_y, int other_width, int other_height);
    void update();
    void move(double delta_time);
    void controls(SDL_Event event);
    SDL_Surface *get_sprite();
    int get_x() const;
    int get_y() const;
    ~Player();
};



#endif //PLAYER_H
