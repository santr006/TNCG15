#pragma once

#include "glm.hpp"

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 col);
	~Light();

	glm::vec3 position;
	glm::vec3 intensity;
	float material; /* Lambertian reflection material.
					To be replaced with BRDF later */
};