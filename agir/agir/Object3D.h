#pragma once

#include <glm.hpp>
#include "Rgb.h"
#include "Ray.h"

class Object3D
{
public:
	
	Object3D();
	virtual ~Object3D() = 0;	// pure virtual destructor

	virtual bool testRayIntersection(Ray& r) = 0; // pure virtual function

	glm::vec3 getPosition(){ return position; }
	glm::vec3 getRotation(){ return rotation; }
	Rgb* getColor(){ return &color; }

protected:

	glm::vec3 position;
	glm::vec3 rotation;

	Rgb color;  // to be replaced with material?
				// TODO: BRDF with material properties
};

