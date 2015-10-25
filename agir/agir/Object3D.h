#pragma once

#include <iostream>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Ray.h"
#include "Intersection.h"

const float PI = 3.14159265359f;

class Object3D
{
public:
	
	Object3D();
	virtual ~Object3D();

	virtual Intersection* testRayIntersection(Ray& r) = 0; // pure virtual function

	glm::vec3 position;
	glm::vec3 rotation; //rotation round x-axis and rotation round y-axis
	glm::vec3 color;  // to be replaced with material?
				// TODO: BRDF with material properties
	float reflectionCoef; //A constant reflection coefficient [0,1] for Lambertian surfaces

};

