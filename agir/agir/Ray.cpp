#include "Ray.h"

Ray::Ray(glm::vec3 pos, glm::vec3 dir, bool shadow, float _tMin, float _tMax)
{
	startPosition = pos;
	direction = glm::normalize(dir);
	shadowRay = shadow;
	tMin = _tMin;
	tMax = _tMax;
}

Ray::~Ray()
{

}