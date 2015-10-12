#pragma once

#include "Object3D.h"
class Sphere3D : public Object3D
{

public:
	Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, Rgb &col);
	~Sphere3D();

	bool testRayIntersection(Ray& r);

private:
	float radius;
};