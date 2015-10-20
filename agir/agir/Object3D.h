#pragma once

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Rgb.h"
#include "Ray.h"
#include "Intersection.h"

class Object3D
{
public:

	Object3D(){};
	virtual ~Object3D(){};

	//virtual bool testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint) = 0; // pure virtual function
	virtual Intersection* rayIntersection(Ray &r) = 0;

	glm::mat4 worldToLocal;
	glm::vec3 center;
	glm::vec4 color;

protected:
};