#include "Plane3D.h"

Plane3D::Plane3D()
{

}

Plane3D::Plane3D(glm::vec3 pos, glm::vec3 rot, glm::vec2 dim, glm::vec3 col)
{
	position = pos;
	rotation = rot;
	dimensions = dim;
	color = col;
	reflectionCoef = 1.f;
}

Plane3D::Plane3D(glm::vec3 pos, glm::vec3 rot, glm::vec2 dim, glm::vec3 col, float reflecCoef)
{
	position = pos;
	rotation = rot;
	dimensions = dim;
	color = col;
	reflectionCoef = reflecCoef;
}


Plane3D::~Plane3D()
{

}


Intersection* Plane3D::testRayIntersection(Ray &r)
{
	/*std::cout << "ray start pos " << r.startPosition.x << " " << r.startPosition.y << " " << r.startPosition.z << std::endl;
	std::cout << "ray direction " << r.direction.x << " " << r.direction.y << " " << r.direction.z << std::endl;*/
	//The ray is described by Point on ray = start(O) + direction(D) * t
	//If a point B on the ray hits a surface it must be inside the plane of that surface
	//We can check if B is inside the surface by making a vector between a point on the surface(A) and B
	//and dot multiply it with the plan's normal and see if it becomes zero
	//(A-B)*n = 0
	//Then the point B is on the plane

	//We want to know at what t-value we get a point B on the ray that is inside the plane
	//After some calculations this formula was found
	//t = (A - O) dot n / D dot n

	//translate and rotate to local coordinate system for the plane
	glm::mat4 translation = glm::translate(glm::mat4(1.f), -position);
	glm::mat4 translationBack = glm::translate(glm::mat4(1.f), position);
	glm::mat4 rotat = glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.f), -rotation.x, glm::vec3(1, 0, 0)), -rotation.y, glm::vec3(0, 1, 0)), -rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 toLocal = rotat * translation;

	glm::vec4 temp(translationBack * toLocal * glm::vec4(position + glm::vec3(-dimensions.x / 2, -dimensions.y / 2, 0), 1));
	glm::vec3 lowerLeftCorner(temp.x, temp.y, temp.z);
	temp = glm::vec4(translationBack * toLocal * glm::vec4(position + glm::vec3(dimensions.x / 2, -dimensions.y / 2, 0), 1));
	glm::vec3 lowerRightCorner(temp.x, temp.y, temp.z);
	temp = glm::vec4(translationBack * toLocal * glm::vec4(position + glm::vec3(-dimensions.x / 2, dimensions.y / 2, 0), 1));
	glm::vec3 upperLeftCorner(temp.x, temp.y, temp.z);

	/*temp = glm::vec4(toLocal * glm::vec4(r.startPosition, 1));
	r.startPosition = glm::vec3(temp.x, temp.y, temp.z);
	temp = glm::vec4(rotat * glm::vec4(r.direction, 1));
	r.direction = glm::vec3(temp.x, temp.y, temp.z);*/

	//Find plane normal
	glm::vec3 planeNormal(glm::cross(lowerRightCorner - lowerLeftCorner, upperLeftCorner - lowerLeftCorner));

	/*std::cout << "lower left " << lowerLeftCorner.x << " " << lowerLeftCorner.y << " " << lowerLeftCorner.z << std::endl;
	std::cout << "lower right " << lowerRightCorner.x << " " << lowerRightCorner.y << " " << lowerRightCorner.z << std::endl;
	std::cout << "upper left " << upperLeftCorner.x << " " << upperLeftCorner.y << " " << upperLeftCorner.z << std::endl;
	std::cout << "ray start pos " << r.startPosition.x << " " << r.startPosition.y << " " << r.startPosition.z << std::endl;
	std::cout << "ray direction " << r.direction.x << " " << r.direction.y << " " << r.direction.z << std::endl;*/

	//If D not n == 0 the direction goes along the plane and the ray won't hit the plane
	float DdotN = glm::dot(r.direction, planeNormal);
	if (DdotN == 0){
		//std::cout << "DdotN = 0 " << std::endl;
		return nullptr;
	}

	//Find point inside plane -> position, which is the center of the plane
	//Find intersection
	float t = glm::dot(position - r.startPosition, planeNormal) / DdotN;

	//If t < 0 the plane is behind the ray origin and we aren't interested in it
	if (t < 0){
		//std::cout << "t < 0 " << std::endl;
		return nullptr;
	}

	//Is B inside the surface bounds?
	//Create vectors that decribe the bounds
	//"Inside" is on the right side of the vector
	temp = glm::vec4(translationBack * toLocal * glm::vec4(position + glm::vec3(dimensions.x / 2, dimensions.y / 2, 0), 1));
	glm::vec3 upperRightCorner(temp.x, temp.y, temp.z);
	//std::cout << "upper right " << upperRightCorner.x << " " << upperRightCorner.y << " " << upperRightCorner.z << std::endl;

	glm::vec3 leftSide(upperLeftCorner - lowerLeftCorner);
	glm::vec3 topSide(upperRightCorner - upperLeftCorner);
	glm::vec3 rightSide(lowerRightCorner - upperRightCorner);
	glm::vec3 bottomSide(lowerLeftCorner - lowerRightCorner);

	//To know if B is on the correct side of the vector we take the cross product of
	//the bounding vector and a vector from the same starting point as the bounding vector and ends in B
	//If the result is a vector along the plane's normal, the point B is on the correct side of the bounding vector
	//If the result is a vector against the plane's normal the point is on the wrong side
	//To know if the vector is going along the normal we take the dot product between them
	//If it is larger than zero they with eachother

	//leftSide
	glm::vec3 B(r.startPosition + t * r.direction);
	glm::vec3 Bvector(B - lowerLeftCorner);
	if (glm::dot(glm::cross(Bvector, leftSide), planeNormal) > 0)
	{
		//topSide
		Bvector = glm::vec3(B - upperLeftCorner);
		if (glm::dot(glm::cross(Bvector, topSide), planeNormal) > 0)
		{
			//rightSide
			Bvector = glm::vec3(B - upperRightCorner);
			if (glm::dot(glm::cross(Bvector, rightSide), planeNormal) > 0)
			{
				//bottomSide
				Bvector = glm::vec3(B - lowerRightCorner);
				if (glm::dot(glm::cross(Bvector, bottomSide), planeNormal) > 0)
				{
					// If the object is blocked
					if (t > r.tMax)
						return nullptr;

					//else
					r.tMax = t;
					glm::vec3 intersectionPoint(r.startPosition + t * r.direction);
					return new Intersection(intersectionPoint, glm::normalize(intersectionPoint - position), color, reflectionCoef);
				}
			}
		}
	}

	//std::cout << "not inside bounds" << std::endl;
	return nullptr;
}
