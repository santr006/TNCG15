#include "Box3D.h"

Box3D::Box3D(glm::vec3 pos, glm::vec2 rot, glm::vec3 dim, glm::vec3 col)
{
	position = pos;
	rotation = rot;
	dimensions = dim;
	color = col;
}

Box3D::~Box3D()
{
	// bye bye boxie
}

bool Box3D::testRayIntersection(Ray r, float step, glm::vec3 &intersectionPoint)
{
	//Switch to this object's coordinate system

	//move the object so that this coordinate system's origin is at the world coordinate's origin
	glm::mat4 translation = glm::translate(glm::mat4(1.f), -position);

	//rotate the object so it's coordinate system's orientation is the same as the world coordinate's origin
	glm::mat4 rotat = glm::rotate(glm::rotate(glm::mat4(1.f), -rotation.y, glm::vec3(0, 1, 0)), -rotation.x, glm::vec3(1, 0, 0));

	glm::mat4 worldToObject = rotat * translation;
	glm::vec4 objectPos = worldToObject * glm::vec4(position, 1);
	glm::vec4 newStartPos = worldToObject * glm::vec4(r.startPosition, 1);
	//direction only gets rotated
	glm::vec4 newDir = rotat * glm::vec4(r.direction, 1);

	/*std::cout << " translation: " << translation[0][3] << " " << translation[1][3] << " " << translation[2][3] << std::endl;
	std::cout << " old object position: " << position.x << " " << position.y << " " << position.z << std::endl;
	std::cout << " new object position: " << objectPos.x << " " << objectPos.y << " " << objectPos.z << std::endl;
	std::cout << " old ray position: " << r.startPosition.x << " " << r.startPosition.y << " " << r.startPosition.z << std::endl;
	std::cout << " new ray position: " << newStartPos.x << " " << newStartPos.y << " " << newStartPos.z << std::endl;
	std::cout << " old dir: " << r.direction.x << " " << r.direction.y << " " << r.direction.z << std::endl;
	std::cout << " new dir: " << newDir.x << " " << newDir.y << " " << newDir.z << std::endl;*/

	//check which surfaces has the normals in the right direction for the ray to be able to hit them
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 normal = getNormalForSide(i);

		//if the dot product between the ray and the normal is smaller than 0, the surface is facing the ray
		if (glm::dot(glm::normalize(glm::vec3(newDir.x, newDir.y, newDir.z)), normal) < 0)
		{
			//std::cout << "ray can hit side " << i << std::endl;
			//does the ray hit this surface?
			Ray newRay(glm::vec3(newStartPos.x, newStartPos.y, newStartPos.z), glm::vec3(newDir.x, newDir.y, newDir.z));

			//we know that the box is axis aligned
			//checking the axis parallell to the normal
			if (axisAlignedPlaneIntersect(newRay, step, intersectionPoint, normal))
			{
				std::cout << "hit side " << i << std::endl; 
				//tillfällig
				intersectionPoint.x = i;
				return true;
			}
		}
	}
	//handle multiple hits later

	return false;
}

glm::vec3 Box3D::getNormalForSide(int side)
{
	glm::vec3 xVec;
	glm::vec3 yVec;
	glm::vec3 zVec;

	switch (side)
	{
	case 0: //front, z-normal
		xVec = glm::vec3(-dimensions.x, 0, 0);
		yVec = glm::vec3(0, -dimensions.y, 0);
		return glm::normalize(glm::cross(xVec, yVec));
	case 1://back, z-normal
		xVec = glm::vec3(-dimensions.x, 0, 0);
		yVec = glm::vec3(0, -dimensions.y, 0);
		return glm::normalize(glm::cross(yVec, xVec));
	case 2: //right, x-normal
		yVec = glm::vec3(0, -dimensions.y, 0);
		zVec = glm::vec3(0, 0, -dimensions.z);
		return glm::normalize(glm::cross(yVec, zVec));
	case 3: //left, x-normal
		yVec = glm::vec3(0, -dimensions.y, 0);
		zVec = glm::vec3(0, 0, -dimensions.z);
		return glm::normalize(glm::cross(zVec, yVec));
	case 4: //top, y-normal
		xVec = glm::vec3(-dimensions.x, 0, 0);
		zVec = glm::vec3(0, 0, -dimensions.z);
		return glm::normalize(glm::cross(zVec, xVec));
	case 5: //bottom, y-normal
		xVec = glm::vec3(-dimensions.x, 0, 0);
		zVec = glm::vec3(0, 0, -dimensions.z);
		return glm::normalize(glm::cross(xVec, zVec));
	}
}

//Checks if the given ray r hits the box on the side which is on the plane with a normal equal to normalAxis
//and if the ray does the point where the ray hit the plane is saved in intersectionPoint
bool Box3D::axisAlignedPlaneIntersect(Ray r, float step, glm::vec3 &intersectionPoint, glm::vec3 normalAxis)
{
	if (normalAxis.x != 0)
	{
		//where does the ray intersect the first plane orthogonal to the x-axis?
		glm::vec3 xIntersect = r.startPosition;;

		if (normalAxis.x > 0)
			while (xIntersect.x > 0)
				xIntersect += r.direction * step;
		else
			while (xIntersect.x < -dimensions.x)
				xIntersect += r.direction * step;

		//where is this on the y-axis? Is it in range of the bounding box's surface?
		if (xIntersect.y <= 0 && xIntersect.y >= -dimensions.y)
		{
			//where is this on the x-axis? Is it in range of the box's surface?
			if (xIntersect.z <= 0 && xIntersect.z >= -dimensions.z)
			{
				//save the position in world coordinates where the ray intersects the box
				//std::cout << "intersects" << std::endl;
				intersectionPoint = glm::vec3(xIntersect.x, xIntersect.y, xIntersect.z);
				return true;
			}
		}
	}
	else if (normalAxis.y != 0)
	{
		//where does the ray intersect the first plane orthogonal to the z-axis?
		glm::vec3 yIntersect = r.startPosition;

		if (normalAxis.y > 0)
			while (yIntersect.y > 0)
				yIntersect += r.direction * step;
		else
			while (yIntersect.y < -dimensions.y)
				yIntersect += r.direction * step;

		//where is this on the y-axis? Is it in range of the bounding box's surface?
		if (yIntersect.z <= 0 && yIntersect.z >= -dimensions.z)
		{
			//where is this on the x-axis? Is it in range of the box's surface?
			if (yIntersect.x <= 0 && yIntersect.x >= -dimensions.x)
			{
				//save the position in world coordinates where the ray intersects the box
				//std::cout << "intersects" << std::endl;
				intersectionPoint = glm::vec3(yIntersect.x, yIntersect.y, yIntersect.z);
				return true;
			}
		}
	}
	else
	{
		//where does the ray intersect the first plane orthogonal to the z-axis?
		glm::vec3 zIntersect = r.startPosition;

		if (normalAxis.z > 0)
			while (zIntersect.z > 0)
				zIntersect += r.direction * step;
		else
			while (zIntersect.z < -dimensions.z)
				zIntersect += r.direction * step;

		//where is this on the y-axis? Is it in range of the bounding box's surface?
		if (zIntersect.y <= 0 && zIntersect.y >= -dimensions.y)
		{
			//where is this on the x-axis? Is it in range of the box's surface?
			if (zIntersect.x <= 0 && zIntersect.x >= -dimensions.x)
			{
				//save the position in world coordinates where the ray intersects the box
				//std::cout << "intersects" << std::endl;
				intersectionPoint = glm::vec3(zIntersect.x, zIntersect.y, zIntersect.z);
				return true;
			}
		}
	}

	return false;
}