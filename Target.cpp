#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "Target.h"

Target::Target(double r) {
	radius = r;
    moveLeft = true;
}

void Target::update(double delta_t)
{
    if (moveLeft) {
        if (this->getAnim().getPos().x > -7) {
            this->getAnim().setPos(Point(this->getAnim().getPos().x - 0.1, this->getAnim().getPos().y, this->getAnim().getPos().z));
        }
        else {
            moveLeft = false;
        }
    }
    else {
        if (this->getAnim().getPos().x < 7) {
            this->getAnim().setPos(Point(this->getAnim().getPos().x + 0.1, this->getAnim().getPos().y, this->getAnim().getPos().z));
        }
        else {
            moveLeft = true;
        }
    }
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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    gluQuadricTexture(quad, texture_id);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);
}