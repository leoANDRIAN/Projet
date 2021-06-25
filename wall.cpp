#include <cmath>
#include<gl/glut.h>
#include<SDL.h>
#include "wall.h"

Wall::Wall(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Wall::update(double delta_t)
{
}


void Wall::render()
{
    // Les quatre coins de la face
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length * vdir1);
    p3 = p2;
    p3.translate(width * vdir2);
    p4.translate(width * vdir2);

    // Autorisation de la texture choisie a la creation de la face (cf main())
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Actions communes a toutes les formes
    Form::render();
    glBegin(GL_QUADS);
    {
        //All subsequent vertices will have an associated texture coordinate (coord x y).
        glTexCoord2f(0.0f, 0.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p1.x, p1.y, p1.z);
        glTexCoord2f(1.0f, 0.0f); //All subsequent vertices will have an associated texture coordinate of (1,0)
        glVertex3d(p2.x, p2.y, p2.z);
        glTexCoord2f(1.0f, 1.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p3.x, p3.y, p3.z);
        glTexCoord2f(0.0f, 1.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();

    // Ne plus appliquer la texture pour la suite
    glDisable(GL_TEXTURE_2D);

}