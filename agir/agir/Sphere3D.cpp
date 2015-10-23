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

Intersection* Sphere3D::testRayIntersection(Ray& r, float step, glm::vec3 &intersectionPoint)
{
	glm::vec3 L = position - r.startPosition;
	float tca = glm::dot(L, r.direction);
	if (tca < 0)
		return nullptr;

	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > radius * radius)
		return nullptr;

	float thc = sqrt(radius * radius - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 > t1){
		t0 = t1;
		if (t0 < 0)
			return nullptr;
	}

	// If the object is blocked
	if (t0 > r.tMax)
		return nullptr;
	r.tMax = t0;

	intersectionPoint = r.startPosition + t0 * r.direction;
//	return new Intersection(intersectionPoint, glm::normalize(position - intersectionPoint), color); // as it should be (?)
	return new Intersection(intersectionPoint, glm::normalize(intersectionPoint - position), color); // flip'd direction
}