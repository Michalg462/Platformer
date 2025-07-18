#ifndef PLAYER_H
#define PLAYER_H

#include "../Entity.h"
#include <vector>
#include <SDL.h>

class Player : public Entity{
    double health;
    SDL_Surface *sprite_sheet;      // Temporary (for testing) just one texture to display, I want to make it work first, then deal with animation
    std::vector<double> directions; // Value between 0 - 1, works like a speed modifier, the closer it gets to 1 the closer the speed is to max value
    int key_hold;                   // When the A(-1) or D(1) keys are pressed
    int last_dir;                   // Stores the last X axis direction where the player moved
    int jumps;                      // Number of jumps from the last landing, also used in animation
    int animation_frame;            // Current animation frame
    int animation_state;            // Will be used for animations not connected to movement, tbi
public:
    Player(double x, double y, double speed, double health, std::vector<double> directions);
    void check_collision(int other_x, int other_y, int other_width, int other_height);
    void move(double delta_time);
    void controls(const SDL_Event& event);

    void animation_manager(double delta_time);
    void animation_idle();
    void animation_running();
    void animation_airtime();

    void update(std::vector<std::vector<int>> static_elements, std::vector<Entity*> entities) override;
    SDL_Surface *get_sprite() const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
    bool is_Alive() const override;
    ~Player() override;
};



#endif //PLAYER_H
