#include "Sphere3D.h"

Sphere3D::Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, Rgb &col)
{
	position = pos;
	rotation = rot;
	radius = r;
	color = col;
}

Sphere3D::~Sphere3D()
{
	// so long sphere!
}

bool Sphere3D::testRayIntersection(Ray &r)
{
	// Code MOAR here!
	return false;
}