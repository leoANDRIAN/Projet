#pragma once
#include "forms.h"
class SpeedIndicator : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
    GLuint texture_id;
public:
    SpeedIndicator(Vector v1 = Vector(1, 0, 0), Vector v2 = Vector(0, 0, 1),
        Point org = Point(), double l = 25.0, double w = 25.0);
    void setTexture(GLuint textureid) { texture_id = textureid; }
    void update(double delta_t);
    void render();
};

