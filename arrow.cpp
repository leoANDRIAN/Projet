#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "arrow.h"

Arrow::Arrow(double w, double s)
{
    weight = w;
    size = s;
}

void Arrow::update(double delta_t)
{
    if (this->getAnim().getPos().z > -20) {
        this->getAnim().setPos(Point(this->getAnim().getPos().x, this->getAnim().getPos().y, this->getAnim().getPos().z - 0.07));
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


void Sphere::update(double delta_t)
{
    // this.getAnim().phi = delta_t;
}


void Sphere::render()
{
    GLUquadric* quad;

    quad = gluNewQuadric();

    Form::render();

    gluSphere(quad, this->getRadius(), 20, 20);

    gluDeleteQuadric(quad);
}
