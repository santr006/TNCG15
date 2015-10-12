/*************************************
* Axis aligned bounding box (AABB)	 *
**************************************/

#pragma once
#include <glm.hpp>
#include "Ray.h"
#include "Object3D.h"

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	//checks if the given ray intersects this box 
	//and saves the position in world coordinates
	//in intersectionPoint if it does
	bool intersects(Ray r, float step, glm::vec3 &intersectionPoint);
	//overlap(BBox b)
	//union(BBox b)
	//inside(vec3 point)

	glm::vec3 position; //the corner with the largest x-value, y-value and z-value
	glm::vec3 dimension; //height, width, depth
	std::vector<Object3D*> objects; //vector storing pointers to the objects inside this bounding box

};

