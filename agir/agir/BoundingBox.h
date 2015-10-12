/*************************************
* Axis aligned bounding box (AABB)	 *
**************************************/

#pragma once
#include <glm.hpp>
#include "Ray.h"

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	bool intersects(Ray r);
	//overlap(BBox b)
	//union(BBox b)
	//inside(vec3 point)

	glm::vec3 position;
	glm::vec3 dimension; //height, width, depth
};

