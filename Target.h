#pragma once
#include "forms.h"
class Target : public Form
{
private:
	// Liste des variables
	double radius;
public:
	// Liste des fonctions
	Target(double r);
	double getRadius() const { return radius; }
	void setRadius(double r) { radius = r; }
	void update(double delta_t);
	void render();
};

