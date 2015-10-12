#pragma once

#include <glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 pos, glm::vec3 dir);
	~Ray();

	glm::vec3 startPosition;
	glm::vec3 direction;
};

