#pragma once
#include "Object3D.h"
class Box3D : public Object3D
{
public:
	Box3D(glm::vec3 pos, glm::vec3 rot, glm::vec3 dim, Rgb* col);
	~Box3D();

	bool testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint);

private:
	glm::vec3 dimensions;
};