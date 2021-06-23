#pragma once
#include "forms.h"
class Target : public Form
{
private:
	// Liste des variables
	double radius;
	bool moveLeft;
	GLuint texture_id;
public:
	// Liste des fonctions
	Target(double r);
	double getRadius() const { return radius; }
	void setRadius(double r) { radius = r; }
	void setTexture(GLuint textureid) { texture_id = textureid; }
	void update(double delta_t);
	void render();
};

