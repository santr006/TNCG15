#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 col)
{
	position = pos;
	intensity = col;
}

Light::~Light()
{
}