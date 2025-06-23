#ifndef ENTITY_H
#define ENTITY_H



class Entity {
    protected:
    double x, y;
    double speed;
    public:
    Entity(double x, double y, double speed) : x(x), y(y), speed(speed) {};
    void check_collision();
    void update();
    void move(double delta_time);
};



#endif //ENTITY_H
