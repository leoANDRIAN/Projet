#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "arrow.h"
using namespace std;

Arrow::Arrow(double w, double s, Vector mov)
{
    weight = w;
    size = s;
    anim.setSpeed(mov);
}

Arrow::Arrow(double w, double s)
{
    weight = w;
    size = s;
}

void Arrow::update(double delta_t)
{
    if (this->canMove) {
        // Movement
        anim.setSpeed(anim.getSpeed() + delta_t * anim.getAccel());
        Vector newVec = delta_t * anim.getSpeed();
        Point newP = Point(anim.getPos().x + newVec.x, anim.getPos().y + newVec.y, anim.getPos().z + newVec.z);
        anim.setPos(newP);
        // Check fleche dans salle
        if (newP.x > 25 || newP.x < -25 || newP.y > 5 || newP.y < -4 || newP.z > 25 || newP.z < -25) {
            canMove = false;
        }
        // Check si fleche a touché la cible
        if (newP.x == 40000) {
            canMove = false;
            onCible = true;
        }
    }
    else if (onCible) {

    }
}


void Arrow::render()
{
    glPushMatrix();
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
    glColor4f(1,1,1, 0);
    glPopMatrix();

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

void Sphere::update(double delta_t) {
    anim.setSpeed(anim.getSpeed() + delta_t * anim.getAccel());
    Vector newVec = delta_t * anim.getSpeed();
    Point newP = Point(anim.getPos().x + newVec.x, anim.getPos().y + newVec.y, anim.getPos().z + newVec.z);
    // cout << newP << endl;
    anim.setPos(newP);
}


void Sphere::render()
{
    GLUquadric* quad;
    quad = gluNewQuadric();

    // Complete this part
    Form::render(); // Position the form and assign its color

    // Mise en route de la texture associee
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    gluQuadricTexture(quad, texture_id);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluSphere(quad, radius, 20, 20);

    gluDeleteQuadric(quad);

    // Ne plus appliquer la texture pour la suite
    glDisable(GL_TEXTURE_2D);
}
