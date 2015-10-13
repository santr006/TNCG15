#pragma once

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Rgb.h"
#include "Ray.h"

class Object3D
{
public:
	
	Object3D();
	virtual ~Object3D();

	virtual bool testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint) = 0; // pure virtual function

	glm::vec3 getPosition(){ return position; }
	glm::vec2 getRotation(){ return rotation; }
	glm::vec3 getColor(){ return color; }

protected:

	glm::vec3 position;
	glm::vec2 rotation; //rotation round x-axis and rotation round y-axis

	glm::vec3 color;  // to be replaced with material?
				// TODO: BRDF with material properties

};

