#ifndef ENTITY_H
#define ENTITY_H



class Entity {
    protected:
    int x, y;
    double speed;
    public:
    Entity(int x, int y, int speed) : x(x), y(y), speed(speed) {};
    void check_collision();
    void update();
    void move(double delta_time);
};



#endif //ENTITY_H
