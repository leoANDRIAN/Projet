#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "wind.h"

Wind::Wind(bool dir, int s)
{
    direction = dir; //True : wind from left, False : wind from right
    speed = s;
}

void Wind::update(double delta_t)
{
}


void Wind::render()
{
    GLUquadric* quad;
    quad = gluNewQuadric();
    Form::render();

    //direction
    if (direction == true) {
        glRotatef(-90.0, 0.0, 1.0, 0.0);
        getAnim().setPos(Point(-3.0, 3.0, -10.0));
    }else {
        glRotatef(90.0, 0.0, 1.0, 0.0);
        getAnim().setPos(Point(3.0, 3.0, -10.0));
    }

    //vitesse / couleur
    if (speed <= 3) {
        glColor4f(0, 1, 0, 0);
    }else if (speed <= 7) {
        glColor4f(1, 0.65, 0, 0);
    }else {
        glColor4f(1, 0, 0, 0);
    }

    //render arrow
    gluCylinder(quad, 0, 0.15, 0.3, 100, 100);
    glTranslatef(0, 0, 0.3);
    gluDisk(quad, 0, 0.15, 100, 100);
    gluCylinder(quad, 0.05, 0.05, 0.4, 100, 100);
    glTranslatef(0, 0, 0.4);
    gluDisk(quad, 0, 0.05, 100, 100);
}