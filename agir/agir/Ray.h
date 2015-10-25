#pragma once

#include <glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 pos, glm::vec3 dir, bool shadow = false, float _tMin = 0.f, float _tMax = 10000.f);
	~Ray();

	glm::vec3 startPosition;
	glm::vec3 direction;
	bool shadowRay;
	float tMin, tMax;
};