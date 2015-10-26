#pragma once

#include "glm.hpp"
#include <gtc\matrix_transform.hpp>

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 col, float i);
	Light(glm::vec3 pos, glm::vec3 col, float i, bool areaL, glm::vec2 dim, glm::vec3 rot);
	~Light();

	glm::vec3 randomPointOnAreaLight();

	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	bool areaLight;
	glm::vec2 dimensions;
	glm::vec3 rotation;
	//all area light sources are planes
};