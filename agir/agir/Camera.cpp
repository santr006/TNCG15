#include "Camera.h"

Camera::Camera(World* w)
{
	theWorld = w;
	position = glm::vec3(0, 0, 0);
	upDirection = glm::vec3(0, 1, 0);
	lookAtDirection = glm::vec3(0, 0, -1);
	nearPlane = 1;
	farPlane = 2;
	widthInPixels = 1;
	heightInPixels = 1;
}

Camera::Camera(World* w, int width, int height)
{
	theWorld = w;
	position = glm::vec3(0, 0, 0);
	upDirection = glm::vec3(0, 1, 0);
	lookAtDirection = glm::vec3(0, 0, -1);
	nearPlane = 1;
	farPlane = 2;
	widthInPixels = width;
	heightInPixels = height;
}

Camera::~Camera(){}

//creates a ray from the position pos in the direction dir
//and returns the color the path of the ray defines
glm::vec3 Camera::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	//create a color: black
	glm::vec3 p(0, 0, 0);

	//Send a ray from pos in the direction dir
	Ray r(pos, dir);

	//did the ray hit anything?

	//checks if the given ray intersects any object in the scene

	//check if the ray hits any bounding box
	//if there are bounding boxes in the scene
	if (theWorld->BBoxList.size() != 0) //check if the ray hits any bounding box
	{
		for (unsigned int i = 0; i < theWorld->BBoxList.size(); i++)
		{
			glm::vec3 boxIntersectionPoint;
			float iterationStep = 0.1f;

			//if it did check the objects inside it
			if (theWorld->BBoxList.at(i)->intersects(r, iterationStep, boxIntersectionPoint))
			{
				//create a new ray from the intersection point
				Ray newRay(boxIntersectionPoint, r.direction);

				//check if the ray hits any of the objects inside the bounding box
				for (unsigned int j = 0; j < theWorld->BBoxList.at(i)->objects.size(); j++)
				{
					//if it did the color will be white, if not the color is still black
					if (theWorld->BBoxList.at(i)->objects.at(j)->testRayIntersection(newRay))
					{
						p = glm::vec3(1.f, 1.f, 1.f); //std::cout << "hit" << std::endl;
					}
					//use object intersection point in a later stage
				}
			}
		}
	}
	else //check if the ray hits any object
	{
		Intersection* closestIntersection = nullptr;

		for (unsigned int j = 0; j < theWorld->objectList.size(); j++)
		{
			Intersection* current = nullptr;
			current = theWorld->objectList.at(j)->testRayIntersection(r);

			if (current != nullptr)
			{
				closestIntersection = current;
			}
		}

		if (closestIntersection != nullptr)
			p = closestIntersection->color;
	}

	return p;
}

//uses ray tracing to find a color for every pixel
void Camera::render()
{
	//create the picture to be rendered to
	Image im(widthInPixels, heightInPixels);
	std::cout << "created image" << std::endl;

	//place near cutting plane
	glm::vec3 center = position + lookAtDirection * nearPlane;
	float height = heightInPixels * STEP_BETWEEN_PIXELS;
	float width = widthInPixels * STEP_BETWEEN_PIXELS;
	glm::vec3 rightDir = glm::normalize(glm::cross(lookAtDirection, upDirection));

	glm::vec3 rightUpperCorner = center + height*0.5f * upDirection + width*0.5f * rightDir;
	glm::vec3 leftUpperCorner = center + height*0.5f * upDirection - width*0.5f * rightDir;
	glm::vec3 rightLowerCorner = center - height*0.5f * upDirection + width*0.5f * rightDir;
	glm::vec3 leftLowerCorner = center - height*0.5f * upDirection - width*0.5f * rightDir;

	std::cout << "rightUpperCorner (" << rightUpperCorner.x << ", " << rightUpperCorner.y << ", " << rightUpperCorner.z << ")" << std::endl;
	std::cout << "leftUpperCorner (" << leftUpperCorner.x << ", " << leftUpperCorner.y << ", " << leftUpperCorner.z << ")" << std::endl;
	std::cout << "rightLowerCorner (" << rightLowerCorner.x << ", " << rightLowerCorner.y << ", " << rightLowerCorner.z << ")" << std::endl;
	std::cout << "leftLowerCorner (" << leftLowerCorner.x << ", " << leftLowerCorner.y << ", " << leftLowerCorner.z << ")" << std::endl;

	//give a color to each pixel
	for (unsigned int i = 0; i < heightInPixels; i++)
	{
		for (unsigned int j = 0; j < widthInPixels; j++)
		{
			//to decide the color for this pixel send a ray from it
			glm::vec3 rayStart(STEP_BETWEEN_PIXELS*0.5f + leftUpperCorner.x + STEP_BETWEEN_PIXELS * j, -STEP_BETWEEN_PIXELS*0.5f + leftUpperCorner.y - STEP_BETWEEN_PIXELS * i, leftUpperCorner.z);
			glm::vec3 rayDir = rayStart - position; //the pixel's position - the cameras position

			//std::cout << "setting pixel in (" << rayStart.x << ", " << rayStart.y << ", " << rayStart.z << ")" << std::endl;
			//std::cout << "dir (" << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << ")" << std::endl;

			//save the colors in the image
			im.setPixel(j, i, generateRay(rayStart, rayDir));
		}
	}

	//save the image to a file
	std::string fileName = "a.ppm";
	im.saveAsPPM(fileName.c_str());
}