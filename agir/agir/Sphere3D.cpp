#include "Sphere3D.h"

// "inspiration" from http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

Intersection* Sphere3D::rayIntersection(Ray &r)
{
	float t0, t1;
	glm::vec3 point, surfaceNormal;

	glm::vec3 L = center - r.startPosition;

	float tca = glm::dot(L, r.direction);
	if (tca < 0){ return nullptr; }

	float d2 = glm::dot(L, L) - tca*tca;
	if (d2 > radius*radius) { return nullptr; }

	float thc = sqrt(radius*radius - d2);
	t0 = tca - thc; //front
	t1 = tca + thc; //back

	// object in front
	glm::vec3 front(t0);
	//glm::vec3 back(t1);

	// if hit
	return new Intersection(front, glm::normalize(front - center), color);
}