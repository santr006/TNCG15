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
	iterationStep = 0.1f;
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
	iterationStep = 0.1f;
}

Camera::~Camera(){}

//creates a ray from the position pos in the direction dir
//and returns the color the path of the ray defines
glm::vec3 Camera::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	//create a color: black
	glm::vec3 result(0.f, 0.f, 0.f);

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
			glm::vec3 objectIntersectionPoint;

			//if it did check the objects inside it
			if (theWorld->BBoxList.at(i)->intersects(r, iterationStep, boxIntersectionPoint))
			{
				//create a new ray from the intersection point
				Ray newRay(boxIntersectionPoint, r.direction);

				//check if the ray hits any of the objects inside the bounding box
				for (unsigned int j = 0; j < theWorld->BBoxList.at(i)->objects.size(); j++)
				{
					//if it did the color will be white, if not the color is still black
					if (theWorld->BBoxList.at(i)->objects.at(j)->testRayIntersection(newRay, iterationStep, objectIntersectionPoint))
					{
						result = glm::vec3(1.f, 1.f, 1.f); //std::cout << "hit" << std::endl;
					}
					//use object intersection point in a later stage
				}
			}
		}
	}
	else //check if the ray hits any object
	{
		glm::vec3 objectIntersectionPoint(0);
		Intersection* closestInsersection = nullptr;

		for (unsigned int o = 0; o < theWorld->objectList.size(); o++)
		{
			// A new dawn.
			Intersection* current;
			current = theWorld->objectList.at(o)->testRayIntersection(r, iterationStep, objectIntersectionPoint);
			// nullptr is returned if t is larger than the ray's tMax

			if (current != nullptr)
			{
				closestInsersection = current;
			}
		}

		if (closestInsersection != nullptr){
			// result = closestInsersection->color;
			result = generateShadowRay(closestInsersection);
		}
	}
	return result;
}

glm::vec3 Camera::generateShadowRay(Intersection* i)
{
	glm::vec3 dirToLight;

	Light* currentLight;
	Object3D* currentObject;

	bool isObscured;

	float localIllumination;
	glm::vec3 resultColor(0.f, 0.f, 0.f);

	// Temporary variables. To be removed later, pendejo!
	float stepSize = 0.f;
	glm::vec3 intersectionPoint(0.f);

	for (int l = 0; l < theWorld->lightList.size(); l++)
	{
		currentLight = theWorld->lightList.at(l);
		dirToLight = glm::normalize(currentLight->position - i->point);

		Ray shadowRay(i->point, dirToLight, 0.1f, glm::length(dirToLight));
		Intersection* obscuringIntersection = nullptr;

		isObscured = false;

		// go through all objects in the scene
		for (int o = 0; o < theWorld->objectList.size(); o++)
		{
			currentObject = theWorld->objectList.at(o);
			obscuringIntersection = currentObject->testRayIntersection(shadowRay, stepSize, intersectionPoint);

			// if object is hit between surface point and the light source
			if (obscuringIntersection != nullptr){
				isObscured = true;
				//resultColor = in->color
			}
		}

		// if no object is between surface point and light source
		if (!isObscured)
		{
			// calculate the light locally

			localIllumination = std::max(0.f, glm::dot(i->surfaceNormal, shadowRay.direction)); // diffuse reflection factor
			//std::cout << glm::dot(i->surfaceNormal, shadowRay.direction) << std::endl;
			resultColor = resultColor + i->color*(currentLight->intensity*localIllumination);

			//resultColor = resultColor + glm::vec3(0.1f, 0.1f, 0.1f);
			// dumb ambient
			//system("pause");

			//std::cout << resultColor.r << " " << resultColor.g << " " << resultColor.b << std::endl;
		}
	}
	return resultColor;
}

glm::vec3 Camera::phongShading(Object3D* o, Light* l, Intersection* i)
{
	/*
	glm::vec3 ambient = glm::vec3(o->ka) * l->intensity;
	glm::vec3 diffuse = glm::vec3(o->kd) * glm::dot(L,N) * o->col
	*/
	return glm::vec3(0.f);
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

			//save the colors in the image
			im.setPixel(j, i, generateRay(rayStart, rayDir));
		}
	}

	//save the image to a file
	std::string fileName = "render.ppm";
	im.saveAsPPM(fileName.c_str());
}