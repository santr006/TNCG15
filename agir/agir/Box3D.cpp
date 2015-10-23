#include "Box3D.h"

Box3D::Box3D(glm::vec3 pos, glm::vec3 rot, glm::vec3 dim, glm::vec3 col)
{
	position = pos;
	rotation = rot;
	dimensions = dim;
	color = col;

	//calculate the positions of the planes after rotation
	glm::mat4 translation = glm::translate(glm::mat4(1.f), -pos);
	glm::mat4 translationBack = glm::translate(glm::mat4(1.f), pos);
	//we have chosen not to rotate around z
	glm::mat4 rotat =  glm::rotate(glm::rotate(glm::mat4(1.f), -rot.x, glm::vec3(1, 0, 0)), -rot.y, glm::vec3(0, 1, 0));

	glm::vec4 planePos(pos.x, pos.y, pos.z + dim.z / 2, 1);
	planePos = translationBack * rotat * translation * planePos;
	front = Plane3D(glm::vec3(planePos.x, planePos.y, planePos.z), rot, glm::vec2(dim.x, dim.y), col);
	
	planePos = glm::vec4(pos.x, pos.y, pos.z - dim.z / 2, 1);
	planePos = translationBack * rotat * translation * planePos;
	back = Plane3D(glm::vec3(planePos.x, planePos.y, planePos.z), rot, glm::vec2(dim.x, dim.y), glm::vec3(0.5f, 0.5f, 0.5f));

	planePos = glm::vec4(pos.x + dim.x / 2, pos.y, pos.z, 1);
	planePos = translationBack * rotat * translation * planePos;
	right = Plane3D(glm::vec3(planePos.x, planePos.y, planePos.z), glm::vec3(rot.x, rot.y + glm::half_pi<float>(), 0), glm::vec2(dim.z, dim.y), glm::vec3(1, 1, 1));

	planePos = glm::vec4(pos.x - dim.x / 2, pos.y, pos.z, 1);
	planePos = translationBack * rotat * translation * planePos;
	left = Plane3D(glm::vec3(planePos.x, planePos.y, planePos.z), glm::vec3(rot.x, rot.y - glm::half_pi<float>(), 0), glm::vec2(dim.z, dim.y), glm::vec3(0, 1, 0));

	planePos = glm::vec4(pos.x, pos.y + dim.y / 2, pos.z, 1);
	planePos = translationBack * rotat * translation * planePos;
	//to make the top and bottom move like we want them to we must rotate around z when the box rotates around y
	top = Plane3D(glm::vec3(planePos.x, planePos.y, planePos.z), glm::vec3(rot.x - glm::half_pi<float>(), 0, -rot.y), glm::vec2(dim.x, dim.z), glm::vec3(0, 0, 1));
	
	planePos = glm::vec4(pos.x, pos.y - dim.y / 2, pos.z, 1);
	planePos = translationBack * rotat * translation * planePos;
	//to make the top and bottom move like we want them to we must rotate around z when the box rotates around y
	bottom = Plane3D(glm::vec3(planePos.x, planePos.y, planePos.z), glm::vec3(rot.x - glm::half_pi<float>(), 0, -rot.y), glm::vec2(dim.x, dim.z), glm::vec3(1, 1, 0));
	
}

Box3D::~Box3D()
{
	// bye bye boxie
}

Intersection* Box3D::testRayIntersection(Ray &r)
{
	//Does the ray intersect any of the planes that this box consists of?
	Intersection* closest = front.testRayIntersection(r);

	Intersection* current = back.testRayIntersection(r);
	if (current != nullptr)
		closest = current;

	current = right.testRayIntersection(r);
	if (current != nullptr)
		closest = current;

	current = left.testRayIntersection(r);
	if (current != nullptr)
		closest = current;

	current = top.testRayIntersection(r);
	if (current != nullptr)
		closest = current;

	current = bottom.testRayIntersection(r);
	if (current != nullptr)
		closest = current; 
	
	return closest;
}
