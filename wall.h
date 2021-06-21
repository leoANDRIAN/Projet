#pragma once
#include "forms.h"

class Wall : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
public:
    Wall(Vector v1 = Vector(1, 0, 0), Vector v2 = Vector(0, 0, 1),
        Point org = Point(), double l = 25.0, double w = 25.0,
        Color cl = Color());
    void update(double delta_t);
    void render();
};

