#pragma once
#include "Object3D.h"
#include "Plane3D.h"

class Box3D : public Object3D
{
public:
	Box3D(glm::vec3 pos, glm::vec3 rot, glm::vec3 dim, glm::vec3 col, float reflecCoef);
	~Box3D();

	Intersection* testRayIntersection(Ray r);

	glm::vec3 dimensions;
	Plane3D front;
	Plane3D back;
	Plane3D right;
	Plane3D left;
	Plane3D top;
	Plane3D bottom;
};