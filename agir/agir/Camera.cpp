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
}

Camera::~Camera(){}

//creates a ray from the position pos in the direction dir
//and returns the color the path of the ray defines
Rgb Camera::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	//create a color
	Rgb p(0);

	//Send a ray from pos in the direction dir
	Ray r(pos, dir);

	//did the ray hit anything?
	//if it did the color will be white, if not the color is still black
	if (r.rayIntersect())
		p.setRGB(255);

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
			//save the colors in the image
			im.setPixel(j, i, &generateRay(rayStart, lookAtDirection));
		}
	}

	//save the image to a file
	std::string fileName = "render.ppm";
	im.saveAsPPM(fileName.c_str());
}