#pragma once

#include <glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 pos, glm::vec3 dir, float min=1, float max=500);
	~Ray();

	glm::vec3 step(const float &dt);
	float near, far;

	glm::vec3 startPosition;
	glm::vec3 direction;
};

