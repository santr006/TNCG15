#include "glm.hpp"

struct Intersection{

	// A struct may be initialized with a initializer list or with this constructor
	Intersection(glm::vec3 p, glm::vec3 sf, glm::vec3 c) : point(p), surfaceNormal(sf), color(c){}

	// data members
	glm::vec3 point, surfaceNormal;
	glm::vec3 color;
};