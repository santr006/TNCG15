#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

bool BoundingBox::intersects(Ray r, float step, glm::vec3 &intersectionPoint)
{
	//where does the ray intersect the first plane orthogonal to the z-axis?
	glm::vec3 zIntersect = r.startPosition;
	while (zIntersect.z > position.z)
		zIntersect += r.direction * step;

	//where is this on the y-axis? Is it in range of the bounding box's surface?
	if (zIntersect.y < r.startPosition.y && zIntersect.y > r.startPosition.y - r.direction.y)
	{
		//where is this on the x-axis? Is it in range of the bounding box's surface?
		if (zIntersect.x < r.startPosition.x && zIntersect.x > r.startPosition.x - r.direction.x)
		{
			//save the position in world coordinates where the ray intersects the box
			intersectionPoint = zIntersect;
			return true;
		}
	}
	
	return false;
}