#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 col, float i)
{
	position = pos;
	color = col;
	intensity = i;
}

Light::~Light()
{
}