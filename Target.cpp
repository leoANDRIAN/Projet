#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "Target.h"

Target::Target(double r) {
	radius = r;
}

void Target::update(double delta_t)
{
    
}


void Target::render()
{
    GLUquadric* quad;
    quad = gluNewQuadric();
    Form::render();
    glColor4f(1., 1., 1., 0);
    gluDisk(quad, (this->radius*3)/4, this->radius, 100, 100);
    glColor4f(1., 0., 0., 0);
    gluDisk(quad, this->radius/2, (this->radius*3)/4, 100, 100);
    glColor4f(1., 1., 1., 0);
    gluDisk(quad, this->radius/4, this->radius/2, 100, 100);
    glColor4f(1., 0., 0., 0);
    gluDisk(quad, 0, this->radius/4, 100, 100);
    gluDeleteQuadric(quad);
}