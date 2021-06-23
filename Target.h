#pragma once
#include "forms.h"
class Target : public Form
{
private:
	// Liste des variables
	double radius;
public:
	bool moveLeft;
	bool pause = false;
	// Vecteurs pour calcul de la normale
	Vector v1 = Vector(1, 0, 0);
	Vector v2 = Vector(0, 1, 0);
	// Liste des fonctions
	Target(double r);
	double getRadius() const { return radius; }
	void setRadius(double r) { radius = r; }
	void update(double delta_t);
	void render();
};

