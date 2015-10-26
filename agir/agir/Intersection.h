#include "glm.hpp"

struct Intersection{

	// A struct may be initialized with a initializer list or with this constructor
	Intersection(glm::vec3 p, glm::vec3 sf, glm::vec3 c, float reflecCoef, float dist) : point(p), surfaceNormal(sf), color(c), reflectionCoef(reflecCoef), distance(dist){}

	// data members
	glm::vec3 point; //the point where the ray intersects the surface
	glm::vec3 surfaceNormal; //the surface's normal, if the surface is a sphere it's the point's normal on the surface
	glm::vec3 color; //the surface's color
	float reflectionCoef; //how much of incoming radiance is reflected from the surface
	float distance; //distance traveled along the ray
};