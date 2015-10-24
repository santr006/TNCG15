#include "Ray.h"

Ray::Ray(glm::vec3 pos, glm::vec3 dir, float _tMin)
{
	startPosition = pos;
	direction = glm::normalize(dir);
	tMin = _tMin;
}

Ray::~Ray()
{

}