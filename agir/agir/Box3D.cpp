#include "Box3D.h"

Box3D::Box3D(glm::vec3 pos, glm::vec3 rot, glm::vec3 dim, Rgb* col)
{

}

Box3D::~Box3D()
{
	// bye bye boxie
}

bool Box3D::testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint)
{
	//Switch to this object's coordinate system

	//move the object so that this coordinate system's origin is at the world coordinate's origin
	glm::mat4 translation(1, 0, 0, -position.x,
						  0, 1, 0, -position.y,
						  0, 0, 1, -position.z,
						  0, 0, 0, 1);

	//rotate the object so it's coordinate system's orientation is the same as the world coordinate's origin
	glm::mat4 rotation( glm::cos(-rotation.x), -glm::sin(-rotation.x)*glm::sin(-rotation.y), -glm::sin(-rotation.x)*glm::cos(-rotation.y), 0,
						0, glm::cos(-rotation.y), -glm::sin(-rotation.y), 0,
						glm::sin(-rotation.x), glm::cos(-rotation.x)*glm::sin(-rotation.y), glm::cos(-rotation.x)*glm::cos(-rotation.y), 0,
						0, 0, 0, 1);

	glm::mat4 worldToObject = rotation * translation;
	glm::vec4 newStartPos = worldToObject * glm::vec4(r.startPosition, 1);
	glm::vec4 newDir = worldToObject * glm::vec4(r.direction, 1);
	
	//where does the ray intersect the first plane orthogonal to the z-axis?
	glm::vec4 zIntersect = newStartPos;
	while (zIntersect.z > 0)
		zIntersect += newDir * step;

	//where is this on the y-axis? Is it in range of the bounding box's surface?
	if (zIntersect.y < newStartPos.y && zIntersect.y > newStartPos.y - newDir.y)
	{
		//where is this on the x-axis? Is it in range of the bounding box's surface?
		if (zIntersect.x < newStartPos.x && zIntersect.x > newStartPos.x - newDir.x)
		{
			//save the position in world coordinates where the ray intersects the box
			intersectionPoint = glm::vec3(zIntersect.x, zIntersect.y, zIntersect.z);
			return true;
		}
	}

	return false;
}