#pragma once

#include "glm.hpp"

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 col, float i);
	~Light();

	glm::vec3 getPosition(){ return position; }
	glm::vec3 getColor(){ return color; }
	float getIntensity() { return intensity; }

private:
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};