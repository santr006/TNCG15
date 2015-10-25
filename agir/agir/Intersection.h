#include "glm.hpp"

struct Intersection{

	// A struct may be initialized with a initializer list or with this constructor
	Intersection(glm::vec3 p, glm::vec3 sf, glm::vec3 c, float reflecCoef, float dist) : point(p), surfaceNormal(sf), color(c), reflectionCoef(reflecCoef), distance(dist){}

	// data members
	glm::vec3 point;
	glm::vec3 surfaceNormal;
	glm::vec3 color;
	float reflectionCoef;
	float distance;
};