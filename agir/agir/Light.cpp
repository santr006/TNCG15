#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 col, float i)
{
	position = pos;
	color = col;
	intensity = i;
	areaLight = false;
	dimensions = glm::vec2(0.f);
	rotation = glm::vec3(0.f);
}

Light::Light(glm::vec3 pos, glm::vec3 col, float i, bool areaL, glm::vec2 dim, glm::vec3 rot)
{
	position = pos;
	color = col;
	intensity = i;
	areaLight = areaL;
	dimensions = dim;
	rotation = rot;
}

Light::~Light()
{
}

glm::vec3 Light::randomPointOnAreaLight()
{
	//translate the plane to it's local coordinate system rotate it and tranlate it beck
	glm::mat4 translation = glm::translate(glm::mat4(1.f), -position);
	glm::mat4 translationBack = glm::translate(glm::mat4(1.f), position);
	glm::mat4 rotat = glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.f), -rotation.x, glm::vec3(1, 0, 0)), -rotation.y, glm::vec3(0, 1, 0)), -rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 toLocal = rotat * translation;

	float random1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float random2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	glm::vec4 randomPoint(dimensions.x * (0.5f - random1), dimensions.y * (0.5f - random2), 0.f, 1.f);

	randomPoint = translationBack * rotat * randomPoint;
	return glm::vec3(randomPoint.x, randomPoint.y, randomPoint.z);
}