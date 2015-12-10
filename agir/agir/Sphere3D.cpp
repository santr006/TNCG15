#include "Sphere3D.h"

Sphere3D::Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, glm::vec3 col)
{
	position = pos;
	rotation = rot;
	radius = r;
	color = col;
	reflectionCoef = 1.f;
	transparent = false;
}

Sphere3D::Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, glm::vec3 col, float reflecCoef)
{
	position = pos;
	rotation = rot;
	radius = r;
	color = col;
	reflectionCoef = reflecCoef;
	transparent = false;
}

Sphere3D::Sphere3D(glm::vec3 &pos, glm::vec3 &rot, float r, bool trans)
{
	position = pos;
	rotation = rot;
	radius = r;
	color = glm::vec3(0.f);
	reflectionCoef = 1.f;
	transparent = trans;
}

Sphere3D::~Sphere3D()
{
	// so long sphere!
}

Intersection* Sphere3D::testRayIntersection(Ray r)
{
	//L is the vector from the start position of the ray to the center of the sphere
	glm::vec3 L = position - r.startPosition;
	//tca is L projected onto the ray direction vector, a scalar representing L's lenght along ray direction
	float tca = glm::dot(L, r.direction);
	//if tca is < 0 the sphere is behind the start position and won't be hit
	if (tca < 0)
		return nullptr;

	//d is the vector ortogonal from the ray direction that goes from the center of the sphere to the ray direction * tca
	//together L, d and tca are an orthogonal triangle
	//d2 is (the length of d)^2
	//pythagoras gives d2 + tca2 = L2
	//if d2 > (the sphere radius)^2, the ray doesn't intersect the sphere
	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > radius * radius)
		return nullptr;

	//thc is the vector from the intersection point P in the ray direction untill it hits d
	//the intersection point will be at tca - thc
	float thc = sqrt(radius * radius - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 > t1){
		t0 = t1;
		if (t0 < 0)
			return nullptr;
	}

	glm::vec3 intersectionPoint(r.startPosition + t0 * r.direction);
	if (transparent)
		return new Intersection(intersectionPoint, glm::normalize(intersectionPoint - position), color, reflectionCoef, t0, false, true);
	return new Intersection(intersectionPoint, glm::normalize(intersectionPoint - position), color, reflectionCoef, t0);
}

Intersection* Sphere3D::testRayIntersectionInside(Ray r){
	//at which point on the ray that isn't the start point does the ray intersect the sphere?
	//when P = O + t*D and v = P - pos has length radius

	//L is the vector from the start position of the ray to the center of the sphere
	glm::vec3 L = position - r.startPosition;
	//tca is L projected onto the ray direction vector, a scalar representing L's lenght along ray direction
	float tca = glm::dot(L, r.direction);
	//if tca is < 0 omething went wrong
	if (tca < 0)
		return nullptr;

	//the intersection point is at 2 tca
	float t = 2*tca;

	if (t < 0)
		return nullptr;

	glm::vec3 intersectionPoint(r.startPosition + t * r.direction);
	return new Intersection(intersectionPoint, glm::normalize(intersectionPoint - position), color, reflectionCoef, t);
}