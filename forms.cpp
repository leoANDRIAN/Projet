#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "forms.h"


void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}


void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();
    glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);
    glRotated(this->getAnim().getTheta(), 1, 0, 0);
    glRotated(this->getAnim().getPhi(), 0, 1, 0);
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


Cube_face::Cube_face(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Cube_face::update(double delta_t)
{
    this->getAnim().setPhi(this->getAnim().getPhi() + 1);
    // this->getAnim().setTheta(this->getAnim().getTheta()+1);
}


void Cube_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length * vdir1);
    p3 = p2;
    p3.translate(width * vdir2);
    p4.translate(width * vdir2);

    Form::render();

    glBegin(GL_QUADS);
    {
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}