#pragma once

#include <glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 pos, glm::vec3 dir);
	~Ray();

	//checks if the given ray intersects any object in the scene
	bool rayIntersect();

	glm::vec3 startPosition;
	glm::vec3 direction;
};

