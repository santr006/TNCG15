#pragma once

#include "Object3D.h"
class Sphere3D : public Object3D
{

public:
	Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, glm::vec3 col);
	Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, glm::vec3 col, float reflecCoef);
	
	~Sphere3D();

	Intersection* testRayIntersection(Ray& r);

	float radius;
};