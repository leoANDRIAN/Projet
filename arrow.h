#pragma once
#include "forms.h"
#include "Target.h"
class Arrow : public Form
{
private:
	// Liste des variables
	double weight, size;
    Vector move;
public:
    bool canMove = true;
    bool onCible = false;
    double oldProdVec;
	// Liste des fonctions
	Arrow(double w, double s);
    Arrow(double w, double s, Vector mov);
    double getWeight() const { return weight; }
    void setWeight(double w) { weight = w; }
    double getSize() const { return size; }
    void setSize(double s) { size = s; }
    void update(double delta_t);
    void update2(double delta_t, Target *cible);
    void render();
};






// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
    GLuint texture_id;
public:
    Vector move;
    double masse;
    Sphere(double r = 1.0, Point org = Point(), Color cl = Color());
    Sphere(Vector vec, double r = 1.0, Point org = Point(), Color cl = Color());
    double getRadius() const { return radius; }
    void setRadius(double r) { radius = r; }
    void setTexture(GLuint textureid) { texture_id = textureid; }
    void update(double delta_t);
    void render();
};

