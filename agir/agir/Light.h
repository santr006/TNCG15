#pragma once

#include "glm.hpp"
#include <gtc\matrix_transform.hpp>

#include "Object3D.h"

class Light : public Object3D
{
public:
	Light(glm::vec3 pos, glm::vec3 col, float i);
	Light(glm::vec3 pos, glm::vec3 col, float i, bool areaL, glm::vec2 dim, glm::vec3 rot);
	~Light();

	Intersection* testRayIntersection(Ray r);

	glm::vec3 randomPointOnAreaLight();

	float intensity;
	bool areaLight;
	glm::vec2 dimensions;
	//all area light sources are planes
};