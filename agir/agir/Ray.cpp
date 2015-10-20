#include "Ray.h"

Ray::Ray(glm::vec3 pos, glm::vec3 dir, float min, float max)
{
	startPosition = pos;
	direction = glm::normalize(dir);
	near = min;
	far = max;
}

Ray::~Ray()
{
}

glm::vec3 Ray::step(const float &dt)
{
	return startPosition + direction*dt;
}
