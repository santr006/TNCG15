#include "Ray.h"


Ray::Ray(glm::vec3 pos, glm::vec3 dir)
{
	startPosition = pos;
	direction = glm::normalize(dir);
}


Ray::~Ray()
{
}
