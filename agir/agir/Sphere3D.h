#pragma once

#include "Object3D.h"
class Sphere3D : public Object3D
{

public:
	Sphere3D(glm::mat4 w2l, glm::vec4 c, float r)
	{
		worldToLocal = w2l, color = c, radius = r;
		center = glm::vec3(w2l[3]);
	};
	~Sphere3D(){};

	Intersection* rayIntersection(Ray &r);

	float radius;

private:
};