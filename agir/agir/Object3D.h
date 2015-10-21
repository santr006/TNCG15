#pragma once

#include <iostream>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Ray.h"
#include "Intersection.h"

class Object3D
{
public:
	
	Object3D();
	virtual ~Object3D();

	virtual Intersection* testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint) = 0; // pure virtual function

	glm::vec3 position;
	glm::vec2 rotation; //rotation round x-axis and rotation round y-axis

	glm::vec3 color;  // to be replaced with material?
				// TODO: BRDF with material properties

};

