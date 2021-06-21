#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "arrow.h"

Arrow::Arrow(double w, double s, Vector mov)
{
    weight = w;
    size = s;
    move = mov;
}

Arrow::Arrow(double w, double s)
{
    weight = w;
    size = s;
}

void Arrow::update(double delta_t)
{
    if (canMove) {
        // Movement
        Point newP = Point(anim.getPos().x + move.x, anim.getPos().y + move.y, anim.getPos().z + move.z);
        anim.setPos(newP);
        if (newP.x > 25 || newP.x < -25 || newP.y > 5 || newP.y < -4 || newP.z > 25 || newP.z < -25) {
            canMove = false;
        }
    }
}


void Arrow::render()
{
    GLUquadric* quad;
    quad = gluNewQuadric();
    Form::render();

    // Render spike
    glColor4f(0.60, 0.60, 0.60, 0);
    gluCylinder(quad, 0, 0.08, 0.2, 100, 100);
    glTranslatef(0, 0, 0.2);
    gluDisk(quad, 0, 0.08, 100, 100);

    // Render pipe
    glColor4f(0.54, 0.35, 0., 0);
    gluCylinder(quad, 0.01, 0.01, this->size, 100, 100);
    gluDeleteQuadric(quad);

    //Point org = this->getAnim().getPos();
    //glTranslated(org.x, org.y, org.z);
}
















Sphere::Sphere(double r, Point org, Color cl)
{
    anim.setPos(org);
    radius = r;
    col = cl;
}

Sphere::Sphere(Vector vec, double r, Point org, Color cl)
{
    move = vec;
    anim.setPos(org);
    radius = r;
    col = cl;
}

void Sphere::update(double delta_t)
{
    // Movement
    Point newP = Point(anim.getPos().x + move.x, anim.getPos().y + move.y, anim.getPos().z + move.z);
    anim.setPos(newP);
}


void Sphere::render()
{
    GLUquadric* quad;

    quad = gluNewQuadric();

    Form::render();

    gluSphere(quad, this->getRadius(), 20, 20);

    gluDeleteQuadric(quad);
}
