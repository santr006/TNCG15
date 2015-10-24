#pragma once

#include <iostream>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Ray.h"
#include "Intersection.h"

const float PI = 3.14159265359f;
const float EPSILON = 0.00001;


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
	
	
	//float ka = 0.f, kd = 0.9f, ks = 0.3f, roughness = 2;
};