#pragma once
#include "Object3D.h"

class Box3D : public Object3D
{
public:
	Box3D(glm::vec3 pos, glm::vec2 rot, glm::vec3 dim, glm::vec3 col);
	~Box3D();

	bool testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint);

private:
	bool axisAlignedPlaneIntersect(Ray r, float step, glm::vec3 &intersectionPoint, glm::vec3 normalAxis);
	glm::vec3 getNormalForSide(int side);

	glm::vec3 minCorner;
};