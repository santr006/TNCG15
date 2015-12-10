#pragma once

#include "Object3D.h"

class Plane3D : public Object3D
{
public:
	Plane3D();
	Plane3D(glm::vec3 pos, glm::vec3 rot, glm::vec2 dim, glm::vec3 col);
	Plane3D(glm::vec3 pos, glm::vec3 rot, glm::vec2 dim, glm::vec3 col, float reflecCoef);
	Plane3D(glm::vec3 pos, glm::vec3 rot, glm::vec2 dim, glm::vec3 col, bool trans);
	~Plane3D();

	Intersection* testRayIntersection(Ray r);
	Intersection* testRayIntersectionInside(Ray r);

	glm::vec2 dimensions;
};