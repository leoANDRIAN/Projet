#pragma once
#include "forms.h"
class Arrow : public Form
{
private:
	// Liste des variables
	double weight, size;
public:
	// Liste des fonctions
	Arrow(double w, double s);
    double getWeight() const { return weight; }
    void setWeight(double w) { weight = w; }
    double getSize() const { return size; }
    void setSize(double s) { size = s; }
    void update(double delta_t);
    void render();
};





/*
// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
public:
    Sphere(double r = 1.0, Point org = Point(), Color cl = Color());
    double getRadius() const { return radius; }
    void setRadius(double r) { radius = r; }
    void update(double delta_t);
    void render();
};


// A face of a cube
class Cube_face : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
public:
    Cube_face(Vector v1 = Vector(1, 0, 0), Vector v2 = Vector(0, 0, 1),
        Point org = Point(), double l = 1.0, double w = 1.0,
        Color cl = Color());
    void update(double delta_t);
    void render();
};
*/

