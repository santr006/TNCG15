#include "glm.hpp"

//loop include :( !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#include "Object3D.h"

struct Intersection{

	// A struct may be initialized with a initializer list or with this constructor
	Intersection(glm::vec3 p, glm::vec3 sfn, glm::vec3 c, float reflecCoef, float dist, bool l = false, bool t = false) : point(p), surfaceNormal(sfn), color(c), reflectionCoef(reflecCoef), distance(dist), light(l), transparent(t){}

	// data members
	glm::vec3 point; //the point where the ray intersects the surface
	glm::vec3 surfaceNormal; //the surface's normal, if the surface is a sphere it's the point's normal on the surface
	glm::vec3 color; //the surface's color
	float reflectionCoef; //how much of incoming radiance is reflected from the surface
	float distance; //distance traveled along the ray
	bool light; //saves if the object hit was a light source or not
	bool transparent; //saves if the object is transparent or not
	//Object3D* object; //save a pointer to the intersected object which is used if the object i transparent
};