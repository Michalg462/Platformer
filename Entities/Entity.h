#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <SDL.h>

class Entity {
    protected:
    double x, y;
    int width, height;
    double speed;
    bool isAlive;
    public:
    virtual ~Entity() = default;
    Entity(double x, double y, int width, int height, double speed, bool is_Alive)
        : x(x), y(y), width(width), height(height), speed(speed), isAlive(is_Alive) {};
    virtual void update(std::vector<std::vector<int>> static_elements, std::vector<Entity*> entities) = 0;
    virtual SDL_Surface *get_sprite() const = 0;
    virtual int get_x() const = 0;
    virtual int get_y() const = 0;
    virtual int get_width() const = 0;
    virtual int get_height() const = 0;
    virtual bool is_Alive() const = 0;
};



#endif //ENTITY_H
