#pragma once
#include "Object3D.h"
#include "Plane3D.h"

#include <gtc/constants.hpp>

class Box3D : public Object3D
{
public:
	Box3D(glm::vec3 pos, glm::vec3 rot, glm::vec3 dim, glm::vec3 col);
	~Box3D();

	Intersection* testRayIntersection(Ray &r, float step, glm::vec3 &intersectionPoint);

	bool axisAlignedPlaneIntersect(Ray r, float step, glm::vec3 &intersectionPoint, glm::vec3 normalAxis);
	glm::vec3 getNormalForSide(int side);

	glm::vec3 dimensions;
	Plane3D front;
	Plane3D back;
	Plane3D right;
	Plane3D left;
	Plane3D top;
	Plane3D bottom;
};