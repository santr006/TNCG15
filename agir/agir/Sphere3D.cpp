#include "Sphere3D.h"

Sphere3D::Sphere3D(glm::vec3 &pos, glm::vec2 &rot, float r, glm::vec3 col)
{
	position = pos;
	rotation = rot;
	radius = r;
	color = col;
}

Sphere3D::~Sphere3D()
{
	// so long sphere!
}

bool Sphere3D::testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint)
{
	//Switch to this object's coordinate system

	//move the object so that this coordinate system's origin is at the world coordinate's origin
	glm::mat4 translation(1, 0, 0, -position.x,
		0, 1, 0, -position.y,
		0, 0, 1, -position.z,
		0, 0, 0, 1);

	//rotate the object so it's coordinate system's orientation is the same as the world coordinate's origin
	//Not needed for sphere
	/*glm::mat4 rotation(glm::cos(-rotation.x), -glm::sin(-rotation.x)*glm::sin(-rotation.y), -glm::sin(-rotation.x)*glm::cos(-rotation.y), 0,
		0, glm::cos(-rotation.y), -glm::sin(-rotation.y), 0,
		glm::sin(-rotation.x), glm::cos(-rotation.x)*glm::sin(-rotation.y), glm::cos(-rotation.x)*glm::cos(-rotation.y), 0,
		0, 0, 0, 1);*/

	glm::mat4 worldToObject = translation;
	glm::vec4 newStartPos = worldToObject * glm::vec4(r.startPosition, 1);
	glm::vec4 newDir = worldToObject * glm::vec4(r.direction, 1);

	//
	
	//where does the ray intersect the plane orthogonal to the z-axis which contains the center of the sphere (origin)?
	glm::vec4 zIntersect = newStartPos;
	while (zIntersect.z > 0)
		zIntersect += newDir * step;

	//where is this on the y-axis? Is it inside the radius of the sphere?
	if (glm::abs(zIntersect.y) < radius)
	{
		//where is this on the x-axis? Is it inside the radius of the sphere?
		if (glm::abs(zIntersect.x) < radius)
		{
			//where does the ray hit the sphere?

			//step back along the ray until we are at the surface
			intersectionPoint = glm::vec3(zIntersect.x, zIntersect.y, zIntersect.z);
			float stepInSphere = radius * 0.1f;
			while (glm::pow(intersectionPoint.x, 2) + glm::pow(intersectionPoint.y, 2) + glm::pow(intersectionPoint.z, 2) < glm::pow(radius, 2))
			{
				intersectionPoint -= glm::vec3(newDir.x, newDir.y, newDir.z) * stepInSphere;
			}

			//the position in world coordinates where the ray intersects the box are found and saved
			return true;
		}
	}

	return false;
}