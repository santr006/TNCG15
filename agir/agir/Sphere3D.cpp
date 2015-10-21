#include "Sphere3D.h"

Sphere3D::Sphere3D(glm::vec3 &pos, glm::vec2 &rot, float r, glm::vec3 col)
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

bool Sphere3D::testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint)
{
	glm::vec3 L = position - r.startPosition;
	float tca = glm::dot(L, r.direction);
	if (tca < 0) 
		return false;

	float d2 = glm::dot(L, L) -tca * tca;
	if (d2 > radius * radius) 
		return false;
	float thc = sqrt(radius * radius - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;

	intersectionPoint = r.startPosition + t0 * r.direction;
	return true;
}