#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "arrow.h"
#include "Target.h"

using namespace std;

Arrow::Arrow(double w, double s, Vector mov, Target *cible)
{
    weight = w;
    size = s;
    anim.setSpeed(mov);
    target = cible;
    Vector v(anim.getPos(), cible->getAnim().getPos()); // Vecteur pointe / centre cible
    oldProdVec = (v * cible->getNormal()) / (v.norm() * cible->getNormal().norm());
}

Arrow::Arrow(double w, double s)
{
    weight = w;
    size = s;
}

void Arrow::update(double delta_t) {
    if (canMove) {
        // Movement
        anim.setSpeed(anim.getSpeed() + delta_t * anim.getAccel());
        Vector newVec = delta_t * anim.getSpeed();
        Point newP = Point(anim.getPos().x + newVec.x, anim.getPos().y + newVec.y, anim.getPos().z + newVec.z);
        anim.setPos(newP);
        // Check fleche dans salle
        if (newP.x > 40 || newP.x < -40 || newP.y > 5 || newP.y < -4 || newP.z > 40 || newP.z < -40) {
            canMove = false;
        }
        // Check si fleche a touch� la cible
        Vector v(anim.getPos(), target->getAnim().getPos()); // Vecteur pointe / centre cible
        double prodVec = (v * target->getNormal()) / (v.norm() * target->getNormal().norm());
        if (((prodVec < 0 && oldProdVec > 0) || (prodVec > 0 && oldProdVec < 0)) && (v.norm() <= target->getRadius())) {
            canMove = false;
            onCible = true;
        }
        oldProdVec = prodVec;
    }
    else if (onCible && !target->pause) {
        if (target->moveLeft) {
            this->getAnim().setPos(Point(this->getAnim().getPos().x - 0.1, this->getAnim().getPos().y, this->getAnim().getPos().z));
        }
        else {
            this->getAnim().setPos(Point(this->getAnim().getPos().x + 0.1, this->getAnim().getPos().y, this->getAnim().getPos().z));
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
