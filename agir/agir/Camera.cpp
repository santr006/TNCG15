#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0, 0, 0);
	upDirection = glm::vec3(0, 1, 0);
	lookAtDirection = glm::vec3(0, 0, -1);
	nearPlane = 1;
	farPlane = 2;
	widthInPixels = 1;
	heightInPixels = 1;
	iterationStep = 0.1f;
}

Camera::~Camera(){}

//creates a ray from the position pos in the direction dir
//and returns the color the path of the ray defines
Rgb Camera::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	//create a color: black
	Rgb p(0);

	//Send a ray from pos in the direction dir
	Ray r(pos, dir);

	//did the ray hit anything?

	//checks if the given ray intersects any object in the scene
	//check if the ray hits any bounding box
	for (int i = 0; i < theWorld.BBoxList.size(); i++)
	{
		glm::vec3 boxIntersectionPoint;
		glm::vec3 objectIntersectionPoint;

		//if it did check the objects inside it
		if (theWorld.BBoxList.at(i)->intersects(r, iterationStep, boxIntersectionPoint))
		{
			//create a new ray from the intersection point
			Ray newRay(boxIntersectionPoint, r.direction);

			//check if the ray hits any of the objects inside the bounding box
			for (int j = 0; j < theWorld.BBoxList.at(i)->objects.size(); j++)
			{
				//if it did the color will be white, if not the color is still black
				if (theWorld.BBoxList.at(i)->objects.at(j)->testRayIntersection(newRay, iterationStep, objectIntersectionPoint))
					p.setRGB(255);
					//use object intersection point in a later stage
			}
		}
	}

	return p;
}

//uses ray tracing to find a color for every pixel
void Camera::render()
{
	//create the picture to be rendered to
	Image im(widthInPixels, heightInPixels);
	
	//place near cutting plane
	glm::vec3 center = position + lookAtDirection * nearPlane;
	float height = heightInPixels * STEP_BETWEEN_PIXELS;
	float width = widthInPixels * STEP_BETWEEN_PIXELS;
	glm::vec3 rightDir = glm::normalize(glm::cross(lookAtDirection, upDirection));

	glm::vec3 rightUpperCorner = center + height*0.5f * upDirection + width*0.5f * rightDir;
	glm::vec3 leftUpperCorner = center + height*0.5f * upDirection - width*0.5f * rightDir;
	glm::vec3 rightLowerCorner = center - height*0.5f * upDirection + width*0.5f * rightDir;
	glm::vec3 leftLowerCorner = center - height*0.5f * upDirection - width*0.5f * rightDir;

	//give a color to each pixel
	for (unsigned int i = 0; i < heightInPixels; i++)
	{
		for (unsigned int j = 0; j < widthInPixels; j++)
		{
			//to decide the color for this pixel send a ray from it
			glm::vec3 rayStart(leftUpperCorner.x + STEP_BETWEEN_PIXELS * j, leftUpperCorner.y - STEP_BETWEEN_PIXELS * i, leftUpperCorner.z);
			glm::vec3 rayDir = rayStart - position; //the pixel's position - the cameras position
			//save the colors in the image
//			im.setPixel(j, i, &generateRay(rayStart, rayDir));
		}
	}

	//save the image to a file
	std::string fileName = "render.ppm";
	im.saveAsPPM(fileName.c_str());
}