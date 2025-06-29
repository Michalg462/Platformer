#ifndef BOX_H
#define BOX_H
#include "../Entity.h"


class Box : public Entity{
    int hp;
    bool collided;
    SDL_Surface *sprite_sheet;
public:
    Box(int x, int y, int hp);
    void check_collision(Entity* other);

    void update(std::vector<std::vector<int>> static_elements, std::vector<Entity*> entities) override;
    SDL_Surface *get_sprite() const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
    bool is_Alive() const override;

    ~Box() override;
};



#endif //BOX_H
