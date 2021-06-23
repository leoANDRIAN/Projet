#pragma once
#include "forms.h"
class Wind : public Form
{
private:
    // Liste des variables
    bool direction; //True : wind from left, False : wind from right
    int speed;
public:
    // Liste des fonctions
    Wind(bool dir, int s);
    bool getDirection() const { return direction; }
    void setDirection(bool dir) { direction = dir; }
    int getSpeed() const { return speed; }
    void setSpeed(int s) { s = speed; }
    void update(double delta_t);
    void render();
};

