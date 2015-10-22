#pragma once

#include "Object3D.h"

class Plane3D : public Object3D
{
public:
	Plane3D(glm::vec3 pos, glm::vec2 rot, glm::vec2 dim, glm::vec3 col);
	~Plane3D();

	Intersection* testRayIntersection(Ray &r, float step, glm::vec3 &intersectionPoint);

	glm::vec2 dimensions;
};